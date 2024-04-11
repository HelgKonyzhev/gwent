#pragma once
#include <QtQmlIntegration/qqmlintegration.h>
#include <common/result.h>
#include <QObject>
#include <QHash>

class QJsonObject;
class Deck;

class PlayerData : public QObject
{
    Q_PROPERTY(QString name READ name CONSTANT);
    Q_PROPERTY(QString password READ password CONSTANT);

public:
    PlayerData();
    PlayerData(const QString& name, const QString& password);
    ~PlayerData() override;

    const QString& name() const { return m_name; }
    const QString& password() const { return m_password; }

    Result fromJson(const QJsonObject& json);
    Result fromVariant(const QVariantHash& data);
    QJsonObject toJson() const;
    QVariantHash toVariant() const;

private:
    QString m_name;
    QString m_password;
    QHash<QString, Deck*> m_decks;
};

Q_DECLARE_METATYPE(PlayerData);
