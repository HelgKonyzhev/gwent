#include "event.h"
#include <QJsonObject>
#include <QJsonDocument>
#include "login_event.h"
#include "register_event.h"
#include "error_event.h"
#include "lobby_updated_event.h"
#include "start_game_event.h"
#include "request_game_event.h"

Event::Event(Type t)
    : QEvent{static_cast<QEvent::Type>(t)}
{
}

Event* Event::make(Type t)
{
    switch (t)
    {
        case LogIn:
            return new LogInEvent();
        case Register:
            return new RegisterEvent();
        case Error:
            return new ErrorEvent();
        case Registered:
            return new RegisteredEvent();
        case LoggedIn:
            return new LoggedInEvent();
        case LogInFailed:
            return new LogInFailedEvent();
        case RegistrationFailed:
            return new RegistrationFailedEvent();
        case LobbyUpdated:
            return new LobbyUpdatedEvent();
        case UpdateLobby:
            return new UpdateLobbyEvent();
        case StartGame:
            return new StartGameEvent();
        case GameStarted:
            return new GameStartedEvent();
        case GameStartFailed:
            return new GameStartFailedEvent();
        case RequestGame:
            return new RequestGameEvent();
        case GameAccepted:
            return new GameAcceptedEvent();
        default:
            return nullptr;
    };
}

ResultValue<Event*> Event::fromJson(const QJsonObject& eventJs)
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

ResultValue<Event*> Event::fromRawJson(const QByteArray& rawJson)
{
    QJsonParseError error;
    auto json = QJsonDocument::fromJson(rawJson, &error);
    if (error.error != QJsonParseError::NoError)
        return ResultError{"failed to parse: ", error.error};

    if (!json.isObject())
        return ResultError{"expected json object"};

    return fromJson(json.object());
}

QJsonObject Event::toJson() const
{
    QJsonObject eventJs;
    eventJs.insert("type", {type()});
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
