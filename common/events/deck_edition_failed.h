#pragma once
#include <QtQmlIntegration/qqmlintegration.h>
#include <common/events/event.h>

class PlayerData;

class DeckEditionFailedEvent : public Event
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(const QString& name READ name WRITE setName)
    Q_PROPERTY(const QString& reason READ reason WRITE setReason)

public:
    DeckEditionFailedEvent();
    DeckEditionFailedEvent(const QString& name, const QString& reason);

    void setName(const QString& name) { m_name = name; }
    const QString& name() const { return m_name; }

    void setReason(const QString& reason) { m_reason = reason; }
    const QString& reason() const { return m_reason; }

private:
    QString m_name;
    QString m_reason;
};

DECLARE_EVENT(DeckEditionFailedEvent, Event::DeckEditionFailed)
