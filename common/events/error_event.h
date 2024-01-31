#pragma once
#include "event.h"

class ErrorEvent : public Event
{
public:
    ErrorEvent();
    ErrorEvent(const QString& username);

    void setDescription(const QString& username) { m_description = username; }
    const QString& description() const { return m_description; }

    Result parse(const QJsonObject& eventJs);
    QJsonObject toJson() const;

private:
    QString m_description;
};

TO_TYPE_ID(ErrorEvent, Event::LogIn);
