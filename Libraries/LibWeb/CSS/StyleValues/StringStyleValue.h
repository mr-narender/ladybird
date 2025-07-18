/*
 * Copyright (c) 2022-2024, Sam Atkins <atkinssj@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/FlyString.h>
#include <LibWeb/CSS/CSSStyleValue.h>
#include <LibWeb/CSS/Serialize.h>

namespace Web::CSS {

class StringStyleValue : public StyleValueWithDefaultOperators<StringStyleValue> {
public:
    static ValueComparingNonnullRefPtr<StringStyleValue const> create(FlyString const& string)
    {
        return adopt_ref(*new (nothrow) StringStyleValue(string));
    }
    virtual ~StringStyleValue() override = default;

    FlyString const& string_value() const { return m_string; }
    virtual String to_string(SerializationMode) const override { return serialize_a_string(m_string); }
    virtual Vector<Parser::ComponentValue> tokenize() const override
    {
        return { Parser::Token::create_string(m_string) };
    }

    bool properties_equal(StringStyleValue const& other) const { return m_string == other.m_string; }

private:
    explicit StringStyleValue(FlyString const& string)
        : StyleValueWithDefaultOperators(Type::String)
        , m_string(string)
    {
    }

    FlyString m_string;
};

}
