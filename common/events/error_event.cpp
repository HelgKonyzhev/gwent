#include "error_event.h"
#include <QJsonObject>

ErrorEvent::ErrorEvent()
    : Event{Event::Error}
{
}

ErrorEvent::ErrorEvent(const QString& username)
    : Event{Event::Error}
    , m_description{username}
{
}

Result ErrorEvent::parse(const QJsonObject& eventJs)
{
    if (!eventJs.contains("description"))
        return ResultError{"\'description\' not specified"};

    m_description = eventJs["description"].toString();
    return {};
}

QJsonObject ErrorEvent::toJson() const
{
    auto eventJs = Event::toJson();
    eventJs.insert("description", {m_description});
    return eventJs;
}
