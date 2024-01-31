#pragma once
#include "event.h"

class StartGameEvent : public Event
{
public:
    StartGameEvent();
    StartGameEvent(const QString& opponent);

    void setOpponent(const QString& player) { m_opponent = player; }
    const QString& opponent() const { return m_opponent; }

    Result parse(const QJsonObject& eventJs) override;
    QJsonObject toJson() const override;

private:
    QString m_opponent;
};

TO_TYPE_ID(StartGameEvent, Event::StartGame);

class GameStartedEvent : public Event
{
public:
    GameStartedEvent();
    GameStartedEvent(const QString& opponent);

    void setOpponent(const QString& player) { m_opponent = player; }
    const QString& opponent() const { return m_opponent; }

    Result parse(const QJsonObject& eventJs) override;
    QJsonObject toJson() const override;

private:
    QString m_opponent;
};

TO_TYPE_ID(GameStartedEvent, Event::GameStarted);

class GameStartFailedEvent : public Event
{
public:
    GameStartFailedEvent();
    GameStartFailedEvent(const QString& reason);

    void setReason(const QString& reason) { m_reason = reason; }
    const QString& reason() const { return m_reason; }

    Result parse(const QJsonObject& eventJs) override;
    QJsonObject toJson() const override;

private:
    QString m_reason;
};

TO_TYPE_ID(GameStartFailedEvent, Event::GameStartFailed);
