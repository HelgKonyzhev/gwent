#pragma once
#include <QtQmlIntegration/qqmlintegration.h>
#include <common/state.h>

class Socket;
class RegisterEvent;
class RegisteredEvent;
class RegistrationFailedEvent;
class LogInEvent;
class LogInFailedEvent;

Q_DECLARE_OPAQUE_POINTER(RegisterEvent*);
Q_DECLARE_OPAQUE_POINTER(RegisteredEvent*);
Q_DECLARE_OPAQUE_POINTER(RegistrationFailedEvent*);
Q_DECLARE_OPAQUE_POINTER(LogInEvent*);
Q_DECLARE_OPAQUE_POINTER(LogInFailedEvent*);

class DoorstepState : public State
{
    Q_OBJECT
    QML_ELEMENT

public:
    DoorstepState(Socket* socket, QState* parent);

signals:
    Q_INVOKABLE void registration(RegisterEvent* event);
    Q_INVOKABLE void registered(RegisteredEvent* event);
    Q_INVOKABLE void registrationFailed(RegistrationFailedEvent* event);
    Q_INVOKABLE void logging(LogInEvent* event);
    Q_INVOKABLE void loginFailed(LogInFailedEvent* event);

private:
    Socket* m_socket;
};

Q_DECLARE_METATYPE(DoorstepState);
