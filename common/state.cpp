#include "state.h"
#include <common/events/event.h>
#include <QDebug>
#include <common/socket.h>

void State::onEntry(QEvent *event)
{
    qDebug() << __func__ << objectName() << event->type();

    if (event->type() < QEvent::Type::User)
        return;

    emit entered(static_cast<Event *>(event));
}

void State::onExit(QEvent *event)
{
    qDebug() << __func__ << objectName() << event->type();

    if (event->type() < QEvent::Type::User)
        return;

    emit exited(static_cast<Event *>(event));
}
