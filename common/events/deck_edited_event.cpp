#include "deck_edited_event.h"
#include <common/player_data.h>
#include <QJsonObject>

DeckEditedEvent::DeckEditedEvent()
    : Event{Event::DeckEdited}
{
    qRegisterMetaType<DeckEditedEvent>();
}

DeckEditedEvent::DeckEditedEvent(const QString& name, QSharedPointer<PlayerData> playerData)
    : Event{Event::DeckEdited}
    , m_name{name}
    , m_playerData{playerData}
{
    qRegisterMetaType<DeckEditedEvent>();
}

Result DeckEditedEvent::parse(const QVariantHash& eventData)
{
    if (!eventData.contains("player_data"))
        return ResultError{"no \'player_data\' specified"};

    if (!eventData["player_data"].canConvert<QVariantHash>())
        return ResultError{"\'player_data\' expected to be object"};

    m_playerData.reset(new PlayerData);
    auto parsed = m_playerData->fromVariant(eventData["player_data"].toHash());
    if (!parsed)
    {
        m_playerData.reset();
        return ResultError{parsed.error()};
    }

    return {};
}

QJsonObject DeckEditedEvent::toJson() const
{
    auto eventJs = Event::toJson();
    if (m_playerData)
        eventJs.insert("player_data", m_playerData->toJson());
    return eventJs;
}

REGISTER_EVENT(DeckEditedEvent)
