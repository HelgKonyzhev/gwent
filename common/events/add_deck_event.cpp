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
