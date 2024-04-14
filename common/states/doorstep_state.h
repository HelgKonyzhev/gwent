#pragma once
#include <QtQmlIntegration/qqmlintegration.h>
#include <common/states/state.h>

class Socket;

class DoorstepState : public State
{
    Q_OBJECT
    QML_ELEMENT

public:
    DoorstepState(Socket* socket, QState* parent);

private:
    Socket* m_socket;
};

Q_DECLARE_METATYPE(DoorstepState);
