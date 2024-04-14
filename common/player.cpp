#include "player.h"
#include <common/events/login_event.h>
#include <common/events/register_event.h>
#include <common/events/error_event.h>
#include <common/event_transition.h>
#include <common/states/state.h>
#include <common/socket.h>
#include <common/events/register_event.h>
#include <common/events/login_event.h>
#include <common/events/lobby_updated_event.h>
#include <common/events/start_game_event.h>
#include <common/events/request_game_event.h>
#include <common/states/doorstep_state.h>
#include <common/states/lobby_state.h>
#include <common/deck.h>
#include <common/events/add_deck_event.h>
#include <common/events/update_deck_event.h>
#include <common/events/erase_deck_event.h>
#include <common/events/deck_edited_event.h>
#include <common/events/deck_edition_failed.h>
#include <common/state_machine.h>

Player::Player(Socket *socket, QObject *parent)
    : QObject{parent}
    , m_fsm{new StateMachine{this}}
    , m_socket{socket}
    , m_doorstepState{new DoorstepState{m_socket, m_fsm}}
    , m_lobbyState{new LobbyState{m_socket, m_fsm}}
{
    qRegisterMetaType<Player>();

    connect(m_socket, &Socket::connected, m_fsm, &QStateMachine::start);
    connect(m_socket, &Socket::disconnected, m_fsm, &QStateMachine::stop);

    m_fsm->setInitialState(m_doorstepState);

    m_doorstepState->addTransition(new EventTransition{m_socket, Event::LoggedIn, m_lobbyState});

    connect(m_lobbyState, &State::entered, this, [this](QEvent *) { m_inLobby = true; });
    connect(m_lobbyState, &State::exited, this, [this](QEvent *) { m_inLobby = false; });

    connect(m_socket, &Socket::eventRecieved, this,
            [this](Event *e)
            {
                e->accept();
                m_fsm->postEvent(e);
            });

    m_fsm->onTransition<RegisterEvent>(this, &Player::registration);
    m_fsm->onTransition<RegisteredEvent>(this, &Player::registered);
    m_fsm->onTransition<RegistrationFailedEvent>(this, &Player::registrationFailed);
    m_fsm->onTransition<LogInEvent>(this, &Player::logging);
    m_fsm->onTransition<LogInFailedEvent>(this, &Player::loginFailed);
    m_fsm->onTransition<LoggedInEvent>(this,
                                       [&](LoggedInEvent *event)
                                       {
                                           m_data = event->playerData();
                                           emit loggedIn(event);
                                       });

    m_fsm->onTransition<UpdateLobbyEvent>(this, &Player::updatingLobby);
    m_fsm->onTransition<LobbyUpdatedEvent>(this,
                                           [&](LobbyUpdatedEvent *event)
                                           {
                                               m_playersInLobby = event->players();
                                               emit lobbyUpdated(event);
                                           });

    m_fsm->onTransition<StartGameEvent>(this, &Player::startingGame);
    m_fsm->onTransition<GameStartFailedEvent>(this, &Player::gameStartFailed);
    m_fsm->onTransition<GameStartedEvent>(this, &Player::gameStarted);
    m_fsm->onTransition<RequestGameEvent>(this, &Player::gameRequested);
    m_fsm->onTransition<GameAcceptedEvent>(this, &Player::gameAccepted);
    m_fsm->onTransition<AddDeckEvent>(this, &Player::addingDeck);
    m_fsm->onTransition<UpdateDeckEvent>(this, &Player::updatingDeck);
    m_fsm->onTransition<EraseDeckEvent>(this, &Player::erasingDeck);
    m_fsm->onTransition<DeckEditedEvent>(this,
                                         [&](DeckEditedEvent *event)
                                         {
                                             m_data = event->playerData();
                                             emit deckEdited(event);
                                         });

    m_fsm->onTransition<DeckEditionFailedEvent>(this, &Player::deckEditionFailed);

    if (m_socket->state() == QAbstractSocket::ConnectedState)
        m_fsm->start();
}

bool Player::inLobby() const { return m_fsm->configuration().contains(m_lobbyState); }

void Player::postEvent(Event *event)
{
    event->ignore();
    m_fsm->postEvent(event);
}
