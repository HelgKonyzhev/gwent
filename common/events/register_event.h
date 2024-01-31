#pragma once
#include "event.h"

class RegisterEvent : public Event
{
public:
    RegisterEvent();
    RegisterEvent(const QString& username, const QString& password);

    void setUsername(const QString& username) { m_username = username; }
    const QString& username() const { return m_username; }

    void setPassword(const QString& password) { m_password = password; }
    const QString& password() const { return m_password; }

    Result parse(const QJsonObject& eventJs);
    QJsonObject toJson() const;

private:
    QString m_username;
    QString m_password;
};

TO_TYPE_ID(RegisterEvent, Event::Register);

class RegisteredEvent : public Event
{
public:
    RegisteredEvent();
    RegisteredEvent(const QString& username);

    void setUsername(const QString& username) { m_username = username; }
    const QString& username() const { return m_username; }

    Result parse(const QJsonObject& eventJs) override;
    QJsonObject toJson() const override;

private:
    QString m_username;
};

TO_TYPE_ID(RegisteredEvent, Event::Registered);

class RegistrationFailedEvent : public Event
{
public:
    RegistrationFailedEvent();
    RegistrationFailedEvent(const QString& username, const QString& reason);

    void setUsername(const QString& username) { m_username = username; }
    const QString& username() const { return m_username; }

    void setReason(const QString& reason) { m_reason = reason; }
    const QString& reason() const { return m_reason; }

    Result parse(const QJsonObject& eventJs) override;
    QJsonObject toJson() const override;

private:
    QString m_username;
    QString m_reason;
};

TO_TYPE_ID(RegistrationFailedEvent, Event::RegistrationFailed);
