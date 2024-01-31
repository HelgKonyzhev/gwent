#pragma once
#include "event.h"
#include <common/player_data.h>

class LogInEvent : public Event
{
public:
    LogInEvent();
    LogInEvent(const QString& username, const QString& password);

    void setUsername(const QString& username) { m_username = username; }
    const QString& username() const { return m_username; }

    void setPassword(const QString& password) { m_password = password; }
    const QString& password() const { return m_password; }

    Result parse(const QJsonObject& eventJs) override;
    QJsonObject toJson() const override;

private:
    QString m_username;
    QString m_password;
};

TO_TYPE_ID(LogInEvent, Event::LogIn);

class LoggedInEvent : public Event
{
public:
    LoggedInEvent();
    LoggedInEvent(const PlayerData& playerData);

    void setPlayerData(const PlayerData& playerData) { m_playerData = playerData; }
    const PlayerData& playerData() const { return m_playerData; }

    Result parse(const QJsonObject& eventJs) override;
    QJsonObject toJson() const override;

private:
    PlayerData m_playerData;
};

TO_TYPE_ID(LoggedInEvent, Event::LoggedIn);

class LogInFailedEvent : public Event
{
public:
    LogInFailedEvent();
    LogInFailedEvent(const QString& username, const QString& reason);

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

TO_TYPE_ID(LogInFailedEvent, Event::LogInFailed);
