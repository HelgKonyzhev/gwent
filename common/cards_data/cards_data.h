#pragma once
#include <QMap>
#include <QSet>
#include <QObject>
#include <common/cards_data/cards_set.h>

class CardData;

class CardsData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(const QStringList& allNames READ allNames CONSTANT)
    Q_PROPERTY(const QList<const CardData*>& all READ all CONSTANT)
    Q_PROPERTY(qsizetype size READ size CONSTANT)

public:
    static CardsData* instance();

    const CardsSet& cards() const { return m_cards; }

    Q_INVOKABLE QSet<const CardData*> get(const QHash<QString, QVariantList>& filters) const
    {
        return m_cards.get(filters);
    }

    Q_INVOKABLE QSet<const CardData*> get(const QString& filter, const QVariantList& values) const
    {
        return m_cards.get(filter, values);
    }

    Q_INVOKABLE const CardData* get(const QString& cardName) const { return m_cards.get(cardName); }

    const QStringList& allNames() const { return m_cards.allNames(); }
    const QList<const CardData*>& all() const { return m_cards.all(); }
    qsizetype size() const { return m_cards.size(); }

private:
    CardsData();

    CardsSet m_cards;
};

Q_DECLARE_METATYPE(CardsData)
