#pragma once
#include "event.h"

class ErrorEvent : public Event
{
public:
    ErrorEvent();
    ErrorEvent(const QString& description);

    void setDescription(const QString& description) { m_description = description; }
    const QString& description() const { return m_description; }

private:
    QString m_description;
};

DECLARE_EVENT(ErrorEvent, Event::Error);
