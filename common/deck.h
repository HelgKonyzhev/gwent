#pragma once
#include <QtQmlIntegration/qqmlintegration.h>
#include <common/result.h>
#include <QStringList>
#include <QObject>
#include <QMap>
#include <common/cards_data/cards_set.h>
#include <optional>

class QJsonObject;

Q_DECLARE_OPAQUE_POINTER(CardData*);
Q_DECLARE_OPAQUE_POINTER(const CardData*);

class Limit : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int thresholdValue READ thresholdValue CONSTANT);
    Q_PROPERTY(int value READ value CONSTANT);
    Q_PROPERTY(bool valid READ valid CONSTANT);

public:
    Limit(int thresholdValue, int value, bool valid)
        : m_thresholdValue{thresholdValue}
        , m_value{value}
        , m_valid{valid}
    {
    }

    auto thresholdValue() const { return m_thresholdValue; }
    auto value() const { return m_value; }
    auto valid() const { return m_valid; }

    operator bool() const { return valid(); }

private:
    const int m_thresholdValue;
    const int m_value;
    const bool m_valid;
};

class Deck : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString fraction READ fraction WRITE setFraction NOTIFY fractionChanged)
    Q_PROPERTY(const CardData* leader READ leader WRITE setLeader NOTIFY leaderChanged)
    Q_PROPERTY(const QStringList& allNames READ allNames CONSTANT)
    Q_PROPERTY(const QList<const CardData*>& all READ all CONSTANT)

public:
    static constexpr int SquadsMinCount = 22;
    static constexpr int SpecialsMaxCount = 10;

    Deck(QObject* parent);
    Deck(const QString& name, const QString fraction, const QStringList& cards, QObject* parent);

    Result fromJson(const QJsonObject& deckJs);
    Result fromVariant(const QVariantHash& deckData);
    QJsonObject toJson() const;
    QVariantHash toVariant() const;

    bool setName(const QString& name);
    const QString& name() const { return m_name; }

    bool setFraction(const QString& fraction);
    const QString& fraction() const { return m_fraction; }

    bool setLeader(const QString& leaderName);
    bool setLeader(const CardData* leader);
    const CardData* leader() const;
    bool hasLeader() const { return leader() != nullptr; }

    Q_INVOKABLE bool addCard(const QString& cardName);
    Q_INVOKABLE bool addCard(const CardData* card);
    Q_INVOKABLE bool eraseCard(const QString& cardName);
    Q_INVOKABLE bool eraseCard(const CardData* card);
    const CardsSet& cards() const { return m_cards; }

    Limit squadsLimit() const;
    Limit specialsLimit() const;

    bool valid() const;
    void copy(const Deck* other);

    const QStringList& allNames() const { return m_cards.allNames(); }
    const QList<const CardData*> all() const { return m_cards.all(); }

signals:
    Q_INVOKABLE void nameChanged(const QString&);
    Q_INVOKABLE void fractionChanged(const QString&);
    Q_INVOKABLE void leaderChanged(const CardData*);
    Q_INVOKABLE void cardAdded(const CardData*);
    Q_INVOKABLE void cardErased(const CardData*);

private:
    bool doAddCard(const CardData* card);

    QString m_name;
    QString m_fraction;
    CardsSet m_cards;
};

Q_DECLARE_METATYPE(Deck)
