#include "player.h"
#include <common/events/login_event.h>
#include <common/events/register_event.h>
#include <common/events/error_event.h>
#include <common/event_transition.h>
#include <common/state.h>
#include <common/socket.h>
#include <common/events/register_event.h>
#include <common/events/login_event.h>
#include <QStateMachine>
#include <common/events/lobby_updated_event.h>
#include <common/events/start_game_event.h>
#include <common/events/request_game_event.h>
#include <common/doorstep_state.h>
#include <common/lobby_state.h>
#include <QSignalTransition>
#include <common/deck.h>
#include <common/events/add_deck_event.h>
#include <common/events/update_deck_event.h>
#include <common/events/erase_deck_event.h>

Player::Player(Socket *socket, QObject *parent)
    : QObject{parent}
    , m_fsm{new QStateMachine{this}}
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

    connect(m_lobbyState, &LobbyState::loggedIn, this,
            [this](LoggedInEvent *event) { m_data = event->playerData(); });

    connect(m_lobbyState, &LobbyState::lobbyUpdated, this,
            [this](LobbyUpdatedEvent *event) { m_playersInLobby = event->players(); });

    connect(m_socket, &Socket::eventRecieved, this,
            [this](Event *e)
            {
                e->setSource(m_socket);
                m_fsm->postEvent(e);
            });

    connect(m_lobbyState, &LobbyState::loggedIn, this, &Player::loggedIn);
    connect(m_doorstepState, &DoorstepState::loginFailed, this, &Player::loginFailed);

    connect(m_doorstepState, &DoorstepState::registered, this, &Player::registered);
    connect(m_doorstepState, &DoorstepState::registrationFailed, this, &Player::registrationFailed);

    connect(m_lobbyState, &LobbyState::deckAdded, this, &Player::onDeckAdded);
    connect(m_lobbyState, &LobbyState::deckUpdated, this, &Player::onDeckUpdated);
    connect(m_lobbyState, &LobbyState::deckErased, this, &Player::onDeckErased);

    if (m_socket->state() == QAbstractSocket::ConnectedState)
        m_fsm->start();
}

void Player::postEvent(Event *event)
{
    event->setSource(this);
    m_fsm->postEvent(event);
}

void Player::onDeckAdded(DeckAddedEvent *event)
{
    Deck deck{event->name(), event->fraction(), event->cards(), nullptr};
    if (!m_data->addDeck(deck))
        qWarning() << "failed to add deck" << event->name() << event->fraction();
}

void Player::onDeckUpdated(DeckUpdatedEvent *event)
{
    Deck deck{event->name(), event->fraction(), event->cards(), nullptr};
    if (!m_data->updateDeck(deck))
        qWarning() << "failed to update deck" << event->name() << event->fraction();
}

void Player::onDeckErased(DeckErasedEvent *event)
{
    if (!m_data->eraseDeck(event->name()))
        qWarning() << "failed to erase deck" << event->name();
}
