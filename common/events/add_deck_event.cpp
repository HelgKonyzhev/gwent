#include "add_deck_event.h"

AddDeckEvent::AddDeckEvent()
    : Event{Event::AddDeck}
{
    qRegisterMetaType<AddDeckEvent>();
}

AddDeckEvent::AddDeckEvent(const QString& name, const QString& fraction, const QStringList& cards)
    : Event{Event::AddDeck}
    , m_name{name}
    , m_fraction{fraction}
    , m_cards{cards}
{
    qRegisterMetaType<AddDeckEvent>();
}

REGISTER_EVENT(AddDeckEvent)

DeckAddedEvent::DeckAddedEvent()
    : Event{Event::DeckAdded}
{
    qRegisterMetaType<DeckAddedEvent>();
}

DeckAddedEvent::DeckAddedEvent(const AddDeckEvent* event)
    : Event{Event::DeckAdded}
    , m_name{event->name()}
    , m_fraction{event->fraction()}
    , m_cards{event->cards()}
{
    qRegisterMetaType<DeckAddedEvent>();
}

REGISTER_EVENT(DeckAddedEvent)

DeckAddFailedEvent::DeckAddFailedEvent()
    : Event{Event::DeckAddFailed}
{
    qRegisterMetaType<DeckAddFailedEvent>();
}

DeckAddFailedEvent::DeckAddFailedEvent(const QString& name, const QString& reason)
    : Event{Event::DeckAddFailed}
    , m_name{name}
    , m_reason{reason}
{
    qRegisterMetaType<DeckAddFailedEvent>();
}

REGISTER_EVENT(DeckAddFailedEvent)
