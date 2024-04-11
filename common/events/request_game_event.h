#pragma once
#include "event.h"
#include <QtQml/qqmlregistration.h>

class RequestGameEvent : public Event
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString opponent READ opponent WRITE setOpponent)

public:
    RequestGameEvent();
    RequestGameEvent(const QString& opponent);

    void setOpponent(const QString& player) { m_opponent = player; }
    const QString& opponent() const { return m_opponent; }

private:
    QString m_opponent;
};

DECLARE_EVENT(RequestGameEvent, Event::RequestGame);

class GameAcceptedEvent : public Event
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString opponent READ opponent WRITE setOpponent)

public:
    GameAcceptedEvent();
    GameAcceptedEvent(const QString& opponent);

    void setOpponent(const QString& player) { m_opponent = player; }
    const QString& opponent() const { return m_opponent; }

private:
    QString m_opponent;
};

DECLARE_EVENT(GameAcceptedEvent, Event::GameAccepted);
