#pragma once
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

class LobbyState : public State
{
    Q_OBJECT

public:
    LobbyState(Socket* socket, QState* parent);

signals:
    void loggedIn(LoggedInEvent* event);
    void updatingLobby(UpdateLobbyEvent* event);
    void lobbyUpdated(LobbyUpdatedEvent* event);
    void startingGame(StartGameEvent* event);
    void gameStartFailed(GameStartFailedEvent* event);
    void gameStarted(GameStartedEvent* event);
    void gameRequested(RequestGameEvent* event);
    void gameAccepted(GameAcceptedEvent* event);

private:
    Socket* m_socket;
};
