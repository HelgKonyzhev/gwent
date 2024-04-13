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
