/*
 * Copyright (c) 2022, Andreas Kling <andreas@ladybird.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <LibWeb/Bindings/Intrinsics.h>
#include <LibWeb/Bindings/SubmitEventPrototype.h>
#include <LibWeb/HTML/SubmitEvent.h>

namespace Web::HTML {

GC_DEFINE_ALLOCATOR(SubmitEvent);

GC::Ref<SubmitEvent> SubmitEvent::create(JS::Realm& realm, FlyString const& event_name, SubmitEventInit const& event_init)
{
    return realm.create<SubmitEvent>(realm, event_name, event_init);
}

WebIDL::ExceptionOr<GC::Ref<SubmitEvent>> SubmitEvent::construct_impl(JS::Realm& realm, FlyString const& event_name, SubmitEventInit const& event_init)
{
    return create(realm, event_name, event_init);
}

SubmitEvent::SubmitEvent(JS::Realm& realm, FlyString const& event_name, SubmitEventInit const& event_init)
    : DOM::Event(realm, event_name, event_init)
    , m_submitter(event_init.submitter)
{
}

SubmitEvent::~SubmitEvent() = default;

void SubmitEvent::initialize(JS::Realm& realm)
{
    WEB_SET_PROTOTYPE_FOR_INTERFACE(SubmitEvent);
    Base::initialize(realm);
}

void SubmitEvent::visit_edges(Cell::Visitor& visitor)
{
    Base::visit_edges(visitor);
    visitor.visit(m_submitter);
}

}
