#include "state.h"
#include <common/events/event.h>
#include <QDebug>

void State::onEntry(QEvent *event)
{
    qDebug() << __func__ << objectName() << event->type();

    emit entered(event);
}

void State::onExit(QEvent *event)
{
    qDebug() << __func__ << objectName() << event->type();

    emit exited(event);
}
