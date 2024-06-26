#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QtWebSockets/QWebSocket>
#include <QUrl>
#include <QTextEdit>
#include <common/events/login_event.h>
#include <common/events/register_event.h>
#include <common/socket.h>
#include <common/player.h>
#include <common/events/lobby_updated_event.h>
#include <QListWidget>
#include <common/events/start_game_event.h>
#include <QMessageBox>
#include <common/events/request_game_event.h>
#include <common/states/doorstep_state.h>
#include <common/states/lobby_state.h>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <common/cards_data/cards_data.h>
#include <common/cards_data/card_data.h>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_socket{new Socket{new QWebSocket{"", QWebSocketProtocol::VersionLatest, this}}}
    , m_player{new Player{m_socket, this}}
{
    setCentralWidget(new QWidget{this});
    m_layout = new QGridLayout{};
    m_statusLabel = new QLabel{this};
    centralWidget()->setLayout(m_layout);

    auto resultLayeout = new QHBoxLayout;
    resultLayeout->addWidget(new QLabel{"Result:", this});
    resultLayeout->addWidget(m_statusLabel);
    m_layout->addLayout(resultLayeout, 0, 0);

    auto connectButton = new QPushButton{"connect", this};
    connect(connectButton, &QPushButton::clicked, this, &MainWindow::onConnectClicked);
    m_layout->addWidget(connectButton, 1, 0);

    auto disconnectButton = new QPushButton{"disconnect", this};
    connect(disconnectButton, &QPushButton::clicked, this, &MainWindow::onDisconnectClicked);
    m_layout->addWidget(disconnectButton, 1, 1);

    connect(m_socket, &Socket::connected, this, &MainWindow::onConnected);
    connect(m_socket, &Socket::disconnected, this, &MainWindow::onDisconnected);

    m_requestText = new QTextEdit{this};
    m_layout->addWidget(m_requestText, 0, 2);

    auto sendRequestButton = new QPushButton{"send request", this};
    connect(sendRequestButton, &QPushButton::clicked, this, &MainWindow::onSendRequestClicked);
    m_layout->addWidget(sendRequestButton, 1, 2);

    auto userCredsLayout = new QVBoxLayout;
    m_usernameText = new QTextEdit{"username", this};
    userCredsLayout->addWidget(m_usernameText);

    m_passwordText = new QTextEdit{"password", this};
    userCredsLayout->addWidget(m_passwordText);

    m_layout->addItem(userCredsLayout, 0, 3);

    auto loginButton = new QPushButton{"login", this};
    connect(loginButton, &QPushButton::clicked, this, &MainWindow::onLoginClicked);
    m_layout->addWidget(loginButton, 1, 3);

    auto registerButton = new QPushButton{"register", this};
    connect(registerButton, &QPushButton::clicked, this, &MainWindow::onRegisterClicked);
    m_layout->addWidget(registerButton, 1, 4);

    m_playersInLobbyList = new QListWidget{this};
    m_layout->addWidget(m_playersInLobbyList, 0, 5);

    connect(m_playersInLobbyList, &QListWidget::currentItemChanged, this,
            [this](QListWidgetItem* current, QListWidgetItem* previous)
            {
                Q_UNUSED(previous);
                if (current)
                    m_opponentLabel->setText(current->text());
            });

    auto updateLobbyButton = new QPushButton{"update lobby", this};
    connect(updateLobbyButton, &QPushButton::clicked, this, &MainWindow::onUpdateLobbyClicked);
    m_layout->addWidget(updateLobbyButton, 1, 5);

    auto gameLayout = new QVBoxLayout;
    gameLayout->addWidget(new QLabel{"Play with", this});

    m_opponentLabel = new QLabel{this};
    gameLayout->addWidget(m_opponentLabel);

    m_layout->addItem(gameLayout, 0, 6);

    auto startGameButton = new QPushButton{"start game", this};
    connect(startGameButton, &QPushButton::clicked, this, &MainWindow::onStartGameClicked);
    m_layout->addWidget(startGameButton, 1, 6);

    {
        //        QHBoxLayout* cardsLayout = new QHBoxLayout;
        //        auto cardsData = CardsData::instance();

        //        QHash<QString, QVariantList> filters;
        //        filters["tags"] = {"leader"};
        //        const auto filtered = cardsData->cards().get(filters);

        //        for (const auto card : filtered)
        //        {
        //            auto imgPath = QString(":/%1").arg(card->image());
        //            QPixmap pic;
        //            auto res = pic.load(imgPath);
        //            if (!res)
        //            {
        //                qDebug() << "failed" << imgPath;
        //                continue;
        //            }

        //            auto l = new QLabel{this};
        //            l->setPixmap(pic);
        //            cardsLayout->addWidget(l);
        //        }

        //        for (const auto cardData : cardsData->cards().getAll())
        //        {
        //            auto imgPath = QString(":/%1").arg(cardData->image());
        //            QPixmap pic;
        //            auto res = pic.load(imgPath);
        //            if (!res)
        //            {
        //                qDebug() << "failed" << imgPath;
        //                continue;
        //            }

        //            auto l = new QLabel{this};
        //            l->setPixmap(pic);
        //            cardsLayout->addWidget(l);
        //        }

        //        m_layout->addItem(cardsLayout, 0, 7);
    }

    connect(m_socket, &Socket::eventRecieved, this, &MainWindow::onEventRecieved);

    connect(m_player, &Player::registered, this, &MainWindow::onRegistered);
    connect(m_player, &Player::registrationFailed, this, &MainWindow::onRegistrationFailed);
    connect(m_player, &Player::loginFailed, this, &MainWindow::onLogInFailed);

    connect(m_player, &Player::loggedIn, this, &MainWindow::onLoggedIn);
    connect(m_player, &Player::lobbyUpdated, this, &MainWindow::onLobbyUpdated);
    connect(m_player, &Player::gameStartFailed, this, &MainWindow::onGameStartFailed);
    connect(m_player, &Player::gameRequested, this, &MainWindow::onGameRequested);
    connect(m_player, &Player::gameStarted, this, &MainWindow::onGameStarted);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::onConnectClicked(bool)
{
    m_statusLabel->setText("Connecting...");
    QUrl url;
    url.setScheme("ws");
    url.setHost("localhost");
    url.setPort(8888);
    qDebug() << url;
    m_socket->open(url);
}

void MainWindow::onDisconnectClicked(bool)
{
    m_statusLabel->setText("Disconnectiong...");
    m_socket->close();
}

void MainWindow::onConnected() { m_statusLabel->setText("Connected"); }

void MainWindow::onDisconnected() { m_statusLabel->setText("Disconnected"); }

void MainWindow::onSendRequestClicked(bool)
{
    //    const auto request = m_requestText->toPlainText();
    //    const auto sentCount = m_ws->sendTextMessage(request);
    //    m_statusLabel->setText(sentCount == request.size() ? "Sent" : "Not sent");
}

void MainWindow::onLoginClicked(bool)
{
    m_player->postEvent(new LogInEvent{m_usernameText->toPlainText(), m_passwordText->toPlainText()});
}

void MainWindow::onRegisterClicked(bool)
{
    m_player->postEvent(new RegisterEvent{m_usernameText->toPlainText(), m_passwordText->toPlainText()});
}

void MainWindow::onEventRecieved(Event* /*event*/) {}

void MainWindow::onRegistrationFailed(RegistrationFailedEvent* event)
{
    m_statusLabel->setText(
        QString{"User \'%1\' registration failed: %2"}.arg(event->username(), event->reason()));
}

void MainWindow::onRegistered(RegisteredEvent* event)
{
    m_statusLabel->setText(QString{"User \'%1\' registered"}.arg(event->username()));
}

void MainWindow::onLoggedIn(LoggedInEvent* event)
{
    m_statusLabel->setText(QString{"User \'%1\' logged in"}.arg(event->playerData()->name()));
}

void MainWindow::onLogInFailed(LogInFailedEvent* event)
{
    m_statusLabel->setText(QString{"User \'%1\' log in failed: %2"}.arg(event->username(), event->reason()));
}

void MainWindow::onLobbyUpdated(LobbyUpdatedEvent* event)
{
    auto oldStatus = m_statusLabel->text();
    m_statusLabel->setText(QString{"%1\nLobby updated"}.arg(oldStatus));

    m_playersInLobbyList->clear();
    for (const auto& player : event->players())
        m_playersInLobbyList->addItem(player);
}

void MainWindow::onUpdateLobbyClicked(bool) { m_player->postEvent(new UpdateLobbyEvent{}); }

void MainWindow::onStartGameClicked(bool)
{
    m_player->postEvent(new StartGameEvent{m_opponentLabel->text()});
}

void MainWindow::onGameStartFailed(GameStartFailedEvent* event)
{
    m_statusLabel->setText(QString{"Failed to start game: %1"}.arg(event->reason()));
}

void MainWindow::onGameRequested(RequestGameEvent* event)
{
    QMessageBox msgBox;
    msgBox.setText(QString{"Play with %1?"}.arg(event->opponent()));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    const auto res = msgBox.exec();
    if (res == QMessageBox::Yes)
    {
        m_player->postEvent(new GameAcceptedEvent{event->opponent()});
    }
    else
    {
        m_player->postEvent(
            new GameStartFailedEvent{QString{"declined by %1"}.arg(m_player->data()->name())});
    }
}

void MainWindow::onGameStarted(GameStartedEvent* event)
{
    m_statusLabel->setText(QString{"Game started with %1"}.arg(event->opponent()));
}
