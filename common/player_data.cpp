#include "player_data.h"
#include <QJsonObject>

PlayerData::PlayerData(const QString& name, const QString& password)
    : m_name{name}
    , m_password{password}
{
}

Result PlayerData::fromJson(const QJsonObject& json)
{
    if (!json.contains("name"))
        return ResultError{"no \'name\' specified"};

    if (!json["name"].isString())
        return ResultError{"no \'name\' expected to be string"};

    m_name = json["name"].toString();

    if (!json.contains("password"))
        return ResultError{"no \'password\' specified"};

    if (!json["password"].isString())
        return ResultError{"no \'password\' expected to be string"};

    m_password = json["name"].toString();
    return {};
}

QJsonObject PlayerData::toJson() const
{
    QJsonObject json;
    json.insert("name", m_name);
    json.insert("password", m_password);
    return json;
}
