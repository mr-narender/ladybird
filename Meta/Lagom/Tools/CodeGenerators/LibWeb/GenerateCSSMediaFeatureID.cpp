/*
 * Copyright (c) 2022-2023, Sam Atkins <atkinssj@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "GeneratorUtil.h"
#include <AK/SourceGenerator.h>
#include <AK/StringBuilder.h>
#include <LibCore/ArgsParser.h>
#include <LibMain/Main.h>

ErrorOr<void> generate_header_file(JsonObject& media_feature_data, Core::File& file);
ErrorOr<void> generate_implementation_file(JsonObject& media_feature_data, Core::File& file);

ErrorOr<int> ladybird_main(Main::Arguments arguments)
{
    StringView generated_header_path;
    StringView generated_implementation_path;
    StringView media_features_json_path;

    Core::ArgsParser args_parser;
    args_parser.add_option(generated_header_path, "Path to the MediaFeatureID header file to generate", "generated-header-path", 'h', "generated-header-path");
    args_parser.add_option(generated_implementation_path, "Path to the MediaFeatureID implementation file to generate", "generated-implementation-path", 'c', "generated-implementation-path");
    args_parser.add_option(media_features_json_path, "Path to the JSON file to read from", "json-path", 'j', "json-path");
    args_parser.parse(arguments);

    auto json = TRY(read_entire_file_as_json(media_features_json_path));
    VERIFY(json.is_object());
    auto media_feature_data = json.as_object();

    auto generated_header_file = TRY(Core::File::open(generated_header_path, Core::File::OpenMode::Write));
    auto generated_implementation_file = TRY(Core::File::open(generated_implementation_path, Core::File::OpenMode::Write));

    TRY(generate_header_file(media_feature_data, *generated_header_file));
    TRY(generate_implementation_file(media_feature_data, *generated_implementation_file));

    return 0;
}

ErrorOr<void> generate_header_file(JsonObject& media_feature_data, Core::File& file)
{
    StringBuilder builder;
    SourceGenerator generator { builder };

    generator.set("media_feature_id_underlying_type", underlying_type_for_enum(media_feature_data.size()));

    generator.append(R"~~~(#pragma once

#include <AK/StringView.h>
#include <AK/Traits.h>
#include <LibWeb/CSS/Keyword.h>

namespace Web::CSS {

enum class MediaFeatureValueType {
    Boolean,
    Integer,
    Length,
    Ratio,
    Resolution,
};

enum class MediaFeatureID : @media_feature_id_underlying_type@ {)~~~");

    media_feature_data.for_each_member([&](auto& name, auto&) {
        auto member_generator = generator.fork();
        member_generator.set("name:titlecase", title_casify(name));
        member_generator.append(R"~~~(
    @name:titlecase@,)~~~");
    });

    generator.append(R"~~~(
};

Optional<MediaFeatureID> media_feature_id_from_string(StringView);
StringView string_from_media_feature_id(MediaFeatureID);

bool media_feature_type_is_range(MediaFeatureID);
bool media_feature_accepts_type(MediaFeatureID, MediaFeatureValueType);
bool media_feature_accepts_keyword(MediaFeatureID, Keyword);

bool media_feature_keyword_is_falsey(MediaFeatureID, Keyword);

}
)~~~");

    TRY(file.write_until_depleted(generator.as_string_view().bytes()));
    return {};
}

ErrorOr<void> generate_implementation_file(JsonObject& media_feature_data, Core::File& file)
{
    StringBuilder builder;
    SourceGenerator generator { builder };
    generator.append(R"~~~(
#include <LibWeb/CSS/MediaFeatureID.h>
#include <LibWeb/Infra/Strings.h>

namespace Web::CSS {

Optional<MediaFeatureID> media_feature_id_from_string(StringView string)
{)~~~");

    media_feature_data.for_each_member([&](auto& name, auto&) {
        auto member_generator = generator.fork();
        member_generator.set("name", name);
        member_generator.set("name:titlecase", title_casify(name));
        member_generator.append(R"~~~(
    if (string.equals_ignoring_ascii_case("@name@"sv))
        return MediaFeatureID::@name:titlecase@;
)~~~");
    });

    generator.append(R"~~~(
    return {};
}

StringView string_from_media_feature_id(MediaFeatureID media_feature_id)
{
    switch (media_feature_id) {)~~~");

    media_feature_data.for_each_member([&](auto& name, auto&) {
        auto member_generator = generator.fork();
        member_generator.set("name", name);
        member_generator.set("name:titlecase", title_casify(name));
        member_generator.append(R"~~~(
    case MediaFeatureID::@name:titlecase@:
        return "@name@"sv;)~~~");
    });

    generator.append(R"~~~(
    }
    VERIFY_NOT_REACHED();
}

bool media_feature_type_is_range(MediaFeatureID media_feature_id)
{
    switch (media_feature_id) {)~~~");

    media_feature_data.for_each_member([&](auto& name, auto& value) {
        VERIFY(value.is_object());
        auto& feature = value.as_object();

        auto member_generator = generator.fork();
        member_generator.set("name:titlecase", title_casify(name));
        VERIFY(feature.has("type"sv));
        auto feature_type = feature.get_string("type"sv);
        VERIFY(feature_type.has_value());
        member_generator.set("is_range", feature_type.value() == "range"sv ? "true"_string : "false"_string);
        member_generator.append(R"~~~(
    case MediaFeatureID::@name:titlecase@:
        return @is_range@;)~~~");
    });

    generator.append(R"~~~(
    }
    VERIFY_NOT_REACHED();
}

bool media_feature_accepts_type(MediaFeatureID media_feature_id, MediaFeatureValueType value_type)
{
    switch (media_feature_id) {)~~~");

    media_feature_data.for_each_member([&](auto& name, auto& member) {
        VERIFY(member.is_object());
        auto& feature = member.as_object();

        auto member_generator = generator.fork();
        member_generator.set("name:titlecase", title_casify(name));
        member_generator.append(R"~~~(
    case MediaFeatureID::@name:titlecase@:)~~~");

        bool have_output_value_type_switch = false;
        if (feature.has("values"sv)) {
            auto append_value_type_switch_if_needed = [&] {
                if (!have_output_value_type_switch) {
                    member_generator.append(R"~~~(
        switch (value_type) {)~~~");
                }
                have_output_value_type_switch = true;
            };
            auto values = feature.get_array("values"sv);
            VERIFY(values.has_value());
            auto& values_array = values.value();
            for (auto& type : values_array.values()) {
                VERIFY(type.is_string());
                auto type_name = type.as_string();
                // Skip keywords.
                if (!type_name.starts_with('<'))
                    continue;
                if (type_name == "<mq-boolean>") {
                    append_value_type_switch_if_needed();
                    member_generator.append(R"~~~(
        case MediaFeatureValueType::Boolean:
            return true;)~~~");
                } else if (type_name == "<integer>") {
                    append_value_type_switch_if_needed();
                    member_generator.append(R"~~~(
        case MediaFeatureValueType::Integer:
            return true;)~~~");
                } else if (type_name == "<length>") {
                    append_value_type_switch_if_needed();
                    member_generator.append(R"~~~(
        case MediaFeatureValueType::Length:
            return true;)~~~");
                } else if (type_name == "<ratio>") {
                    append_value_type_switch_if_needed();
                    member_generator.append(R"~~~(
        case MediaFeatureValueType::Ratio:
            return true;)~~~");
                } else if (type_name == "<resolution>") {
                    append_value_type_switch_if_needed();
                    member_generator.append(R"~~~(
        case MediaFeatureValueType::Resolution:
            return true;)~~~");
                } else {
                    warnln("Unrecognized media-feature value type: `{}`", type_name);
                    VERIFY_NOT_REACHED();
                }
            }
        }
        if (have_output_value_type_switch) {
            member_generator.append(R"~~~(
        default:
            return false;
        })~~~");
        } else {
            member_generator.append(R"~~~(
        return false;)~~~");
        }
    });

    generator.append(R"~~~(
    }
    VERIFY_NOT_REACHED();
}

bool media_feature_accepts_keyword(MediaFeatureID media_feature_id, Keyword keyword)
{
    switch (media_feature_id) {)~~~");

    media_feature_data.for_each_member([&](auto& name, auto& member) {
        VERIFY(member.is_object());
        auto& feature = member.as_object();

        auto member_generator = generator.fork();
        member_generator.set("name:titlecase", title_casify(name));
        member_generator.append(R"~~~(
    case MediaFeatureID::@name:titlecase@:)~~~");

        bool have_output_keyword_switch = false;
        if (feature.has("values"sv)) {
            auto append_keyword_switch_if_needed = [&] {
                if (!have_output_keyword_switch) {
                    member_generator.append(R"~~~(
        switch (keyword) {)~~~");
                }
                have_output_keyword_switch = true;
            };
            auto values = feature.get_array("values"sv);
            VERIFY(values.has_value());
            auto& values_array = values.value();
            for (auto& keyword : values_array.values()) {
                VERIFY(keyword.is_string());
                auto const& keyword_name = keyword.as_string();
                // Skip types.
                if (keyword_name.starts_with('<'))
                    continue;
                append_keyword_switch_if_needed();

                auto keyword_generator = member_generator.fork();
                keyword_generator.set("keyword:titlecase", title_casify(keyword_name));
                keyword_generator.append(R"~~~(
        case Keyword::@keyword:titlecase@:
            return true;)~~~");
            }
        }
        if (have_output_keyword_switch) {
            member_generator.append(R"~~~(
        default:
            return false;
        })~~~");
        } else {
            member_generator.append(R"~~~(
        return false;)~~~");
        }
    });

    generator.append(R"~~~(
    }
    VERIFY_NOT_REACHED();
}

bool media_feature_keyword_is_falsey(MediaFeatureID media_feature_id, Keyword keyword)
{
    switch (media_feature_id) {)~~~");
    media_feature_data.for_each_member([&](auto& name, JsonValue const& feature_value) {
        VERIFY(feature_value.is_object());
        auto& feature = feature_value.as_object();
        auto false_keywords = feature.get_array("false-keywords"sv);
        if (!false_keywords.has_value() || false_keywords->is_empty())
            return;

        auto member_generator = generator.fork();
        member_generator.set("name:titlecase", title_casify(name));
        member_generator.append(R"~~~(
    case MediaFeatureID::@name:titlecase@:
        switch (keyword) {)~~~");

        false_keywords.value().for_each([&](JsonValue const& value) {
            auto value_generator = member_generator.fork();
            member_generator.set("false_keyword:titlecase", title_casify(value.as_string()));
            member_generator.append(R"~~~(
        case Keyword::@false_keyword:titlecase@:)~~~");
        });
        member_generator.append(R"~~~(
            return true;
        default:
            return false;
        })~~~");
    });

    generator.append(R"~~~(
    default:
        return false;
    }
}

}
)~~~");

    TRY(file.write_until_depleted(generator.as_string_view().bytes()));
    return {};
}
