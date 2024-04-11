#include "player_data.h"
#include <QJsonObject>
#include <QMetaType>
#include <common/deck.h>

PlayerData::PlayerData() {}

PlayerData::PlayerData(const QString& name, const QString& password)
    : m_name{name}
    , m_password{password}
{
    qRegisterMetaType<PlayerData>();
}

PlayerData::~PlayerData()
{
    //
    //
}

Result PlayerData::fromJson(const QJsonObject& json) { return fromVariant(json.toVariantHash()); }

Result PlayerData::fromVariant(const QVariantHash& data)
{
    if (!data.contains("name"))
        return ResultError{"no \'name\' specified"};

    if (data["name"].typeId() != QMetaType::QString)
        return ResultError{"no \'name\' must to be string"};

    m_name = data["name"].toString();

    if (!data.contains("password"))
        return ResultError{"no \'password\' specified"};

    if (data["password"].typeId() != QMetaType::QString)
        return ResultError{"no \'password\' must to be string"};

    m_password = data["password"].toString();

    if (!data.contains("decks"))
        return ResultError{"no \'decks\' specified"};

    if (data["decks"].typeId() != QMetaType::QVariantList)
        return ResultError{"no \'decks\' must array of objects"};

    for (const auto& deckData : data["decks"].toList())
    {
        auto deck = new Deck{this};
        if (const auto res = deck->fromVariant(deckData.toHash()); !res)
            return ResultError{res.error()};
        m_decks.insert(deck->name(), deck);
    }

    return {};
}

QJsonObject PlayerData::toJson() const { return QJsonObject::fromVariantHash(toVariant()); }

QVariantHash PlayerData::toVariant() const
{
    QVariantHash data;
    data.insert("name", m_name);
    data.insert("password", m_password);

    QVariantList decks;
    for (auto deck : m_decks)
        decks.append(deck->toVariant());

    data.insert("decks", decks);
    return data;
}
