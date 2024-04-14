#include "lobby_state.h"
#include <common/states/state.h>
#include <common/event_transition.h>
#include <common/events/request_game_event.h>
#include <common/events/start_game_event.h>
#include <common/events/lobby_updated_event.h>
#include <common/events/login_event.h>
#include <common/events/add_deck_event.h>
#include <common/events/update_deck_event.h>
#include <common/events/erase_deck_event.h>
#include <common/events/deck_edited_event.h>
#include <common/events/deck_edition_failed.h>

LobbyState::LobbyState(Socket *socket, QState *parent)
    : State{parent}
    , m_socket{socket}
{
    qRegisterMetaType<LobbyState>();

    setObjectName("lobbyState");

    auto initialState = new State{this};
    initialState->setObjectName("initialState");

    setInitialState(initialState);

    auto startingGameState = new State{this};
    startingGameState->setObjectName("startingGameState");

    auto gameState = new State(this);
    gameState->setObjectName("gameState");

    auto updateLobbyTrans = new EventTransition{m_socket, Event::UpdateLobby};

    auto lobbyUpdatedTrans = new EventTransition{m_socket, Event::LobbyUpdated};

    initialState->addTransition(updateLobbyTrans);
    initialState->addTransition(lobbyUpdatedTrans);
    initialState->addTransition(new EventTransition{m_socket, Event::StartGame, startingGameState});
    initialState->addTransition(new EventTransition{m_socket, Event::RequestGame, startingGameState});

    auto gameAcceptedTrans = new EventTransition{m_socket, Event::GameAccepted};

    startingGameState->addTransition(new EventTransition{m_socket, Event::GameStarted, gameState});
    startingGameState->addTransition(new EventTransition{m_socket, Event::GameStartFailed, initialState});
    startingGameState->addTransition(gameAcceptedTrans);

    auto addingDeckState = new State{this};
    addingDeckState->setObjectName("addingDeckState");

    initialState->addTransition(new EventTransition{m_socket, Event::AddDeck, addingDeckState});
    addingDeckState->addTransition(new EventTransition{m_socket, Event::DeckEdited, initialState});
    addingDeckState->addTransition(new EventTransition{m_socket, Event::DeckEditionFailed, initialState});

    auto updatingDeckState = new State{this};
    updatingDeckState->setObjectName("updatingDeckState");

    updatingDeckState->addTransition(new EventTransition{m_socket, Event::UpdateDeck, updatingDeckState});
    addingDeckState->addTransition(new EventTransition{m_socket, Event::DeckEdited, initialState});
    addingDeckState->addTransition(new EventTransition{m_socket, Event::DeckEditionFailed, initialState});

    auto erasingDeckState = new State{this};
    erasingDeckState->setObjectName("erasingDeckState");

    erasingDeckState->addTransition(new EventTransition{m_socket, Event::EraseDeck, erasingDeckState});
    addingDeckState->addTransition(new EventTransition{m_socket, Event::DeckEdited, initialState});
    addingDeckState->addTransition(new EventTransition{m_socket, Event::DeckEditionFailed, initialState});
}
