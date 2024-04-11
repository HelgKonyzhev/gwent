#include "request_game_event.h"
#include <QJsonObject>

RequestGameEvent::RequestGameEvent()
    : Event{RequestGame}
{
    qRegisterMetaType<RequestGameEvent>();
}

RequestGameEvent::RequestGameEvent(const QString& opponent)
    : Event{RequestGame}
    , m_opponent{opponent}
{
    qRegisterMetaType<RequestGameEvent>();
}

REGISTER_EVENT(RequestGameEvent)

GameAcceptedEvent::GameAcceptedEvent()
    : Event{GameAccepted}
{
    qRegisterMetaType<GameAcceptedEvent>();
}

GameAcceptedEvent::GameAcceptedEvent(const QString& opponent)
    : Event{GameAccepted}
    , m_opponent{opponent}
{
    qRegisterMetaType<GameAcceptedEvent>();
}

REGISTER_EVENT(GameAcceptedEvent)
