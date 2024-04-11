#include "lobby_updated_event.h"
#include <QJsonObject>
#include <QJsonArray>

LobbyUpdatedEvent::LobbyUpdatedEvent()
    : Event{LobbyUpdated}
{
    qRegisterMetaType<LobbyUpdatedEvent>();
}

LobbyUpdatedEvent::LobbyUpdatedEvent(const QStringList& players)
    : Event{LobbyUpdated}
    , m_players{players}
{
}

REGISTER_EVENT(LobbyUpdatedEvent)

UpdateLobbyEvent::UpdateLobbyEvent()
    : Event{UpdateLobby}
{
    qRegisterMetaType<UpdateLobbyEvent>();
}

REGISTER_EVENT(UpdateLobbyEvent)
