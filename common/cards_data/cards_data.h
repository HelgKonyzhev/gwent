#pragma once
#include <QObject>
#include <QMap>
#include <QSet>
#include <common/cards_data/cards_set.h>

class CardData;

class CardsData : public QObject
{
    using CardsDataVector = QVector<CardData*>;
    using CardsDataConstIterator = CardsDataVector::const_iterator;

public:
    static CardsData* instance();

    const CardsSet& cards() const { return m_cards; }

private:
    CardsData();

    CardsSet m_cards;
};
