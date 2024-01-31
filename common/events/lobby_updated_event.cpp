#include "lobby_updated_event.h"
#include <QJsonObject>
#include <QJsonArray>

LobbyUpdatedEvent::LobbyUpdatedEvent()
    : Event{LobbyUpdated}
{
}

LobbyUpdatedEvent::LobbyUpdatedEvent(const QStringList& players)
    : Event{LobbyUpdated}
    , m_players{players}
{
}

Result LobbyUpdatedEvent::parse(const QJsonObject& eventJs)
{
    if (!eventJs.contains("players"))
        return ResultError{"\'players\' not specified"};

    if (!eventJs["players"].isArray())
        return ResultError{"\'players\' expected to be array"};

    m_players = eventJs["players"].toVariant().toStringList();
    return {};
}

QJsonObject LobbyUpdatedEvent::toJson() const
{
    auto json = Event::toJson();
    json.insert("players", QJsonArray::fromStringList(m_players));
    return json;
}

UpdateLobbyEvent::UpdateLobbyEvent()
    : Event{UpdateLobby}
{
}

Result UpdateLobbyEvent::parse(const QJsonObject& /*eventJs*/) { return {}; }
