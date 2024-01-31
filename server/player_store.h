#pragma once
#include <common/player_data.h>
#include <optional>
#include <QJsonDocument>
#include <QFile>
#include <shared_mutex>
#include <common/result.h>

class PlayerStore
{
public:
    PlayerStore();
    ~PlayerStore();

    bool load();
    Result add(const PlayerData& user);
    ResultValue<PlayerData> get(const QString& username, const QString& password);

private:
    QFile m_usersFile;
    QJsonDocument m_usersJson;
    std::shared_mutex m_mtx;
};
