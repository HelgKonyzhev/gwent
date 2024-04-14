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
class StateMachine;
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
class Deck;
class DeckAddedEvent;
class DeckUpdatedEvent;
class DeckErasedEvent;
class AddDeckEvent;
class UpdateDeckEvent;
class EraseDeckEvent;
class DeckEditedEvent;
class DeckEditionFailedEvent;

Q_DECLARE_OPAQUE_POINTER(DoorstepState*);
Q_DECLARE_OPAQUE_POINTER(LobbyState*);

Q_DECLARE_OPAQUE_POINTER(RegisterEvent*);
Q_DECLARE_OPAQUE_POINTER(RegisteredEvent*);
Q_DECLARE_OPAQUE_POINTER(RegistrationFailedEvent*);
Q_DECLARE_OPAQUE_POINTER(LogInEvent*);
Q_DECLARE_OPAQUE_POINTER(LogInFailedEvent*);
Q_DECLARE_OPAQUE_POINTER(UpdateLobbyEvent*);
Q_DECLARE_OPAQUE_POINTER(LobbyUpdatedEvent*);
Q_DECLARE_OPAQUE_POINTER(StartGameEvent*);
Q_DECLARE_OPAQUE_POINTER(GameStartFailedEvent*);
Q_DECLARE_OPAQUE_POINTER(RequestGameEvent*);
Q_DECLARE_OPAQUE_POINTER(GameAcceptedEvent*);
Q_DECLARE_OPAQUE_POINTER(GameStartedEvent*);
Q_DECLARE_OPAQUE_POINTER(LoggedInEvent*);
Q_DECLARE_OPAQUE_POINTER(AddDeckEvent*);
Q_DECLARE_OPAQUE_POINTER(UpdateDeckEvent*);
Q_DECLARE_OPAQUE_POINTER(EraseDeckEvent*);
Q_DECLARE_OPAQUE_POINTER(DeckEditedEvent*);
Q_DECLARE_OPAQUE_POINTER(DeckEditionFailedEvent*);

class Player : public QObject
{
    Q_OBJECT
    Q_PROPERTY(const PlayerData* data READ dataPtr CONSTANT)
    Q_PROPERTY(DoorstepState* doorstepState READ doorstepState CONSTANT)
    Q_PROPERTY(LobbyState* lobbyState READ lobbyState CONSTANT)

public:
    Player(Socket* socket, QObject* parent = nullptr);

    const Socket* socket() const { return m_socket; }
    void postEvent(Event* event);

    bool inLobby() const;
    QSharedPointer<PlayerData> data() const { return m_data; }
    const QStringList& playersInLobby() const { return m_playersInLobby; }

    DoorstepState* doorstepState() const { return m_doorstepState; }
    LobbyState* lobbyState() const { return m_lobbyState; }
    StateMachine* fsm() const { return m_fsm; }

signals:
    Q_INVOKABLE void registration(RegisterEvent* event);
    Q_INVOKABLE void registered(RegisteredEvent* event);
    Q_INVOKABLE void registrationFailed(RegistrationFailedEvent* event);
    Q_INVOKABLE void logging(LogInEvent* event);
    Q_INVOKABLE void loginFailed(LogInFailedEvent* event);
    Q_INVOKABLE void loggedIn(LoggedInEvent* event);
    Q_INVOKABLE void updatingLobby(UpdateLobbyEvent* event);
    Q_INVOKABLE void lobbyUpdated(LobbyUpdatedEvent* event);
    Q_INVOKABLE void startingGame(StartGameEvent* event);
    Q_INVOKABLE void gameStartFailed(GameStartFailedEvent* event);
    Q_INVOKABLE void gameStarted(GameStartedEvent* event);
    Q_INVOKABLE void gameRequested(RequestGameEvent* event);
    Q_INVOKABLE void gameAccepted(GameAcceptedEvent* event);
    Q_INVOKABLE void addingDeck(AddDeckEvent*);
    Q_INVOKABLE void updatingDeck(UpdateDeckEvent*);
    Q_INVOKABLE void erasingDeck(EraseDeckEvent*);
    Q_INVOKABLE void deckEdited(DeckEditedEvent*);
    Q_INVOKABLE void deckEditionFailed(DeckEditionFailedEvent*);

private:
    const PlayerData* dataPtr() const { return m_data.get(); }

    StateMachine* m_fsm;
    Socket* m_socket;
    bool m_inLobby = false;
    QSharedPointer<PlayerData> m_data;
    QStringList m_playersInLobby;
    DoorstepState* m_doorstepState;
    LobbyState* m_lobbyState;
};

Q_DECLARE_METATYPE(Player);
