#pragma once
#include <QString>
#include <QHash>
#include <QVariant>

class CardData;

class CardsSet
{
public:
    CardsSet();

    bool add(const CardData* card);
    bool add(const QString& cardName);
    QSet<const CardData*> get(const QHash<QString, QVariantList> &filters) const;
    const CardData* get(const QString& cardName) const;
    const QSet<const CardData*>& getAll() const { return m_all; }

private:
    QHash<QString, QHash<QVariant, QSet<const CardData*>>> m_cards;
    QSet<const CardData*> m_all;
};
