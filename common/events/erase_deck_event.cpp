#include "erase_deck_event.h"

EraseDeckEvent::EraseDeckEvent()
    : Event{Event::EraseDeck}
{
    qRegisterMetaType<EraseDeckEvent>();
}

EraseDeckEvent::EraseDeckEvent(const QString& name)
    : Event{Event::EraseDeck}
    , m_name{name}
{
    qRegisterMetaType<EraseDeckEvent>();
}

REGISTER_EVENT(EraseDeckEvent)

DeckErasedEvent::DeckErasedEvent()
    : Event{Event::DeckErased}
{
    qRegisterMetaType<DeckErasedEvent>();
}

DeckErasedEvent::DeckErasedEvent(const QString& name)
    : Event{Event::DeckErased}
    , m_name{name}
{
    qRegisterMetaType<DeckErasedEvent>();
}

REGISTER_EVENT(DeckErasedEvent)

DeckEraseFailedEvent::DeckEraseFailedEvent()
    : Event{Event::DeckEraseFailed}
{
    qRegisterMetaType<DeckEraseFailedEvent>();
}

DeckEraseFailedEvent::DeckEraseFailedEvent(const QString& name, const QString& reason)
    : Event{Event::DeckEraseFailed}
    , m_name{name}
    , m_reason{reason}
{
    qRegisterMetaType<DeckEraseFailedEvent>();
}

REGISTER_EVENT(DeckEraseFailedEvent)
