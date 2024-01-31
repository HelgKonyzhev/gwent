#include "request_game_event.h"
#include <QJsonObject>

RequestGameEvent::RequestGameEvent()
    : Event{RequestGame}
{
}

RequestGameEvent::RequestGameEvent(const QString& opponent)
    : Event{RequestGame}
    , m_opponent{opponent}
{
}

Result RequestGameEvent::parse(const QJsonObject& eventJs)
{
    if (!eventJs.contains("opponent"))
        return ResultError{"\'opponent\' not specified"};

    if (!eventJs["opponent"].isString())
        return ResultError{"\'opponent\' expected to be string"};

    m_opponent = eventJs["opponent"].toString();
    return {};
}

QJsonObject RequestGameEvent::toJson() const
{
    auto json = Event::toJson();
    json.insert("opponent", m_opponent);
    return json;
}

GameAcceptedEvent::GameAcceptedEvent()
    : Event{GameAccepted}
{
}

GameAcceptedEvent::GameAcceptedEvent(const QString& opponent)
    : Event{GameAccepted}
    , m_opponent{opponent}
{
}

Result GameAcceptedEvent::parse(const QJsonObject& eventJs)
{
    if (!eventJs.contains("opponent"))
        return ResultError{"\'opponent\' not specified"};

    if (!eventJs["opponent"].isString())
        return ResultError{"\'opponent\' expected to be string"};

    m_opponent = eventJs["opponent"].toString();
    return {};
}

QJsonObject GameAcceptedEvent::toJson() const
{
    auto json = Event::toJson();
    json.insert("opponent", m_opponent);
    return json;
}
