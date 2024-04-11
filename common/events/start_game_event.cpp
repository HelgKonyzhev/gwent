#include "start_game_event.h"
#include <QJsonObject>

StartGameEvent::StartGameEvent()
    : Event{StartGame}
{
    qRegisterMetaType<StartGameEvent>();
}

StartGameEvent::StartGameEvent(const QString& opponent)
    : Event{StartGame}
    , m_opponent{opponent}
{
    qRegisterMetaType<StartGameEvent>();
}

REGISTER_EVENT(StartGameEvent)

GameStartedEvent::GameStartedEvent()
    : Event{GameStarted}
{
    qRegisterMetaType<GameStartedEvent>();
}

GameStartedEvent::GameStartedEvent(const QString& opponent)
    : Event{GameStarted}
    , m_opponent{opponent}
{
    qRegisterMetaType<GameStartedEvent>();
}

REGISTER_EVENT(GameStartedEvent)

GameStartFailedEvent::GameStartFailedEvent()
    : Event{GameStartFailed}
{
    qRegisterMetaType<GameStartFailedEvent>();
}

GameStartFailedEvent::GameStartFailedEvent(const QString& reason)
    : Event{GameStartFailed}
    , m_reason{reason}
{
    qRegisterMetaType<GameStartFailedEvent>();
}

REGISTER_EVENT(GameStartFailedEvent)
