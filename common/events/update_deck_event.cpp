#include "update_deck_event.h"

UpdateDeckEvent::UpdateDeckEvent()
    : Event{Event::UpdateDeck}
{
    qRegisterMetaType<UpdateDeckEvent>();
}

UpdateDeckEvent::UpdateDeckEvent(const QString& name, const QString& fraction, const QStringList& cards)
    : Event{Event::UpdateDeck}
    , m_name{name}
    , m_fraction{fraction}
    , m_cards{cards}
{
    qRegisterMetaType<UpdateDeckEvent>();
}

REGISTER_EVENT(UpdateDeckEvent)

DeckUpdatedEvent::DeckUpdatedEvent()
    : Event{Event::DeckUpdated}
{
    qRegisterMetaType<DeckUpdatedEvent>();
}

DeckUpdatedEvent::DeckUpdatedEvent(const UpdateDeckEvent* event)
    : Event{Event::DeckUpdated}
    , m_name{event->name()}
    , m_fraction{event->fraction()}
    , m_cards{event->cards()}
{
    qRegisterMetaType<DeckUpdatedEvent>();
}

REGISTER_EVENT(DeckUpdatedEvent)

DeckUpdateFailedEvent::DeckUpdateFailedEvent()
    : Event{Event::DeckUpdateFailed}
{
    qRegisterMetaType<DeckUpdateFailedEvent>();
}

DeckUpdateFailedEvent::DeckUpdateFailedEvent(const QString& name, const QString& reason)
    : Event{Event::DeckUpdateFailed}
    , m_name{name}
    , m_reason{reason}
{
    qRegisterMetaType<DeckUpdateFailedEvent>();
}

REGISTER_EVENT(DeckUpdateFailedEvent)
