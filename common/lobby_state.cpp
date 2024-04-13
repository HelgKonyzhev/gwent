#include "lobby_state.h"
#include <common/state.h>
#include <common/event_transition.h>
#include <common/events/request_game_event.h>
#include <common/events/start_game_event.h>
#include <common/events/lobby_updated_event.h>
#include <common/final_state.h>
#include <common/events/login_event.h>
#include <common/events/add_deck_event.h>
#include <common/events/update_deck_event.h>
#include <common/events/erase_deck_event.h>

LobbyState::LobbyState(Socket *socket, QState *parent)
    : State{parent}
    , m_socket{socket}
{
    qRegisterMetaType<LobbyState>();

    setObjectName("lobbyState");

    auto initialState = new State{this};
    initialState->setObjectName("initialState");

    auto finalState = new FinalState{this};
    finalState->setObjectName("finalState");

    setInitialState(initialState);

    connect(initialState, &State::entered, this,
            [this](QEvent *qevent)
            {
                auto event = castToEvent(qevent);
                if (!event)
                    return;

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
                    case Event::DeckAdded:
                        emit deckAdded(event->to<DeckAddedEvent>());
                        break;
                    case Event::DeckAddFailed:
                        emit deckAddFailed(event->to<DeckAddFailedEvent>());
                        break;
                    case Event::DeckUpdated:
                        emit deckUpdated(event->to<DeckUpdatedEvent>());
                        break;
                    case Event::DeckUpdateFailed:
                        emit deckUpdateFailed(event->to<DeckUpdateFailedEvent>());
                        break;
                    case Event::DeckErased:
                        emit deckErased(event->to<DeckErasedEvent>());
                        break;
                    case Event::DeckEraseFailed:
                        emit deckEraseFailed(event->to<DeckEraseFailedEvent>());
                        break;
                    default:
                        qDebug() << "unexpected event" << event->type();
                        break;
                };
            });

    auto startingGameState = new State{this};
    startingGameState->setObjectName("startingGameState");

    connect(startingGameState, &State::entered, this,
            [this](QEvent *qevent)
            {
                auto event = castToEvent(qevent);
                if (!event)
                    return;

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
            [this](QEvent *qevent) { emit gameStarted(castToEvent(qevent)->to<GameStartedEvent>()); });

    auto updateLobbyTrans = new EventTransition{m_socket, Event::UpdateLobby};

    connect(updateLobbyTrans, &EventTransition::triggered, this,
            [this](QEvent *qevent) { emit updatingLobby(castToEvent(qevent)->to<UpdateLobbyEvent>()); });

    auto lobbyUpdatedTrans = new EventTransition{m_socket, Event::LobbyUpdated};

    connect(lobbyUpdatedTrans, &EventTransition::triggered, this,
            [this](QEvent *qevent)
            {
                auto e = castToEvent(qevent)->to<LobbyUpdatedEvent>();
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

    auto addingDeckState = new State{this};
    addingDeckState->setObjectName("addingDeckState");

    initialState->addTransition(new EventTransition{m_socket, Event::AddDeck, addingDeckState});
    addingDeckState->addTransition(new EventTransition{m_socket, Event::DeckAdded, initialState});
    addingDeckState->addTransition(new EventTransition{m_socket, Event::DeckAddFailed, initialState});

    connect(addingDeckState, &State::entered, this,
            [&](QEvent *qevent) { emit addingDeck(castToEvent(qevent)->to<AddDeckEvent>()); });

    auto updatingDeckState = new State{this};
    updatingDeckState->setObjectName("updatingDeckState");

    updatingDeckState->addTransition(new EventTransition{m_socket, Event::UpdateDeck, updatingDeckState});
    addingDeckState->addTransition(new EventTransition{m_socket, Event::DeckUpdated, initialState});
    addingDeckState->addTransition(new EventTransition{m_socket, Event::DeckUpdateFailed, initialState});

    connect(updatingDeckState, &State::entered, this,
            [&](QEvent *qevent) { emit updatingDeck(castToEvent(qevent)->to<UpdateDeckEvent>()); });

    auto erasingDeckState = new State{this};
    erasingDeckState->setObjectName("erasingDeckState");

    erasingDeckState->addTransition(new EventTransition{m_socket, Event::EraseDeck, erasingDeckState});
    addingDeckState->addTransition(new EventTransition{m_socket, Event::DeckErased, initialState});
    addingDeckState->addTransition(new EventTransition{m_socket, Event::DeckEraseFailed, initialState});

    connect(erasingDeckState, &State::entered, this,
            [&](QEvent *qevent) { emit erasingDeck(castToEvent(qevent)->to<EraseDeckEvent>()); });
}
