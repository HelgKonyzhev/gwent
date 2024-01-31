#pragma once
#include <common/state.h>

class Socket;
class RegisterEvent;
class RegisteredEvent;
class RegistrationFailedEvent;
class LogInEvent;
// class LoggedInEvent;
class LogInFailedEvent;

class ThresholdState : public State
{
    Q_OBJECT

public:
    ThresholdState(Socket* socket, QState* parent);

signals:
    void registration(RegisterEvent* event);
    void registered(RegisteredEvent* event);
    void registrationFailed(RegistrationFailedEvent* event);
    void logging(LogInEvent* event);
    void loginFailed(LogInFailedEvent* event);
    //    void loggedIn(LoggedInEvent* event);

private:
    Socket* m_socket;
};
