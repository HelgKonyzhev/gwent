#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QWebSocket>
#include <QQmlContext>
#include <common/socket.h>
#include "quick_player.h"
#include <common/player_data.h>
#include <common/cards_data/card_data.h>
#include <common/cards_data/cards_data.h>
#include <common/cards_data/cards_set.h>
#include <common/events/event.h>
#include <common/doorstep_state.h>
#include <common/lobby_state.h>
#include <common/deck.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/qml/main.qml"_qs);
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreationFailed, &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    auto socket = new Socket{new QWebSocket{"", QWebSocketProtocol::VersionLatest}, &app};
    auto player = new QuickPlayer{socket, &app};

    qmlRegisterUncreatableType<Event>("common.events", 1, 0, "Event", "Not creatable base event class");

    qmlRegisterType<PlayerData>("common", 1, 0, "PlayerData");
    qmlRegisterType<PlayerData *>("common", 1, 0, "PlayerData");
    qmlRegisterType<const PlayerData *>("common", 1, 0, "PlayerData");
    qmlRegisterType<QScopedPointer<PlayerData>>("common", 1, 0, "PlayerData");

    qmlRegisterType<DoorstepState>("common", 1, 0, "DoorstepState");
    qmlRegisterType<DoorstepState *>("common", 1, 0, "DoorstepState");

    qmlRegisterType<LobbyState>("common", 1, 0, "DoorstepState");
    qmlRegisterType<LobbyState *>("common", 1, 0, "DoorstepState");

    qmlRegisterType<CardData>("common.card_data", 1, 0, "CardData");
    qmlRegisterType<CardData *>("common.card_data", 1, 0, "CardData");
    qmlRegisterType<const CardData *>("common.card_data", 1, 0, "CardData");

    qmlRegisterType<CardsData *>("common.cards_data", 1, 0, "CardsData");
    qmlRegisterType<const CardsData *>("common.cards_data", 1, 0, "CardsData");

    qmlRegisterType<CardsSet>("common.cards_set", 1, 0, "CardsSet");
    qmlRegisterType<CardsSet *>("common.cards_set", 1, 0, "CardsSet");
    qmlRegisterType<const CardsSet *>("common.cards_set", 1, 0, "CardsSet");

    qmlRegisterType<Deck>("common", 1, 0, "Deck");
    qmlRegisterType<Deck *>("common", 1, 0, "Deck");
    qmlRegisterType<const Deck *>("common", 1, 0, "Deck");

    engine.rootContext()->setContextProperty("socket", socket);
    engine.rootContext()->setContextProperty("player", player);
    engine.rootContext()->setContextProperty("cardsData", CardsData::instance());

    engine.load(url);

    QTimer connectTimer;
    connectTimer.setInterval(100);

    QObject::connect(&connectTimer, &QTimer::timeout,
                     [&]
                     {
                         const auto state = socket->state();
                         if (state == QAbstractSocket::UnconnectedState)
                             socket->open(QUrl{"ws://localhost:8888"});
                     });

    connectTimer.start();
    return app.exec();
}
