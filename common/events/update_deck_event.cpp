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
