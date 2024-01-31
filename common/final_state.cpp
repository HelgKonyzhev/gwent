#include "final_state.h"
#include <common/events/event.h>
#include <QDebug>

void FinalState::onEntry(QEvent *event)
{
    qDebug() << __func__ << objectName() << event->type();

    QFinalState::onEntry(event);

    if (event->type() < QEvent::Type::User)
        return;

    emit entered(static_cast<Event *>(event));
}

void FinalState::onExit(QEvent *event)
{
    qDebug() << __func__ << objectName() << event->type();

    QFinalState::onExit(event);

    if (event->type() < QEvent::Type::User)
        return;

    emit exited(static_cast<Event *>(event));
}
