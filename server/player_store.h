#pragma once
#include <common/player_data.h>
#include <optional>
#include <QJsonDocument>
#include <QFile>
#include <shared_mutex>
#include <common/result.h>
#include <QObject>
#include <QSharedPointer>

class PlayerStore : public QObject
{
public:
    PlayerStore();
    ~PlayerStore();

    bool load();
    Result add(const QString& playerName, const QString& password);
    ResultValue get(const QString& username, const QString& password);
    Result addDeck(const QString& playerName, const Deck &deck);
    Result updateDeck(const QString& playerName, class Deck& deck);
    Result eraseDeck(const QString& playerName, const QString& deckName);

private:
    Result save();

    std::shared_mutex m_mtx;
    QHash<QString, QSharedPointer<PlayerData>> m_players;
};
