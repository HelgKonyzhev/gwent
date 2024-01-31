#include "start_game_event.h"
#include <QJsonObject>

StartGameEvent::StartGameEvent()
    : Event{StartGame}
{
}

StartGameEvent::StartGameEvent(const QString& opponent)
    : Event{StartGame}
    , m_opponent{opponent}
{
}

Result StartGameEvent::parse(const QJsonObject& eventJs)
{
    if (!eventJs.contains("opponent"))
        return ResultError{"\'opponent\' not specified"};

    if (!eventJs["opponent"].isString())
        return ResultError{"\'opponent\' expected to be string"};

    m_opponent = eventJs["opponent"].toString();
    return {};
}

QJsonObject StartGameEvent::toJson() const
{
    auto json = Event::toJson();
    json.insert("opponent", m_opponent);
    return json;
}

GameStartedEvent::GameStartedEvent()
    : Event{GameStarted}
{
}

GameStartedEvent::GameStartedEvent(const QString& opponent)
    : Event{GameStarted}
    , m_opponent{opponent}
{
}

Result GameStartedEvent::parse(const QJsonObject& eventJs)
{
    if (!eventJs.contains("opponent"))
        return ResultError{"\'opponent\' not specified"};

    if (!eventJs["opponent"].isString())
        return ResultError{"\'opponent\' expected to be string"};

    m_opponent = eventJs["opponent"].toString();
    return {};
}

QJsonObject GameStartedEvent::toJson() const
{
    auto json = Event::toJson();
    json.insert("opponent", m_opponent);
    return json;
}

GameStartFailedEvent::GameStartFailedEvent()
    : Event{GameStartFailed}
{
}

GameStartFailedEvent::GameStartFailedEvent(const QString& reason)
    : Event{GameStartFailed}
    , m_reason{reason}
{
}

Result GameStartFailedEvent::parse(const QJsonObject& eventJs)
{
    //    if (!eventJs.contains("player1"))
    //        return ResultError{"\'player1\' not specified"};

    //    if (!eventJs["player1"].isString())
    //        return ResultError{"\'player1\' expected to be string"};

    //    m_player1 = eventJs["player1"].toString();

    //    if (!eventJs.contains("player2"))
    //        return ResultError{"\'player2\' not specified"};

    //    if (!eventJs["player2"].isString())
    //        return ResultError{"\'player2\' expected to be string"};

    //    m_player2 = eventJs["player2"].toString();
    return {};
}

QJsonObject GameStartFailedEvent::toJson() const
{
    auto json = Event::toJson();
    //    json.insert("player1", m_player1);
    //    json.insert("player2", m_player2);
    return json;
}
