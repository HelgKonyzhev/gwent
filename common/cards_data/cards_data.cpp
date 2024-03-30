#include "cards_data.h"
#include <QFile>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "card_data.h"
#include <QMetaProperty>

namespace
{
constexpr char CardsDataPath[] = "cards_data/cards_data.json";
}

CardsData* CardsData::instance()
{
    static CardsData inst;
    return &inst;
}

CardsData::CardsData()
{
    QFile cardsDataFile{QString{":/%1"}.arg(CardsDataPath)};
    if (!cardsDataFile.open(QIODevice::ReadOnly))
    {
        qCritical() << "failed to open" << CardsDataPath;
        return;
    }

    QJsonParseError error;
    QJsonDocument cardsDataJS = QJsonDocument::fromJson(cardsDataFile.readAll(), &error);
    cardsDataFile.close();

    if (error.error != QJsonParseError::NoError)
    {
        qCritical() << error.errorString();
        return;
    }

    if (!cardsDataJS.isObject())
    {
        qCritical() << "root is no object";
        return;
    }

    if (!cardsDataJS.object()["cards"].isObject())
    {
        qCritical() << "no \"cards\" object found";
        return;
    }

    CardData cardDataSample{{}, nullptr};
    const auto& cardDataSampleMO = cardDataSample.metaObject();

    for (const auto& fractionJS : cardsDataJS.object()["cards"].toObject())
    {
        for (const auto& cardJS : fractionJS.toArray())
        {
            if (!cardJS.isObject())
            {
                qWarning() << "skipping" << cardJS.type();
                continue;
            }

            const auto& cardObj = cardJS.toObject();

            bool fieldsMatch = true;
            for (int i = 0; i < cardDataSampleMO->propertyCount(); ++i)
            {
                const auto& property = cardDataSampleMO->property(i);
                if (strcmp(property.name(), "objectName") == 0)
                    continue;

                if (!cardObj.contains(property.name()))
                {
                    qWarning() << "skipping card with no" << property.name() << "specified";
                    fieldsMatch = false;
                    break;
                }

                const auto propertyValue = cardObj[property.name()].toVariant();
                if (!propertyValue.canConvert(property.metaType()))
                {
                    qWarning() << property.name() << " of type " << property.typeId() << " but "
                               << propertyValue.typeId() << " specified. Skipping.";
                    fieldsMatch = false;
                    break;
                }
            }

            if (!fieldsMatch)
                continue;

            if (m_cards.get(cardObj["name"].toString()))
            {
                qWarning() << "duplicated name" << cardObj["name"].toString();
                continue;
            }

            auto cd = new CardData{cardObj, this};
            m_cards.add(cd);
            qDebug() << cd->name() << "loaded";
        }
    }
}
