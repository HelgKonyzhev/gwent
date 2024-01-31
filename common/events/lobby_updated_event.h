#pragma once
#include "event.h"
#include <QStringList>

class LobbyUpdatedEvent : public Event
{
public:
    LobbyUpdatedEvent();
    LobbyUpdatedEvent(const QStringList& players);

    void setPlayers(const QStringList& players) { m_players = players; }
    const QStringList& players() const { return m_players; }

    Result parse(const QJsonObject& eventJs) override;
    QJsonObject toJson() const override;

private:
    QStringList m_players;
};

TO_TYPE_ID(LobbyUpdatedEvent, Event::LobbyUpdated);

class UpdateLobbyEvent : public Event
{
public:
    UpdateLobbyEvent();

    Result parse(const QJsonObject& eventJs) override;
};

TO_TYPE_ID(UpdateLobbyEvent, Event::UpdateLobby);
