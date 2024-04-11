#pragma once
#include <QObject>
#include <common/player_data.h>
#include <QMetaType>
#include <QSharedPointer>

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

Q_DECLARE_OPAQUE_POINTER(DoorstepState*);
Q_DECLARE_OPAQUE_POINTER(LobbyState*);

class Player : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QSharedPointer<PlayerData> data READ data CONSTANT)
    Q_PROPERTY(DoorstepState* doorstepState READ doorstepState CONSTANT)
    Q_PROPERTY(LobbyState* lobbyState READ lobbyState CONSTANT)

public:
    Player(Socket* socket, QObject* parent = nullptr);

    const Socket* socket() const { return m_socket; }
    void postEvent(Event* event);

    bool inLobby() const { return m_inLobby; }
    const QSharedPointer<PlayerData> data() const { return m_data; }
    const QStringList& playersInLobby() const { return m_playersInLobby; }

    DoorstepState* doorstepState() const { return m_doorstepState; }
    LobbyState* lobbyState() const { return m_lobbyState; }

    Q_INVOKABLE void login(const QString& username, const QString& password);
    Q_INVOKABLE void registerPlayer(const QString& username, const QString& password);

signals:
    Q_INVOKABLE void loggedIn(const LoggedInEvent*);
    Q_INVOKABLE void loginFailed(const LogInFailedEvent*);
    Q_INVOKABLE void registered(const RegisteredEvent*);
    Q_INVOKABLE void registrationFailed(const RegistrationFailedEvent*);

private:
    QStateMachine* m_fsm;
    Socket* m_socket;
    bool m_inLobby = false;
    QSharedPointer<PlayerData> m_data;
    QStringList m_playersInLobby;
    DoorstepState* m_doorstepState;
    LobbyState* m_lobbyState;
};

Q_DECLARE_METATYPE(Player);
