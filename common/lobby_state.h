#pragma once
#include <QtQmlIntegration/qqmlintegration.h>
#include <common/state.h>

class Socket;
class UpdateLobbyEvent;
class LobbyUpdatedEvent;
class StartGameEvent;
class GameStartFailedEvent;
class RequestGameEvent;
class GameAcceptedEvent;
class GameStartedEvent;
class LoggedInEvent;

Q_DECLARE_OPAQUE_POINTER(UpdateLobbyEvent*);
Q_DECLARE_OPAQUE_POINTER(LobbyUpdatedEvent*);
Q_DECLARE_OPAQUE_POINTER(StartGameEvent*);
Q_DECLARE_OPAQUE_POINTER(GameStartFailedEvent*);
Q_DECLARE_OPAQUE_POINTER(RequestGameEvent*);
Q_DECLARE_OPAQUE_POINTER(GameAcceptedEvent*);
Q_DECLARE_OPAQUE_POINTER(GameStartedEvent*);
Q_DECLARE_OPAQUE_POINTER(LoggedInEvent*);

class LobbyState : public State
{
    Q_OBJECT
    QML_ELEMENT

public:
    LobbyState(Socket* socket, QState* parent);

signals:
    Q_INVOKABLE void loggedIn(LoggedInEvent* event);
    Q_INVOKABLE void updatingLobby(UpdateLobbyEvent* event);
    Q_INVOKABLE void lobbyUpdated(LobbyUpdatedEvent* event);
    Q_INVOKABLE void startingGame(StartGameEvent* event);
    Q_INVOKABLE void gameStartFailed(GameStartFailedEvent* event);
    Q_INVOKABLE void gameStarted(GameStartedEvent* event);
    Q_INVOKABLE void gameRequested(RequestGameEvent* event);
    Q_INVOKABLE void gameAccepted(GameAcceptedEvent* event);

private:
    Socket* m_socket;
};

Q_DECLARE_METATYPE(LobbyState);
