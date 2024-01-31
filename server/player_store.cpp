#include "player_store.h"
#include <QJsonObject>
#include <mutex>

constexpr char UsersFile[] = "users.json";

PlayerStore::PlayerStore()
    : m_usersFile{UsersFile}
{
}

PlayerStore::~PlayerStore() {}

bool PlayerStore::load()
{
    std::unique_lock lck{m_mtx};

    if (!m_usersFile.open(QIODeviceBase::Text | QIODeviceBase::ReadWrite))
        return false;

    const auto rawJson = m_usersFile.readAll();
    if (rawJson.isEmpty())
    {
        QJsonDocument json{QJsonObject{}};
        m_usersFile.write(json.toJson(QJsonDocument::Compact));
        m_usersFile.close();
        return true;
    }

    QJsonParseError error;
    m_usersJson = QJsonDocument::fromJson(rawJson, &error);

    m_usersFile.close();

    if (error.error != QJsonParseError::NoError)
        return false;

    if (!m_usersJson.isObject())
    {
        m_usersJson = {};
        return false;
    }

    return true;
}

Result PlayerStore::add(const PlayerData& user)
{
    std::unique_lock lck{m_mtx};

    auto usersObj = m_usersJson.object();
    if (usersObj.contains(user.name()))
        return ResultError{"\'", user.name(), "\'", " aleady registered"};

    if (!m_usersFile.open(QIODeviceBase::Text | QIODeviceBase::ReadWrite))
        return ResultError{"save failed"};

    QJsonObject userObj;
    userObj.insert("password", user.password());
    usersObj.insert(user.name(), userObj);
    m_usersJson.setObject(usersObj);
    m_usersFile.write(m_usersJson.toJson(QJsonDocument::Indented));
    m_usersFile.close();
    return {};
}

ResultValue<PlayerData> PlayerStore::get(const QString& username, const QString& password)
{
    std::shared_lock lck{m_mtx};

    const auto usersObj = m_usersJson.object();
    const auto it = usersObj.find(username);
    if (it == usersObj.end())
        return ResultError{"\'", username, "\'", " doesn't exist"};

    const auto storedPassword = it.value().toObject()["password"].toString();
    if (password != storedPassword)
        return ResultError{QString{"wrong password"}};
    
    auto user = PlayerData{username, password};
    return user;
}
