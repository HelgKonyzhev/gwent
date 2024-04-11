#include "login_event.h"
#include <QJsonObject>

LogInEvent::LogInEvent()
    : Event{Event::LogIn}
{
    qRegisterMetaType<LogInEvent>();
}

LogInEvent::LogInEvent(const QString& username, const QString& password)
    : Event{Event::LogIn}
    , m_username{username}
    , m_password{password}
{
    qRegisterMetaType<LogInEvent>();
}

REGISTER_EVENT(LogInEvent)

LoggedInEvent::LoggedInEvent()
    : Event{Event::LoggedIn}
{
    qRegisterMetaType<LoggedInEvent>();
}

LoggedInEvent::LoggedInEvent(QSharedPointer<PlayerData> playerData)
    : Event{Event::LoggedIn}
    , m_playerData{playerData}
{
    qRegisterMetaType<LoggedInEvent>();
}

Result LoggedInEvent::parse(const QVariantHash& eventData)
{
    if (!eventData.contains("player_data"))
        return ResultError{"no \'player_data\' specified"};

    if (!eventData["player_data"].canConvert<QVariantHash>())
        return ResultError{"\'player_data\' expected to be object"};

    m_playerData.reset(new PlayerData);
    auto parsed = m_playerData->fromVariant(eventData["player_data"].toHash());
    if (!parsed)
    {
        m_playerData.reset();
        return ResultError{parsed.error()};
    }

    return {};
}

QJsonObject LoggedInEvent::toJson() const
{
    auto eventJs = Event::toJson();
    if (m_playerData)
        eventJs.insert("player_data", m_playerData->toJson());
    return eventJs;
}

REGISTER_EVENT(LoggedInEvent)

LogInFailedEvent::LogInFailedEvent()
    : Event{Event::LogInFailed}
{
    qRegisterMetaType<LogInFailedEvent>();
}

LogInFailedEvent::LogInFailedEvent(const QString& username, const QString& reason)
    : Event{Event::LogInFailed}
    , m_username{username}
    , m_reason{reason}
{
    qRegisterMetaType<LogInFailedEvent>();
}

REGISTER_EVENT(LogInFailedEvent)
