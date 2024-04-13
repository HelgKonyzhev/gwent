#pragma once
#include <QObject>
#include "player_store.h"

class QWebSocketServer;
class Player;
class LogInEvent;
class RegisterEvent;
class UpdateLobbyEvent;
class LoggedInEvent;
class StartGameEvent;
class GameAcceptedEvent;
class AddDeckEvent;
class UpdateDeckEvent;
class EraseDeckEvent;

class Server : public QObject
{
    Q_OBJECT

public:
    explicit Server(QObject* parent = nullptr);
    ~Server() override;

    bool serve();

private:
    void onNewConnection();
    void onLoggingIn(Player* player, LogInEvent* event);
    void onRegistration(Player* player, RegisterEvent* event);
    void onLobbyUpdating(Player* player, UpdateLobbyEvent* event);
    void onLoggedIn(Player* player, LoggedInEvent* event);
    void onDisconnected(Player* player);
    void onStartGame(Player* player, StartGameEvent* event);
    void onGameAccepted(Player* player, GameAcceptedEvent* event);
    void onAddDeck(Player* player, AddDeckEvent* event);
    void onUpdateDeck(Player* player, UpdateDeckEvent* event);
    void onEraseDeck(Player* player, EraseDeckEvent* event);

    QWebSocketServer* m_wsServer;
    QSet<Player*> m_players;
    PlayerStore m_playersStore;
    QMap<QString, Player*> m_loggedPlayers;
    QMap<QString, Player*> m_waitingPlayers;
};
