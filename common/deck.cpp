#include "deck.h"
#include <common/cards_data/cards_data.h>
#include <common/cards_data/card_data.h>
#include <QJsonArray>

Deck::Deck(QObject* parent)
    : QObject{parent}
{
    qRegisterMetaType<Deck>();
}

Deck::Deck(const QString& name, const QString fraction, const QStringList& cards, QObject* parent)
    : QObject{parent}
    , m_name{name}
    , m_fraction{fraction}
    , m_cards{cards}
{
    qRegisterMetaType<Deck>();
}

Result Deck::fromJson(const QJsonObject& deckJs) { return fromVariant(deckJs.toVariantHash()); }

Result Deck::fromVariant(const QVariantHash& deckData)
{
    if (!deckData.contains("name"))
        return ResultError{"\'name\' not specified"};

    if (deckData["name"].typeId() != QMetaType::QString)
        return ResultError{"\'name\' must be string"};

    m_name = deckData["name"].toString();

    if (!deckData.contains("fraction"))
        return ResultError{"\'fraction\' not specified"};

    if (deckData["fraction"].typeId() != QMetaType::QString)
        return ResultError{"\'fraction\' must be string"};

    m_fraction = deckData["fraction"].toString();

    if (!deckData.contains("cards"))
        return ResultError{"\'cards\' not specified"};

    const auto cards = deckData["cards"];
    if (cards.typeId() != QMetaType::QVariantList && cards.typeId() != QMetaType::QStringList)
        return ResultError{"\'cards\' must be array of strings"};

    for (const auto& cardName : cards.toStringList())
    {
        if (!m_cards.add(cardName))
            return ResultError{QString{"failed to add card\'%1\'"}.arg(cardName)};
    }

    return {};
}

QJsonObject Deck::toJson() const { return QJsonObject::fromVariantHash(toVariant()); }

QVariantHash Deck::toVariant() const
{
    QVariantHash deckData;
    deckData["name"] = name();
    deckData["fraction"] = fraction();

    QStringList cards;
    for (const auto& card : m_cards.all())
        cards.append(card->name());

    deckData["cards"] = cards;
    return deckData;
}

bool Deck::setName(const QString& name)
{
    if (name.isEmpty())
        return false;

    m_name = name;
    return true;
}

bool Deck::setFraction(const QString& fraction)
{
    m_fraction = fraction;
    return true;
}

bool Deck::setLeader(const QString& leaderName)
{
    auto leader = CardsData::instance()->cards().get(leaderName);
    if (!leader)
        return false;

    return setLeader(leader);
}

bool Deck::setLeader(const CardData* leader)
{
    if (!leader->tags().contains("leader"))
        return false;

    auto oldLeader = this->leader();
    if (oldLeader)
        eraseCard(oldLeader);

    return doAddCard(leader);
}

const CardData* Deck::leader() const
{
    auto res = m_cards.get("tags", {"leader"});
    if (res.empty())
        return nullptr;

    return *res.begin();
}

bool Deck::addCard(const QString& cardName)
{
    auto card = CardsData::instance()->cards().get(cardName);
    if (!card)
        return false;

    return addCard(card);
}

bool Deck::addCard(const CardData* card)
{
    if (card->tags().contains("leader"))
        return setLeader(card);
    return doAddCard(card);
}

bool Deck::eraseCard(const QString& cardName)
{
    auto card = CardsData::instance()->cards().get(cardName);
    if (!card)
        return false;

    return eraseCard(card);
}

bool Deck::eraseCard(const CardData* card)
{
    const auto erased = m_cards.erase(card);
    if (erased)
        emit cardErased(card);
    return erased;
}

Limit Deck::squadsLimit() const
{
    const auto squadsCount = m_cards.get("tags", {"squad"}).size();
    return {SquadsMinCount, static_cast<int>(squadsCount), squadsCount >= SpecialsMaxCount};
}

Limit Deck::specialsLimit() const
{
    const auto specialsCount = m_cards.get("tags", {"special"}).size();
    return {SpecialsMaxCount, static_cast<int>(specialsCount), specialsCount <= SpecialsMaxCount};
}

bool Deck::valid() const
{
    return !name().isEmpty() && !fraction().isEmpty() && hasLeader() && squadsLimit() && specialsLimit();
}

void Deck::copy(const Deck* other)
{
    m_name = other->name();
    m_fraction = other->fraction();
    m_cards = other->cards();
}

bool Deck::doAddCard(const CardData* card)
{
    const auto added = m_cards.add(card);
    if (added)
        emit cardAdded(card);
    return added;
}
