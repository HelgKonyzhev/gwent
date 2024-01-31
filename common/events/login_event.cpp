#include "login_event.h"
#include <QJsonObject>

LogInEvent::LogInEvent()
    : Event{Event::LogIn}
{
}

LogInEvent::LogInEvent(const QString& username, const QString& password)
    : Event{Event::LogIn}
    , m_username{username}
    , m_password{password}
{
}

Result LogInEvent::parse(const QJsonObject& eventJs)
{
    if (!eventJs.contains("username"))
        return ResultError{"\'username\' not specified"};

    if (!eventJs["username"].isString())
        return ResultError{"\'username\' must be string"};

    if (!eventJs.contains("password"))
        return ResultError{"\'password\' not specified"};

    if (!eventJs["password"].isString())
        return ResultError{"\'password\' must be string"};

    m_username = eventJs["username"].toString();
    m_password = eventJs["password"].toString();
    return {};
}

QJsonObject LogInEvent::toJson() const
{
    auto eventJs = Event::toJson();
    eventJs.insert("username", {m_username});
    eventJs.insert("password", {m_password});
    return eventJs;
}

LoggedInEvent::LoggedInEvent()
    : Event{Event::LoggedIn}
{
}

LoggedInEvent::LoggedInEvent(const PlayerData& playerData)
    : Event{Event::LoggedIn}
    , m_playerData{playerData}
{
}

Result LoggedInEvent::parse(const QJsonObject& eventJs)
{
    if (!eventJs.contains("player_data"))
        return ResultError{"no \'player_data\' specified"};

    if (!eventJs["player_data"].isObject())
        return ResultError{"no \'player_data\' expected to be object"};

    return m_playerData.fromJson(eventJs["player_data"].toObject());
}

QJsonObject LoggedInEvent::toJson() const
{
    auto eventJs = Event::toJson();
    eventJs.insert("player_data", m_playerData.toJson());
    return eventJs;
}

LogInFailedEvent::LogInFailedEvent()
    : Event{Event::LogInFailed}
{
}

LogInFailedEvent::LogInFailedEvent(const QString& username, const QString& reason)
    : Event{Event::LogInFailed}
    , m_username{username}
    , m_reason{reason}
{
}

Result LogInFailedEvent::parse(const QJsonObject& eventJs)
{
    if (!eventJs.contains("username"))
        return ResultError{"\'username\' not specified"};

    if (!eventJs["username"].isString())
        return ResultError{"\'username\' must be string"};

    m_username = eventJs["username"].toString();

    if (!eventJs.contains("reason"))
        return ResultError{"\'reason\' not specified"};

    if (!eventJs["reason"].isString())
        return ResultError{"\'reason\' must be string"};

    m_reason = eventJs["reason"].toString();
    return {};
}

QJsonObject LogInFailedEvent::toJson() const
{
    auto eventJs = Event::toJson();
    eventJs.insert("username", {m_username});
    eventJs.insert("reason", {m_reason});
    return eventJs;
}
