#pragma once
#include <QtQmlIntegration/qqmlintegration.h>
#include <common/events/event.h>

class PlayerData;

class DeckEditedEvent : public Event
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QSharedPointer<PlayerData> playerData READ playerData WRITE setPlayerData)

public:
    DeckEditedEvent();
    DeckEditedEvent(const QString& name, QSharedPointer<PlayerData> playerData);

    void setName(const QString& name) { m_name = name; }
    const QString& name() const { return m_name; }

    void setPlayerData(QSharedPointer<PlayerData> playerData) { m_playerData = playerData; }
    QSharedPointer<PlayerData> playerData() const { return m_playerData; }

    Result parse(const QVariantHash& eventData) override;
    QJsonObject toJson() const override;

private:
    QString m_name;
    QSharedPointer<PlayerData> m_playerData;
};

DECLARE_EVENT(DeckEditedEvent, Event::DeckEdited)
