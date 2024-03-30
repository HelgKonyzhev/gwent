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

Player::Player(Socket *socket, QObject *parent)
    : QObject{parent}
    , m_fsm{new QStateMachine{this}}
    , m_socket{socket}
    , m_thresholdState{new DoorstepState{m_socket, m_fsm}}
    , m_lobbyState{new LobbyState{m_socket, m_fsm}}
{

    m_fsm->setInitialState(m_thresholdState);

    m_thresholdState->addTransition(new EventTransition{m_socket, Event::LoggedIn, m_lobbyState});

    connect(m_lobbyState, &State::entered, this, [this](Event *) { m_inLobby = true; });
    connect(m_lobbyState, &State::exited, this, [this](Event *) { m_inLobby = false; });

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

    connect(m_socket, &Socket::connected, m_fsm, &QStateMachine::start);
    connect(m_socket, &Socket::disconnected, m_fsm, &QStateMachine::stop);

    if (m_socket->state() == QAbstractSocket::ConnectedState)
        m_fsm->start();

    //    auto initState = new State{m_fsm};
    //    initState->setObjectName("initState");

    //    connect(initState, &State::entered, this,
    //            [this](Event *event)
    //            {
    //                switch (static_cast<Event::Type>(event->type()))
    //                {
    //                    case Event::Registered:
    //                        emit registered(event->to<RegisteredEvent>());
    //                        break;
    //                    case Event::RegistrationFailed:
    //                        emit registrationFailed(event->to<RegistrationFailedEvent>());
    //                        break;
    //                    case Event::LogInFailed:
    //                        emit loginFailed(event->to<LogInFailedEvent>());
    //                        break;
    //                    default:
    //                        qDebug() << "unexpected event" << event->type();
    //                        break;
    //                }
    //            });

    //    auto registrationState = new State{m_fsm};
    //    registrationState->setObjectName("registrationState");

    //    connect(registrationState, &State::entered, this,
    //            [this](Event *event) { emit registration(event->to<RegisterEvent>()); });

    //    auto loggingInState = new State{m_fsm};
    //    loggingInState->setObjectName("loggingInState");

    //    connect(loggingInState, &State::entered, this,
    //            [this](Event *event) { emit logging(event->to<LogInEvent>()); });

    //    auto lobbyState = new State{m_fsm};
    //    lobbyState->setObjectName("lobbyState");

    //    connect(lobbyState, &State::entered, this,
    //            [this](Event *event)
    //            {
    //                switch (static_cast<Event::Type>(event->type()))
    //                {
    //                    case Event::LoggedIn:
    //                    {
    //                        auto e = event->to<LoggedInEvent>();
    //                        m_data = e->playerData();
    //                        emit loggedIn(e);
    //                        m_inLobby = true;
    //                        break;
    //                    }
    //                    case Event::GameStartFailed:
    //                        emit gameStartFailed(event->to<GameStartFailedEvent>());
    //                        break;
    //                    default:
    //                        qDebug() << "unexpected event" << event->type();
    //                        break;
    //                };
    //            });

    //    connect(lobbyState, &State::exited, this, [this](Event *) { m_inLobby = false; });

    //    auto startingGameState = new State{m_fsm};
    //    startingGameState->setObjectName("startingGameState");

    //    connect(startingGameState, &State::entered, this,
    //            [this](Event *event)
    //            {
    //                switch (static_cast<Event::Type>(event->type()))
    //                {
    //                    case Event::StartGame:
    //                        emit startingGame(event->to<StartGameEvent>());
    //                        break;
    //                    case Event::RequestGame:
    //                        emit gameRequested(event->to<RequestGameEvent>());
    //                        break;
    //                    default:
    //                        qDebug() << "unexpected event" << event->type();
    //                        break;
    //                }
    //            });

    //    auto gameState = new State(m_fsm);
    //    gameState->setObjectName("gameState");

    //    connect(gameState, &State::entered, this,
    //            [this](Event *event) { emit gameStarted(event->to<GameStartedEvent>()); });

    //    initState->addTransition(new EventTransition{m_socket, Event::Register, registrationState});
    //    initState->addTransition(new EventTransition{m_socket, Event::LogIn, loggingInState});

    //    registrationState->addTransition(new EventTransition{m_socket, Event::RegistrationFailed,
    //    initState}); registrationState->addTransition(new EventTransition{m_socket, Event::Registered,
    //    initState});

    //    loggingInState->addTransition(new EventTransition{m_socket, Event::LoggedIn, lobbyState});
    //    loggingInState->addTransition(new EventTransition{m_socket, Event::LogInFailed, initState});

    //    auto updateLobbyTrans = new EventTransition{m_socket, Event::UpdateLobby};

    //    connect(updateLobbyTrans, &EventTransition::triggered, this,
    //            [this](Event *event) { emit updatingLobby(event->to<UpdateLobbyEvent>()); });

    //    auto lobbyUpdatedTrans = new EventTransition{m_socket, Event::LobbyUpdated};

    //    connect(lobbyUpdatedTrans, &EventTransition::triggered, this,
    //            [this](Event *event)
    //            {
    //                auto e = event->to<LobbyUpdatedEvent>();
    //                m_playersInLobby = e->players();
    //                emit lobbyUpdated(e);
    //            });

    //    lobbyState->addTransition(updateLobbyTrans);
    //    lobbyState->addTransition(lobbyUpdatedTrans);
    //    lobbyState->addTransition(new EventTransition{m_socket, Event::StartGame, startingGameState});
    //    lobbyState->addTransition(new EventTransition{m_socket, Event::RequestGame, startingGameState});

    //    auto gameAcceptedTrans = new EventTransition{m_socket, Event::GameAccepted};

    //    connect(gameAcceptedTrans, &EventTransition::triggered, this,
    //            [this](Event *event) { emit gameAccepted(event->to<GameAcceptedEvent>()); });

    //    startingGameState->addTransition(new EventTransition{m_socket, Event::GameStarted, gameState});
    //    startingGameState->addTransition(new EventTransition{m_socket, Event::GameStartFailed, lobbyState});
    //    startingGameState->addTransition(gameAcceptedTrans);

    //    m_fsm->setInitialState(initState);

    //    connect(m_socket, &Socket::eventRecieved, this,
    //            [this](Event *e)
    //            {
    //                e->setSource(m_socket);
    //                m_fsm->postEvent(e);
    //            });

    //    connect(m_socket, &Socket::connected, m_fsm, &QStateMachine::start);
    //    connect(m_socket, &Socket::disconnected, m_fsm, &QStateMachine::stop);

    //    if (m_socket->state() == QAbstractSocket::ConnectedState)
    //        m_fsm->start();
}

void Player::postEvent(Event *event)
{
    event->setSource(this);
    m_fsm->postEvent(event);
}
