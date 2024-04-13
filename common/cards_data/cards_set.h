#pragma once
#include <QString>
#include <QHash>
#include <QVariant>

class CardData;

class CardsSet
{
public:
    CardsSet();
    CardsSet(const CardsSet&) = default;
    CardsSet(CardsSet&&) = default;
    CardsSet& operator=(const CardsSet&) = default;
    CardsSet& operator=(CardsSet&&) = default;
    CardsSet(const QStringList& cards);

    bool add(const CardData* card);
    bool add(const QString& cardName);
    QSet<const CardData*> get(const QHash<QString, QVariantList>& filters) const;
    QSet<const CardData*> get(const QString& filter, const QVariantList& values) const;
    const CardData* get(const QString& cardName) const;
    const QList<const CardData*>& all() const { return m_all; }
    bool erase(const QString& cardName);
    bool erase(const CardData* card);
    qsizetype size() const { return m_all.size(); }
    const QStringList& allNames() const { return m_allNames; }

private:
    QHash<QString, QHash<QVariant, QSet<const CardData*>>> m_cards;
    QList<const CardData*> m_all;
    QStringList m_allNames;
};

Q_DECLARE_METATYPE(CardsSet)
