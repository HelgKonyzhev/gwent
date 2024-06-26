#include "server.h"
#include <QWebSocketServer>
#include <QDebug>
#include <QWebSocket>
#include <common/events/login_event.h>
#include <common/events/register_event.h>
#include <common/socket.h>
#include <common/player.h>
#include <common/events/lobby_updated_event.h>
#include <common/events/start_game_event.h>
#include <common/events/request_game_event.h>
#include <common/states/doorstep_state.h>
#include <common/states/lobby_state.h>
#include <common/events/add_deck_event.h>
#include <common/events/update_deck_event.h>
#include <common/events/erase_deck_event.h>
#include <common/deck.h>
#include <common/events/deck_edited_event.h>
#include <common/events/deck_edition_failed.h>

Server::Server(QObject* parent)
    : QObject{parent}
{
    m_wsServer = new QWebSocketServer{"", QWebSocketServer::NonSecureMode, this};

    connect(m_wsServer, &QWebSocketServer::newConnection, this, &Server::onNewConnection);
}

Server::~Server() {}

bool Server::serve()
{
    if (!m_playersStore.load())
        return false;

    return m_wsServer->listen(QHostAddress::Any, 8888);
}

void Server::onNewConnection()
{
    auto player = new Player{new Socket{m_wsServer->nextPendingConnection()}, this};

    connect(player->socket(), &Socket::disconnected, player, [player, this]() { onDisconnected(player); });

    connect(player, &Player::logging, this,
            [this, player](LogInEvent* event) { onLoggingIn(player, event); });
    connect(player, &Player::registration, this,
            [this, player](RegisterEvent* event) { onRegistration(player, event); });
    connect(player, &Player::loggedIn, this,
            [this, player](LoggedInEvent* event) { onLoggedIn(player, event); });
    connect(player, &Player::updatingLobby, this,
            [this, player](UpdateLobbyEvent* event) { onLobbyUpdating(player, event); });
    connect(player, &Player::startingGame, this,
            [this, player](StartGameEvent* event) { onStartGame(player, event); });
    connect(player, &Player::gameAccepted, this,
            [this, player](GameAcceptedEvent* event) { onGameAccepted(player, event); });
    connect(player, &Player::addingDeck, this,
            [this, player](AddDeckEvent* event) { onAddDeck(player, event); });
    connect(player, &Player::updatingDeck, this,
            [this, player](UpdateDeckEvent* event) { onUpdateDeck(player, event); });
    connect(player, &Player::erasingDeck, this,
            [this, player](EraseDeckEvent* event) { onEraseDeck(player, event); });

    qDebug() << __func__ << player->socket()->requestUrl();

    m_players.insert(player);
}

void Server::onLoggingIn(Player* player, LogInEvent* event)
{
    if (m_loggedPlayers.contains(event->username()))
    {
        player->postEvent(
            new LogInFailedEvent{event->username(), QString{"%1 already logged in"}.arg(event->username())});
        return;
    }

    const auto playerData = m_playersStore.get(event->username(), event->password());
    if (!playerData)
        player->postEvent(new LogInFailedEvent{event->username(), playerData.error()});
    else
        player->postEvent(new LoggedInEvent{playerData.value<QSharedPointer<PlayerData>>()});
}

void Server::onRegistration(Player* player, RegisterEvent* event)
{
    if (const auto res = m_playersStore.add(event->username(), event->password()); !res)
        player->postEvent(new RegistrationFailedEvent{event->username(), res.error()});
    else
        player->postEvent(new RegisteredEvent{event->username()});
}

void Server::onLobbyUpdating(Player* player, UpdateLobbyEvent* event)
{
    Q_UNUSED(event);

    QStringList playersInLobby;
    for (const auto& p : std::as_const(m_players))
    {
        if (p->inLobby() && p->data()->name() != player->data()->name())
            playersInLobby.append(p->data()->name());
    }

    player->postEvent(new LobbyUpdatedEvent{playersInLobby});
}

void Server::onLoggedIn(Player* player, LoggedInEvent* event)
{
    Q_UNUSED(player);
    Q_UNUSED(event);

    m_loggedPlayers.insert(player->data()->name(), player);

    for (auto p : std::as_const(m_players))
        p->postEvent(new UpdateLobbyEvent{});
}

void Server::onDisconnected(Player* player)
{
    m_players.remove(player);

    if (player->data())
    {
        m_loggedPlayers.remove(player->data()->name());
        m_waitingPlayers.remove(player->data()->name());
    }

    for (auto p : std::as_const(m_players))
        p->postEvent(new UpdateLobbyEvent{});

    player->deleteLater();
}

void Server::onStartGame(Player* player, StartGameEvent* event)
{
    auto opponentIt = m_loggedPlayers.find(event->opponent());
    if (opponentIt == m_loggedPlayers.end())
    {
        player->postEvent(
            new GameStartFailedEvent{QString{"player %1 is not ready to play"}.arg(event->opponent())});
        return;
    }

    m_waitingPlayers.insert(player->data()->name(), player);
    opponentIt.value()->postEvent(new RequestGameEvent{player->data()->name()});
}

void Server::onGameAccepted(Player* player, GameAcceptedEvent* event)
{
    auto opponent = m_waitingPlayers.take(event->opponent());
    if (!opponent)
    {
        player->postEvent(
            new GameStartFailedEvent{QString{"player %1 is not waiting for game"}.arg(event->opponent())});
        return;
    }

    player->postEvent(new GameStartedEvent{opponent->data()->name()});
    opponent->postEvent(new GameStartedEvent{player->data()->name()});
}

void Server::onAddDeck(Player* player, AddDeckEvent* event)
{
    Deck deck{event->name(), event->fraction(), event->cards(), nullptr};
    const auto added = m_playersStore.addDeck(player->data()->name(), deck);
    if (added)
        player->postEvent(new DeckEditedEvent{deck.name(), player->data()});
    else
        player->postEvent(new DeckEditionFailedEvent{deck.name(), added.error()});
}

void Server::onUpdateDeck(Player* player, UpdateDeckEvent* event)
{
    Deck deck{event->name(), event->fraction(), event->cards(), nullptr};
    const auto updated = m_playersStore.updateDeck(player->data()->name(), deck);
    if (updated)
        player->postEvent(new DeckEditedEvent{deck.name(), player->data()});
    else
        player->postEvent(new DeckEditionFailedEvent{deck.name(), updated.error()});
}

void Server::onEraseDeck(Player* player, EraseDeckEvent* event)
{
    const auto erased = m_playersStore.eraseDeck(player->data()->name(), event->name());
    if (erased)
        player->postEvent(new DeckEditedEvent{event->name(), player->data()});
    else
        player->postEvent(new DeckEditionFailedEvent{event->name(), erased.error()});
}
