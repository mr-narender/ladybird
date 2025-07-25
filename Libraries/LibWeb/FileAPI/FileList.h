/*
 * Copyright (c) 2022, Andrew Kaster <akaster@serenityos.org>
 * Copyright (c) 2023, Luke Wilde <lukew@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Vector.h>
#include <LibGC/Ptr.h>
#include <LibWeb/Bindings/PlatformObject.h>
#include <LibWeb/FileAPI/File.h>
#include <LibWeb/WebIDL/Types.h>

namespace Web::FileAPI {

class FileList
    : public Bindings::PlatformObject
    , public Bindings::Serializable {
    WEB_PLATFORM_OBJECT(FileList, Bindings::PlatformObject);
    GC_DECLARE_ALLOCATOR(FileList);

public:
    [[nodiscard]] static GC::Ref<FileList> create(JS::Realm&);

    void add_file(GC::Ref<File> file) { m_files.append(file); }

    virtual ~FileList() override;

    // https://w3c.github.io/FileAPI/#dfn-length
    WebIDL::UnsignedLong length() const { return m_files.size(); }

    // https://w3c.github.io/FileAPI/#dfn-item
    File* item(size_t index)
    {
        return index < m_files.size() ? m_files[index].ptr() : nullptr;
    }

    // https://w3c.github.io/FileAPI/#dfn-item
    File const* item(size_t index) const
    {
        return index < m_files.size() ? m_files[index].ptr() : nullptr;
    }

    virtual Optional<JS::Value> item_value(size_t index) const override;

    virtual HTML::SerializeType serialize_type() const override { return HTML::SerializeType::FileList; }
    virtual WebIDL::ExceptionOr<void> serialization_steps(HTML::TransferDataEncoder&, bool for_storage, HTML::SerializationMemory&) override;
    virtual WebIDL::ExceptionOr<void> deserialization_steps(HTML::TransferDataDecoder&, HTML::DeserializationMemory&) override;

private:
    explicit FileList(JS::Realm&);

    virtual void initialize(JS::Realm&) override;
    virtual void visit_edges(Cell::Visitor&) override;

    Vector<GC::Ref<File>> m_files;
};

}
