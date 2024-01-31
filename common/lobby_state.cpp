#include "lobby_state.h"
#include <common/state.h>
#include <common/event_transition.h>
#include <common/events/request_game_event.h>
#include <common/events/start_game_event.h>
#include <common/events/lobby_updated_event.h>
#include <common/final_state.h>
#include <common/events/login_event.h>

LobbyState::LobbyState(Socket *socket, QState *parent)
    : State{parent}
    , m_socket{socket}
{
    setObjectName("lobbyState");

    auto initialState = new State{this};
    initialState->setObjectName("initialState");

    auto finalState = new FinalState{this};
    finalState->setObjectName("finalState");

    setInitialState(initialState);

    connect(initialState, &State::entered, this,
            [this](Event *event)
            {
                switch (static_cast<Event::Type>(event->type()))
                {
                    case Event::LoggedIn:
                    {
                        auto e = event->to<LoggedInEvent>();
                        emit loggedIn(e);
                        break;
                    }
                    case Event::GameStartFailed:
                        emit gameStartFailed(event->to<GameStartFailedEvent>());
                        break;
                    default:
                        qDebug() << "unexpected event" << event->type();
                        break;
                };
            });

    auto startingGameState = new State{this};
    startingGameState->setObjectName("startingGameState");

    connect(startingGameState, &State::entered, this,
            [this](Event *event)
            {
                switch (static_cast<Event::Type>(event->type()))
                {
                    case Event::StartGame:
                        emit startingGame(event->to<StartGameEvent>());
                        break;
                    case Event::RequestGame:
                        emit gameRequested(event->to<RequestGameEvent>());
                        break;
                    default:
                        qDebug() << "unexpected event" << event->type();
                        break;
                }
            });

    auto gameState = new State(this);
    gameState->setObjectName("gameState");

    connect(gameState, &State::entered, this,
            [this](Event *event) { emit gameStarted(event->to<GameStartedEvent>()); });

    auto updateLobbyTrans = new EventTransition{m_socket, Event::UpdateLobby};

    connect(updateLobbyTrans, &EventTransition::triggered, this,
            [this](Event *event) { emit updatingLobby(event->to<UpdateLobbyEvent>()); });

    auto lobbyUpdatedTrans = new EventTransition{m_socket, Event::LobbyUpdated};

    connect(lobbyUpdatedTrans, &EventTransition::triggered, this,
            [this](Event *event)
            {
                auto e = event->to<LobbyUpdatedEvent>();
                emit lobbyUpdated(e);
            });

    initialState->addTransition(updateLobbyTrans);
    initialState->addTransition(lobbyUpdatedTrans);
    initialState->addTransition(new EventTransition{m_socket, Event::StartGame, startingGameState});
    initialState->addTransition(new EventTransition{m_socket, Event::RequestGame, startingGameState});

    auto gameAcceptedTrans = new EventTransition{m_socket, Event::GameAccepted};

    connect(gameAcceptedTrans, &EventTransition::triggered, this,
            [this](Event *event) { emit gameAccepted(event->to<GameAcceptedEvent>()); });

    startingGameState->addTransition(new EventTransition{m_socket, Event::GameStarted, gameState});
    startingGameState->addTransition(new EventTransition{m_socket, Event::GameStartFailed, initialState});
    startingGameState->addTransition(gameAcceptedTrans);
}
