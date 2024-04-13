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
