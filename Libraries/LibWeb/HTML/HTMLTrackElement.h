/*
 * Copyright (c) 2020, the SerenityOS developers.
 * Copyright (c) 2024, Jamie Mansfield <jmansfield@cadixdev.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <LibWeb/Forward.h>
#include <LibWeb/HTML/HTMLElement.h>

namespace Web::HTML {

class HTMLTrackElement final : public HTMLElement {
    WEB_PLATFORM_OBJECT(HTMLTrackElement, HTMLElement);
    GC_DECLARE_ALLOCATOR(HTMLTrackElement);

public:
    virtual ~HTMLTrackElement() override;

    WebIDL::UnsignedShort ready_state();

    GC::Ref<TextTrack> track() { return *m_track; }

private:
    HTMLTrackElement(DOM::Document&, DOM::QualifiedName);

    virtual void initialize(JS::Realm&) override;
    virtual void visit_edges(Cell::Visitor&) override;

    String track_url() const { return m_track_url; }
    void set_track_url(String);

    void start_the_track_processing_model();
    void start_the_track_processing_model_parallel_steps();

    void track_became_ready();
    void track_failed_to_load();

    // ^DOM::Element
    virtual void attribute_changed(FlyString const& name, Optional<String> const& old_value, Optional<String> const& value, Optional<FlyString> const& namespace_) override;
    virtual void inserted() override;

    GC::Ptr<TextTrack> m_track;
    GC::Ptr<TextTrackObserver> m_track_observer;

    // https://html.spec.whatwg.org/multipage/media.html#track-url
    String m_track_url {};

    GC::Ptr<Fetch::Infrastructure::FetchAlgorithms> m_fetch_algorithms;
    GC::Ptr<Fetch::Infrastructure::FetchController> m_fetch_controller;

    bool m_loading { false };
    bool m_awaiting_track_url_change { false };
};

}
