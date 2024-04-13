#include "deck_edition_failed.h"
#include <common/player_data.h>
#include <QJsonObject>

DeckEditionFailedEvent::DeckEditionFailedEvent()
    : Event{Event::DeckEditionFailed}
{
    qRegisterMetaType<DeckEditionFailedEvent>();
}

DeckEditionFailedEvent::DeckEditionFailedEvent(const QString& name, const QString& reason)
    : Event{Event::DeckEditionFailed}
    , m_name{name}
    , m_reason{reason}
{
    qRegisterMetaType<DeckEditionFailedEvent>();
}

REGISTER_EVENT(DeckEditionFailedEvent)
