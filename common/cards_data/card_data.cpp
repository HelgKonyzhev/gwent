#include "card_data.h"
#include <cards_data/cards_data.h>
#include <QJsonArray>

CardData::CardData(const QJsonObject &data, CardsData *parent)
    : QObject{parent}
    , m_data{data}
{
    qRegisterMetaType<CardData>();
}
