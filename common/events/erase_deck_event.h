#pragma once
#include <QtQmlIntegration/qqmlintegration.h>
#include <common/events/event.h>

class EraseDeckEvent : public Event
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString name READ name WRITE setName)

public:
    EraseDeckEvent();
    EraseDeckEvent(const QString& name);

    void setName(const QString& name) { m_name = name; }
    const QString& name() const { return m_name; }

private:
    QString m_name;
};

DECLARE_EVENT(EraseDeckEvent, Event::EraseDeck);

class DeckErasedEvent : public Event
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString name READ name WRITE setName)

public:
    DeckErasedEvent();
    DeckErasedEvent(const QString& name);

    void setName(const QString& name) { m_name = name; }
    const QString& name() const { return m_name; }

private:
    QString m_name;
};

DECLARE_EVENT(DeckErasedEvent, Event::DeckErased);

class DeckEraseFailedEvent : public Event
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString reason READ reason WRITE setReason)

public:
    DeckEraseFailedEvent();
    DeckEraseFailedEvent(const QString& name, const QString& reason);

    void setName(const QString& name) { m_name = name; }
    const QString& name() const { return m_name; }

    void setReason(const QString& reason) { m_reason = reason; }
    const QString& reason() const { return m_reason; }

private:
    QString m_name;
    QString m_reason;
};

DECLARE_EVENT(DeckEraseFailedEvent, Event::DeckEraseFailed);
