#include "cards_set.h"
#include <common/cards_data/cards_data.h>
#include <common/cards_data/card_data.h>
#include <QMetaProperty>

size_t qHash(const QVariant& v)
{
    if (v.canConvert<QString>())
        return qHash(v.toString());
    if (v.canConvert<int>())
        return qHash(v.toInt());
    return 0;
}

CardsSet::CardsSet() { qRegisterMetaType<CardsSet>(); }

bool CardsSet::add(const CardData* card)
{
    if (get(card->name()))
        return false;

    auto mo = card->metaObject();
    for (int i = 0; i < mo->propertyCount(); ++i)
    {
        auto p = mo->property(i);

        if (strcmp(p.name(), "objectName") == 0)
            continue;

        auto& filter = m_cards[p.name()];
        auto value = p.read(card);

        if (value.typeId() == QMetaType::QString)
        {
            filter[value.toString()].insert(card);
        }
        if (value.typeId() == QMetaType::Int)
        {
            filter[value.toInt()].insert(card);
        }
        else if (value.typeId() == QMetaType::QStringList)
        {
            for (const auto& strVal : value.toStringList())
                filter[strVal].insert(card);
        }
    }
    m_all.insert(card);
    return true;
}

bool CardsSet::add(const QString& cardName)
{
    auto cd = CardsData::instance()->cards().get(cardName);
    if (!cd)
        return false;
    return add(cd);
}

QSet<const CardData*> CardsSet::get(const QHash<QString, QVariantList>& filters) const
{
    QSet<const CardData*> res;
    for (auto filterIt = filters.begin(); filterIt != filters.end(); ++filterIt)
    {
        auto filteredCardsIt = m_cards.find(filterIt.key());
        if (filteredCardsIt == m_cards.end())
            continue;

        for (const auto& filterVal : filterIt.value())
        {
            if (filterVal.canConvert<QString>() || filterVal.canConvert<int>())
            {
                auto cardIt = filteredCardsIt->find(filterVal.toString());
                if (cardIt == filteredCardsIt->end())
                    continue;

                res.unite(cardIt.value());
            }
            else if (filterVal.canConvert<QStringList>())
            {
            }
        }
    }
    return res;
}

QSet<const CardData*> CardsSet::get(const QString& filter, const QVariantList& values) const
{
    QHash<QString, QVariantList> filters;
    filters[filter] = values;
    return get(filters);
}

const CardData* CardsSet::get(const QString& cardName) const
{
    auto filterIt = m_cards.find("name");
    if (filterIt == m_cards.end())
        return nullptr;

    auto valuesIt = filterIt.value().find(cardName);
    if (valuesIt == filterIt.value().end())
        return nullptr;

    if (valuesIt.value().empty())
        return nullptr;

    return *valuesIt.value().begin();
}

bool CardsSet::erase(const QString& cardName)
{
    auto card = CardsData::instance()->cards().get(cardName);
    if (!card)
        return false;

    auto cardIt = m_all.find(card);
    if (cardIt == m_all.end())
        return false;

    for (auto filterIt = m_cards.begin(); filterIt != m_cards.end();)
    {
        for (auto valueIt = filterIt.value().begin(); valueIt != filterIt.value().end();)
        {
            auto cardIt = valueIt.value().find(card);
            if (cardIt != valueIt.value().end())
                valueIt.value().erase(cardIt);

            if (valueIt.value().empty())
                valueIt = filterIt.value().erase(valueIt);
            else
                ++valueIt;
        }

        if (filterIt.value().empty())
            filterIt = m_cards.erase(filterIt);
        else
            ++filterIt;
    }

    m_all.erase(cardIt);
    return true;
}

bool CardsSet::erase(const CardData* card) { return erase(card->name()); }
