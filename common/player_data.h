#pragma once
#include <common/result.h>

class QJsonObject;

class PlayerData
{
public:
    PlayerData() = default;
    PlayerData(const QString& name, const QString& password);

    const QString& name() const { return m_name; }
    const QString& password() const { return m_password; }

    Result fromJson(const QJsonObject& json);
    QJsonObject toJson() const;

private:
    QString m_name;
    QString m_password;
};
