#pragma once
#include "event.h"
#include <QStringList>
#include <QtQml/qqmlregistration.h>

class LobbyUpdatedEvent : public Event
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QStringList players READ players WRITE setPlayers)

public:
    LobbyUpdatedEvent();
    LobbyUpdatedEvent(const QStringList& players);

    void setPlayers(const QStringList& players) { m_players = players; }
    const QStringList& players() const { return m_players; }

private:
    QStringList m_players;
};

DECLARE_EVENT(LobbyUpdatedEvent, Event::LobbyUpdated);

class UpdateLobbyEvent : public Event
{
    Q_OBJECT
    QML_ELEMENT

public:
    UpdateLobbyEvent();
};

DECLARE_EVENT(UpdateLobbyEvent, Event::UpdateLobby);
