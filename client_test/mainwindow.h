#pragma once
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class QGridLayout;
class QLabel;
class Socket;
class QTextEdit;
class Event;
class Player;
class RegistrationFailedEvent;
class RegisteredEvent;
class LoggedInEvent;
class LogInFailedEvent;
class LobbyUpdatedEvent;
class UpdateLobbyEvent;
class QListWidget;
class GameStartFailedEvent;
class RequestGameEvent;
class GameStartedEvent;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void onConnectClicked(bool);
    void onDisconnectClicked(bool);
    void onConnected();
    void onDisconnected();
    void onSendRequestClicked(bool);
    void onLoginClicked(bool);
    void onRegisterClicked(bool);
    void onEventRecieved(Event *event);
    void onRegistrationFailed(RegistrationFailedEvent *event);
    void onRegistered(RegisteredEvent *event);
    void onLoggedIn(LoggedInEvent *event);
    void onLogInFailed(LogInFailedEvent *event);
    void onLobbyUpdated(LobbyUpdatedEvent *event);
    void onUpdateLobbyClicked(bool);
    void onStartGameClicked(bool);
    void onGameStartFailed(GameStartFailedEvent *event);
    void onGameRequested(RequestGameEvent *event);
    void onGameStarted(GameStartedEvent *event);

    Ui::MainWindow *ui;
    QGridLayout *m_layout;
    QLabel *m_statusLabel;
    Socket *m_socket;
    QTextEdit *m_requestText;
    QTextEdit *m_usernameText;
    QTextEdit *m_passwordText;
    Player *m_player;
    QListWidget *m_playersInLobbyList;
    QLabel *m_opponentLabel;
};
