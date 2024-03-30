#pragma once
#include <common/result.h>
#include <QStringList>
#include <QObject>
#include <QMap>
#include <common/cards_data/cards_set.h>

class QJsonObject;

class Check : public QObject
{
    Q_OBJECT;
    Q_PROPERTY(QVariant value READ value WRITE setValue NOTIFY valueChanged);

public:
    enum Type
    {
        Name = 0,
        Fraction,
        SpecialCards,
        SquadCards,
    };

    Check(Type type, const QVariant& thersholdValue)
        : m_type{type}
        , m_thresholdValue{thersholdValue}
    {
    }

    Type type() const { return m_type; }
    const auto& value() const { return m_value; }
    bool setValue(QVariant val);

    const auto& thresholdValue() const { return m_thresholdValue; }
    bool isValid() const { return testValue(m_value); }

    virtual bool testValue(const QVariant& val) const = 0;

signals:
    void valueChanged(const QVariant&);

private:
    const Type m_type;
    const QVariant m_thresholdValue;
    QVariant m_value;
};

class Deck : public QObject
{
public:
    Deck();

    Result fromJson(const QJsonObject& json);
    QJsonObject toJson() const;

    bool setName(const QString& name);
    const QString& name() const { return m_name; }

    bool setFraction(const QString& fraction);
    const QString& fraction() const { return m_fraction; }

    QVector<const Check*> checks() const;

    bool isValid() const;
    QVector<const Check*> check() const;

private:
    QString m_name;
    QString m_fraction;
    QStringList m_cardNames;
    QVector<Check*> m_checks;
};
