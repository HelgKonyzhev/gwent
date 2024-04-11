#include "register_event.h"
#include <QJsonObject>

RegisterEvent::RegisterEvent()
    : Event{Event::Register}
{
    qRegisterMetaType<RegisterEvent>();
}

RegisterEvent::RegisterEvent(const QString& username, const QString& password)
    : Event{Event::Register}
    , m_username{username}
    , m_password{password}
{
    qRegisterMetaType<RegisterEvent>();
}

REGISTER_EVENT(RegisterEvent)

RegisteredEvent::RegisteredEvent()
    : Event{Event::Registered}
{
    qRegisterMetaType<RegisteredEvent>();
}

RegisteredEvent::RegisteredEvent(const QString& username)
    : Event{Event::Registered}
    , m_username{username}
{
    qRegisterMetaType<RegisteredEvent>();
}

REGISTER_EVENT(RegisteredEvent)

RegistrationFailedEvent::RegistrationFailedEvent()
    : Event{Event::RegistrationFailed}
{
    qRegisterMetaType<RegistrationFailedEvent>();
}

RegistrationFailedEvent::RegistrationFailedEvent(const QString& username, const QString& reason)
    : Event{Event::RegistrationFailed}
    , m_username{username}
    , m_reason{reason}
{
    qRegisterMetaType<RegistrationFailedEvent>();
}

REGISTER_EVENT(RegistrationFailedEvent)
