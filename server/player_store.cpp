#include "player_store.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <mutex>

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
        return ResultError{"\'", playerName, "\'", " aleady registered"};

    auto playerData = QSharedPointer<PlayerData>{new PlayerData(playerName, password)};
    m_players.insert(playerName, playerData);

    return save();
}

ResultValue<QSharedPointer<PlayerData> > PlayerStore::get(const QString& username, const QString& password)
{
    std::shared_lock lck{m_mtx};

    auto it = m_players.find(username);
    if (it == m_players.end())
        return ResultError{"\'", username, "\'", " doesn't exist"};

    if (it.value()->password() != password)
        return ResultError{QString{"wrong password"}};

    return it.value();
}

Result PlayerStore::save()
{
    QFile usersFile{UsersFile};
    if (!usersFile.open(QIODeviceBase::Text | QIODeviceBase::ReadWrite))
        return ResultError{"save failed"};

    QJsonArray playersJs;
    for (const auto& playerData : m_players)
        playersJs.append(playerData->toJson());

    usersFile.write(QJsonDocument{playersJs}.toJson(QJsonDocument::Indented));
    usersFile.close();
    return {};
}
