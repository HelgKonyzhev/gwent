#include "event.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QMetaProperty>

std::vector<Event::EventCreator>& Event::eventCreators()
{
    static std::vector<Event::EventCreator> creators;
    return creators;
}

void Event::registerEvent(Type tid, EventCreator creator)
{
    auto& ec = eventCreators();

    const auto tn = typeNum(tid);
    if (tn >= ec.size())
        ec.resize(tn + 1);
    ec[tn] = creator;
}

Event::Event(Type t)
    : QEvent{static_cast<QEvent::Type>(t)}
{
    qRegisterMetaType<Event>();
}

Event* Event::make(Type t)
{
    auto& ec = eventCreators();

    const auto tn = typeNum(t);
    if (tn > ec.size())
        return nullptr;
    return ec[tn]();
}

ResultValue Event::fromJson(const QJsonObject& eventJs)
{
    if (!eventJs.contains("type"))
        return ResultError{"\'type\' not specified"};

    if (!eventJs["type"].isDouble())
        return ResultError{"\'type\' must be integer"};

    try
    {
        auto event = make(static_cast<Type>(eventJs["type"].toInt()));
        event->parse(eventJs).value();
        return event;
    }
    catch (const std::logic_error& ex)
    {
        return ResultError{ex.what()};
    }
}

ResultValue Event::fromRawJson(const QByteArray& rawJson)
{
    QJsonParseError error;
    auto json = QJsonDocument::fromJson(rawJson, &error);
    if (error.error != QJsonParseError::NoError)
        return ResultError{QString{"failed to parse: %1"}.arg(error.error)};

    if (!json.isObject())
        return ResultError{"expected json object"};

    return fromJson(json.object());
}

Result Event::parse(const QJsonObject& eventJs) { return parse(eventJs.toVariantHash()); }

Result Event::parse(const QVariantHash& eventData)
{
    for (int i = 0; i < metaObject()->propertyCount(); ++i)
    {
        auto property = metaObject()->property(i);
        if (strcmp(property.name(), "objectName") == 0)
            continue;

        if (!eventData.contains(property.name()))
            return ResultError{QString{"\'%1\' not specified"}.arg(property.name())};

        const auto propertyTid = property.typeId();
        const auto dataTid = eventData[property.name()].typeId();

        if (dataTid == propertyTid)
        {
            property.write(this, eventData[property.name()]);
        }
        else if (propertyTid == QMetaType::QStringList && dataTid == QMetaType::QVariantList)
        {
            property.write(this, eventData[property.name()].toStringList());
        }
        else
        {
            return ResultError{QString{"\'%1\' must be of type %2, but it is of type %3"}
                                   .arg(property.name())
                                   .arg(property.typeName())
                                   .arg(eventData[property.name()].typeName())};
        }
    }
    return {};
}

QJsonObject Event::toJson() const
{
    QJsonObject eventJs;
    eventJs.insert("type", {type()});

    for (int i = 0; i < metaObject()->propertyCount(); ++i)
    {
        auto property = metaObject()->property(i);
        if (strcmp(property.name(), "objectName") == 0)
            continue;

        eventJs[property.name()] = property.read(this).toJsonValue();
    }

    return eventJs;
}

QByteArray Event::toRawJson() const
{
    QJsonDocument eventJs{toJson()};
    return eventJs.toJson(QJsonDocument::Compact);
}

const Event* castToEvent(const QEvent* event)
{
    if (event->type() < QEvent::Type::User)
        return nullptr;
    return static_cast<const Event*>(event);
}

Event* castToEvent(QEvent* event)
{
    if (event->type() < QEvent::Type::User)
        return nullptr;
    return static_cast<Event*>(event);
}

bool operator==(Event::Type lh, QEvent::Type rh) { return lh == static_cast<Event::Type>(rh); }

bool operator==(QEvent::Type lh, Event::Type rh) { return rh == static_cast<Event::Type>(lh); }
