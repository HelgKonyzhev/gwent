#pragma once
#include "event.h"
#include <QtQml/qqmlregistration.h>

class RegisterEvent : public Event
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString username READ username WRITE setUsername)
    Q_PROPERTY(QString password READ password WRITE setPassword);

public:
    RegisterEvent();
    RegisterEvent(const QString& username, const QString& password);

    void setUsername(const QString& username) { m_username = username; }
    const QString& username() const { return m_username; }

    void setPassword(const QString& password) { m_password = password; }
    const QString& password() const { return m_password; }

private:
    QString m_username;
    QString m_password;
};

DECLARE_EVENT(RegisterEvent, Event::Register);

class RegisteredEvent : public Event
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString username READ username WRITE setUsername)

public:
    RegisteredEvent();
    RegisteredEvent(const QString& username);

    void setUsername(const QString& username) { m_username = username; }
    const QString& username() const { return m_username; }

private:
    QString m_username;
};

DECLARE_EVENT(RegisteredEvent, Event::Registered);

class RegistrationFailedEvent : public Event
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString username READ username WRITE setUsername)
    Q_PROPERTY(QString reason READ reason WRITE setReason);

public:
    RegistrationFailedEvent();
    RegistrationFailedEvent(const QString& username, const QString& reason);

    void setUsername(const QString& username) { m_username = username; }
    const QString& username() const { return m_username; }

    void setReason(const QString& reason) { m_reason = reason; }
    const QString& reason() const { return m_reason; }

private:
    QString m_username;
    QString m_reason;
};

DECLARE_EVENT(RegistrationFailedEvent, Event::RegistrationFailed);
