#include "player_store.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <mutex>
#include <common/deck.h>

constexpr char UsersFile[] = "users.json";

PlayerStore::PlayerStore() {}

PlayerStore::~PlayerStore() {}

bool PlayerStore::load()
{
    std::unique_lock lck{m_mtx};

    QFile usersFile{UsersFile};
    if (!usersFile.open(QIODeviceBase::Text | QIODeviceBase::ReadWrite))
        return false;

    const auto rawJson = usersFile.readAll();
    if (rawJson.isEmpty())
    {
        QJsonDocument json{QJsonObject{}};
        usersFile.write(json.toJson(QJsonDocument::Compact));
        usersFile.close();
        return true;
    }

    QJsonParseError error;
    const auto playersJs = QJsonDocument::fromJson(rawJson, &error).array();

    usersFile.close();

    if (error.error != QJsonParseError::NoError)
        return false;

    for (const auto& playerJs : playersJs)
    {
        auto playerData = QSharedPointer<PlayerData>{new PlayerData};
        playerData->fromJson(playerJs.toObject());
        m_players.insert(playerData->name(), playerData);
    }

    return true;
}

Result PlayerStore::add(const QString& playerName, const QString& password)
{
    std::unique_lock lck{m_mtx};

    if (m_players.contains(playerName))
        return ResultError{QString{"\'%1\' already registered"}.arg(playerName)};

    auto playerData = QSharedPointer<PlayerData>{new PlayerData(playerName, password)};
    m_players.insert(playerName, playerData);

    return save();
}

ResultValue PlayerStore::get(const QString& username, const QString& password)
{
    std::shared_lock lck{m_mtx};

    auto it = m_players.find(username);
    if (it == m_players.end())
        return ResultError{QString{"\'%1\' doesn't exist"}.arg(username)};

    if (it.value()->password() != password)
        return ResultError{QString{"wrong password"}};

    return QSharedPointer<PlayerData>{it.value()};
}

Result PlayerStore::save()
{
    QFile usersFile{UsersFile};
    if (!usersFile.open(QIODeviceBase::Text | QIODeviceBase::ReadWrite | QIODeviceBase::Truncate))
        return ResultError{"save failed"};

    QJsonArray playersJs;
    for (const auto& playerData : m_players)
        playersJs.append(playerData->toJson());

    usersFile.write(QJsonDocument{playersJs}.toJson(QJsonDocument::Indented));
    usersFile.close();
    return {};
}

Result PlayerStore::addDeck(const QString& playerName, const Deck& deck)
{
    std::shared_lock lck{m_mtx};

    auto playerIt = m_players.find(playerName);
    if (playerIt == m_players.end())
        return ResultError{QString{"no player \'%1\' registered"}.arg(playerName)};

    if (playerIt.value().data()->decks().contains(deck.name()))
        return ResultError{QString{"player \'%1\' already has deck \'%2\'"}.arg(playerName).arg(deck.name())};

    playerIt->data()->addDeck(deck);
    return save();
}

Result PlayerStore::updateDeck(const QString& playerName, Deck& deck)
{
    std::shared_lock lck{m_mtx};

    auto playerIt = m_players.find(playerName);
    if (playerIt == m_players.end())
        return ResultError{QString{"no player \'%1\' registered"}.arg(playerName)};

    if (!playerIt.value().data()->decks().contains(deck.name()))
        return ResultError{QString{"player \'%1\' has no deck \'%2\'"}.arg(playerName).arg(deck.name())};

    playerIt->data()->updateDeck(deck);
    return save();
}

Result PlayerStore::eraseDeck(const QString& playerName, const QString& deckName)
{
    std::shared_lock lck{m_mtx};

    auto playerIt = m_players.find(playerName);
    if (playerIt == m_players.end())
        return ResultError{QString{"no player \'%1\' registered"}.arg(playerName)};

    if (!playerIt.value().data()->decks().contains(deckName))
        return ResultError{QString{"player \'%1\' has no deck \'%2\'"}.arg(playerName).arg(deckName)};

    playerIt.value().data()->eraseDeck(deckName);
    return save();
}
