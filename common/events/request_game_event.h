#pragma once
#include "event.h"

class RequestGameEvent : public Event
{
public:
    RequestGameEvent();
    RequestGameEvent(const QString& opponent);

    void setOpponent(const QString& player) { m_opponent = player; }
    const QString& opponent() const { return m_opponent; }

    Result parse(const QJsonObject& eventJs) override;
    QJsonObject toJson() const override;

private:
    QString m_opponent;
};

TO_TYPE_ID(RequestGameEvent, Event::RequestGame);

class GameAcceptedEvent : public Event
{
public:
    GameAcceptedEvent();
    GameAcceptedEvent(const QString& opponent);

    void setOpponent(const QString& player) { m_opponent = player; }
    const QString& opponent() const { return m_opponent; }

    Result parse(const QJsonObject& eventJs) override;
    QJsonObject toJson() const override;

private:
    QString m_opponent;
};

TO_TYPE_ID(GameAcceptedEvent, Event::GameAccepted);
