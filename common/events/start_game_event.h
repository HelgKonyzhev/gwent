#pragma once
#include "event.h"
#include <QtQml/qqmlregistration.h>

class StartGameEvent : public Event
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString opponent READ opponent WRITE setOpponent)

public:
    StartGameEvent();
    StartGameEvent(const QString& opponent);

    void setOpponent(const QString& player) { m_opponent = player; }
    const QString& opponent() const { return m_opponent; }

private:
    QString m_opponent;
};

DECLARE_EVENT(StartGameEvent, Event::StartGame);

class GameStartedEvent : public Event
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString opponent READ opponent WRITE setOpponent)

public:
    GameStartedEvent();
    GameStartedEvent(const QString& opponent);

    void setOpponent(const QString& player) { m_opponent = player; }
    const QString& opponent() const { return m_opponent; }

private:
    QString m_opponent;
};

DECLARE_EVENT(GameStartedEvent, Event::GameStarted);

class GameStartFailedEvent : public Event
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString reason READ reason WRITE setReason)

public:
    GameStartFailedEvent();
    GameStartFailedEvent(const QString& reason);

    void setReason(const QString& reason) { m_reason = reason; }
    const QString& reason() const { return m_reason; }

private:
    QString m_reason;
};

DECLARE_EVENT(GameStartFailedEvent, Event::GameStartFailed);
