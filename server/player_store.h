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

private:
    Result save();

    std::shared_mutex m_mtx;
    QHash<QString, QSharedPointer<PlayerData>> m_players;
};
