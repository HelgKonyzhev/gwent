#pragma once
#include <QObject>
#include <common/player_data.h>
#include <optional>

class Socket;
class RegisterEvent;
class RegisteredEvent;
class RegistrationFailedEvent;
class LogInEvent;
class LoggedInEvent;
class LogInFailedEvent;
class QStateMachine;
class Event;
class LobbyUpdatedEvent;
class UpdateLobbyEvent;
class StartGameEvent;
class GameStartFailedEvent;
class GameStartedEvent;
class RequestGameEvent;
class GameAcceptedEvent;
class GameDeclinedEvent;
class DoorstepState;
class LobbyState;

class Player : public QObject
{
    Q_OBJECT

public:
    Player(Socket* socket, QObject* parent = nullptr);

    const Socket* socket() const { return m_socket; }
    void postEvent(Event* event);

    bool inLobby() const { return m_inLobby; }
    const std::optional<PlayerData>& data() const { return m_data; }
    const std::optional<QStringList>& playersInLobby() const { return m_playersInLobby; }

    const DoorstepState* thresholdState() const { return m_thresholdState; }
    const LobbyState* lobbyState() const { return m_lobbyState; }

signals:
    //    void registration(RegisterEvent* event);
    //    void registered(RegisteredEvent* event);
    //    void registrationFailed(RegistrationFailedEvent* event);
    //    void logging(LogInEvent* event);
    //    void loggedIn(LoggedInEvent* event);
    //    void loginFailed(LogInFailedEvent* event);
    //    void updatingLobby(UpdateLobbyEvent* event);
    //    void lobbyUpdated(LobbyUpdatedEvent* event);
    //    void startingGame(StartGameEvent* event);
    //    void gameStartFailed(GameStartFailedEvent* event);
    //    void gameStarted(GameStartedEvent* event);
    //    void gameRequested(RequestGameEvent* event);
    //    void gameAccepted(GameAcceptedEvent* event);

private:
    QStateMachine* m_fsm;
    Socket* m_socket;
    bool m_inLobby = false;
    std::optional<PlayerData> m_data;
    std::optional<QStringList> m_playersInLobby;
    DoorstepState* m_thresholdState;
    LobbyState* m_lobbyState;
};
