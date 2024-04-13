#pragma once
#include <QtQmlIntegration/qqmlintegration.h>
#include <common/result.h>
#include <QObject>
#include <QHash>

class QJsonObject;
class Deck;

class PlayerData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name CONSTANT);
    Q_PROPERTY(QString password READ password CONSTANT);
    Q_PROPERTY(QStringList deckNames READ deckNames CONSTANT);

public:
    PlayerData();
    PlayerData(const QString& name, const QString& password);
    ~PlayerData() override;

    const QString& name() const { return m_name; }
    const QString& password() const { return m_password; }
    const QHash<QString, Deck*>& decks() const { return m_decks; }
    QStringList deckNames() const { return m_decks.keys(); }

    Result fromJson(const QJsonObject& json);
    Result fromVariant(const QVariantHash& data);
    QJsonObject toJson() const;
    QVariantHash toVariant() const;

    bool addDeck(const Deck& deck);
    bool updateDeck(const Deck& deck);
    bool eraseDeck(const QString& name);
    Q_INVOKABLE Deck* getDeck(const QString& name) const;

private:
    QString m_name;
    QString m_password;
    QHash<QString, Deck*> m_decks;
};

Q_DECLARE_METATYPE(PlayerData);
