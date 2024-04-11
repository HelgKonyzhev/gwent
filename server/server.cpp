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
#include <common/doorstep_state.h>
#include <common/lobby_state.h>

Server::Server(QObject* parent)
    : QObject{parent}
{
    m_wsServer = new QWebSocketServer{"", QWebSocketServer::NonSecureMode, this};

    connect(m_wsServer, &QWebSocketServer::newConnection, this, &Server::onNewConnection);
}

Server::~Server() {}

bool Server::serve()
{
    if (!m_users.load())
        return false;

    return m_wsServer->listen(QHostAddress::Any, 8888);
}

void Server::onNewConnection()
{
    auto player = new Player{new Socket{m_wsServer->nextPendingConnection()}, this};

    connect(player->socket(), &Socket::disconnected, player, [player, this]() { onDisconnected(player); });

    connect(player->doorstepState(), &DoorstepState::logging, this,
            [this, player](LogInEvent* event) { onLoggingIn(player, event); });
    connect(player->doorstepState(), &DoorstepState::registration, this,
            [this, player](RegisterEvent* event) { onRegistration(player, event); });
    connect(player->lobbyState(), &LobbyState::loggedIn, this,
            [this, player](LoggedInEvent* event) { onLoggedIn(player, event); });
    connect(player->lobbyState(), &LobbyState::updatingLobby, this,
            [this, player](UpdateLobbyEvent* event) { onLobbyUpdating(player, event); });
    connect(player->lobbyState(), &LobbyState::startingGame, this,
            [this, player](StartGameEvent* event) { onStartGame(player, event); });
    connect(player->lobbyState(), &LobbyState::gameAccepted, this,
            [this, player](GameAcceptedEvent* event) { onGameAccepted(player, event); });

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

    const auto playerData = m_users.get(event->username(), event->password());
    if (!playerData)
        player->postEvent(new LogInFailedEvent{event->username(), playerData.error()});
    else
        player->postEvent(new LoggedInEvent{playerData.value()});
}

void Server::onRegistration(Player* player, RegisterEvent* event)
{
    if (const auto res = m_users.add(event->username(), event->password()); !res)
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
