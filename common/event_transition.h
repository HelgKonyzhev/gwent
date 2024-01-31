#pragma once
#include <common/events/event.h>
#include <QAbstractTransition>

class QAbstractState;
class Socket;

class EventTransition : public QAbstractTransition
{
    Q_OBJECT

public:
    EventTransition(Socket *socket, Event::Type eventType);
    EventTransition(Socket *socket, Event::Type eventType, QAbstractState *targetState);

signals:
    void triggered(Event *event);

private:
    bool eventTest(QEvent *event) override;
    void onTransition(QEvent *event) override;

    Socket *m_socket;
    const Event::Type m_eventType;
};
