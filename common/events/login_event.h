#pragma once
#include "event.h"
#include <common/player_data.h>
#include <QMetaType>
#include <QtQml/qqmlregistration.h>
#include <QSharedPointer>

class LogInEvent : public Event
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString username READ username WRITE setUsername)
    Q_PROPERTY(QString password READ password WRITE setPassword);

public:
    LogInEvent();
    LogInEvent(const QString& username, const QString& password);

    void setUsername(const QString& username) { m_username = username; }
    const QString& username() const { return m_username; }

    void setPassword(const QString& password) { m_password = password; }
    const QString& password() const { return m_password; }

private:
    QString m_username;
    QString m_password;
};

DECLARE_EVENT(LogInEvent, Event::LogIn);

class LoggedInEvent : public Event
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QSharedPointer<PlayerData> playerData READ playerData WRITE setPlayerData)

public:
    LoggedInEvent();
    LoggedInEvent(QSharedPointer<PlayerData> playerData);

    void setPlayerData(QSharedPointer<PlayerData> playerData) { m_playerData = playerData; }
    QSharedPointer<PlayerData> playerData() const { return m_playerData; }

    Result parse(const QVariantHash& eventData) override;
    QJsonObject toJson() const override;

private:
    QSharedPointer<PlayerData> m_playerData;
};

DECLARE_EVENT(LoggedInEvent, Event::LoggedIn);

class LogInFailedEvent : public Event
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString username READ username WRITE setUsername)
    Q_PROPERTY(QString reason READ reason WRITE setReason);

public:
    LogInFailedEvent();
    LogInFailedEvent(const QString& username, const QString& reason);

    void setUsername(const QString& username) { m_username = username; }
    const QString& username() const { return m_username; }

    void setReason(const QString& reason) { m_reason = reason; }
    const QString& reason() const { return m_reason; }

private:
    QString m_username;
    QString m_reason;
};

DECLARE_EVENT(LogInFailedEvent, Event::LogInFailed);
