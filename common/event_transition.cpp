#include "event_transition.h"
#include <QState>
#include <QDebug>
#include <common/socket.h>

EventTransition::EventTransition(Socket *socket, Event::Type eventType)
    : m_socket{socket}
    , m_eventType{eventType}
{
}

EventTransition::EventTransition(Socket *socket, Event::Type eventType, QAbstractState *targetState)
    : m_socket{socket}
    , m_eventType{eventType}
{
    setTargetState(targetState);
}

bool EventTransition::eventTest(QEvent *event) { return event->type() == m_eventType; }

void EventTransition::onTransition(QEvent *event)
{
    auto e = castToEvent(event);
    if (e)
    {
        qDebug() << __func__ << "from" << sourceState()->objectName() << "to"
                 << (targetState() ? targetState()->objectName() : sourceState()->objectName()) << "by event"
                 << event->type();
        emit triggered(e);

        if (!e->isAccepted())
            m_socket->sendEvent(*e);
    }
}
