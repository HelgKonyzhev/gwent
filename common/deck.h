#pragma once
#include <QtQmlIntegration/qqmlintegration.h>
#include <common/result.h>
#include <QStringList>
#include <QObject>
#include <QMap>
#include <common/cards_data/cards_set.h>
#include <optional>

class QJsonObject;

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

public:
    static constexpr int SquadsMinCount = 22;
    static constexpr int SpecialsMaxCount = 10;

    Deck(QObject* parent);

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

    QPair<bool, bool> addCard(const QString& cardName);
    QPair<bool, bool> addCard(const CardData* card);
    const auto& cards() const { return m_cards; }

    Limit squadsLimit() const;
    Limit specialsLimit() const;

    bool valid() const;

private:
    QString m_name;
    QString m_fraction;
    CardsSet m_cards;
};
