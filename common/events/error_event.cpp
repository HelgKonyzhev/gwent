#include "error_event.h"
#include <QJsonObject>

ErrorEvent::ErrorEvent()
    : Event{Event::Error}
{
    qRegisterMetaType<ErrorEvent>();
}

ErrorEvent::ErrorEvent(const QString& description)
    : Event{Event::Error}
    , m_description{description}
{
    qRegisterMetaType<ErrorEvent>();
}

REGISTER_EVENT(ErrorEvent)
