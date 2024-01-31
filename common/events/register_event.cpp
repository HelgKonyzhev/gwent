#include "register_event.h"
#include <QJsonObject>

RegisterEvent::RegisterEvent()
    : Event{Event::Register}
{
}

RegisterEvent::RegisterEvent(const QString& username, const QString& password)
    : Event{Event::Register}
    , m_username{username}
    , m_password{password}
{
}

Result RegisterEvent::parse(const QJsonObject& eventJs)
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

QJsonObject RegisterEvent::toJson() const
{
    auto eventJs = Event::toJson();
    eventJs.insert("username", {m_username});
    eventJs.insert("password", {m_password});
    return eventJs;
}

RegisteredEvent::RegisteredEvent()
    : Event{Event::Registered}
{
}

RegisteredEvent::RegisteredEvent(const QString& username)
    : Event{Event::Registered}
    , m_username{username}
{
}

Result RegisteredEvent::parse(const QJsonObject& eventJs)
{
    if (!eventJs.contains("username"))
        return ResultError{"\'username\' not specified"};

    if (!eventJs["username"].isString())
        return ResultError{"\'username\' must be string"};

    m_username = eventJs["username"].toString();
    return {};
}

QJsonObject RegisteredEvent::toJson() const
{
    auto eventJs = Event::toJson();
    eventJs.insert("username", {m_username});
    return eventJs;
}

RegistrationFailedEvent::RegistrationFailedEvent()
    : Event{Event::RegistrationFailed}
{
}

RegistrationFailedEvent::RegistrationFailedEvent(const QString& username, const QString& reason)
    : Event{Event::RegistrationFailed}
    , m_username{username}
    , m_reason{reason}
{
}

Result RegistrationFailedEvent::parse(const QJsonObject& eventJs)
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

QJsonObject RegistrationFailedEvent::toJson() const
{
    auto eventJs = Event::toJson();
    eventJs.insert("username", {m_username});
    eventJs.insert("reason", {m_reason});
    return eventJs;
}
