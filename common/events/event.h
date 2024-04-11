#pragma once
#include <QEvent>
#include <common/result.h>
#include <QMetaType>
#include <QObject>
#include <functional>

#ifdef EXPOSE_EVENTS_TO_QML
#include <QtQml/QtQml>
#endif

class QJsonObject;

template <typename>
struct event_type_info;

#define DECLARE_EVENT(type, tid)                   \
    template <>                                    \
    struct event_type_info<type>                   \
    {                                              \
        static constexpr Event::Type id = tid;     \
        static constexpr const char* name = #type; \
    };                                             \
    Q_DECLARE_METATYPE(type)

template <typename>
class EventRegistrator;

class Event
    : public QObject
    , public QEvent
{
    Q_OBJECT

    using EventCreator = std::function<Event*()>;

public:
    enum Type
    {
        LogIn = QEvent::Type::User,
        Register,
        Error,
        Registered,
        LoggedIn,
        LogInFailed,
        RegistrationFailed,
        LobbyUpdated,
        UpdateLobby,
        StartGame,
        GameStarted,
        GameStartFailed,
        RequestGame,
        GameAccepted,
    };
    Q_ENUM(Type)

    static Event* make(Type t);
    static ResultValue<Event*> fromJson(const QJsonObject& eventJs);
    static ResultValue<Event*> fromRawJson(const QByteArray& rawJson);

    template <typename T>
    static void registerEvent()
    {
        auto creator = [] { return new T(); };
        registerEvent(event_type_info<T>::id, creator);

#ifdef EXPOSE_EVENTS_TO_QML
        qmlRegisterType<T>("common.events", 1, 0, event_type_info<T>::name);
        qmlRegisterType<T*>("common.events", 1, 0, event_type_info<T>::name);
        qmlRegisterType<const T*>("common.events", 1, 0, event_type_info<T>::name);
#endif
    }

    static void registerEvent(Type tid, EventCreator cr);

    Event(Type t);

    template <typename T>
    T* to()
    {
        if (event_type_info<T>::id != static_cast<Type>(type()))
            return nullptr;
        return static_cast<T*>(this);
    }

    template <typename T>
    const T* to() const
    {
        if (event_type_info<T>::value != static_cast<Type>(type()))
            return nullptr;
        return static_cast<const T*>(this);
    }

    Result parse(const QJsonObject& eventJs);
    virtual Result parse(const QVariantHash& eventData);
    virtual QJsonObject toJson() const;

    QByteArray toRawJson() const;

    void setSource(QObject* source) { m_source = source; }
    QObject* source() const { return m_source; }

private:
    static constexpr size_t typeNum(Type tid)
    {
        return static_cast<size_t>(tid) - static_cast<size_t>(QEvent::Type::User);
    }

    QObject* m_source;
    static std::vector<EventCreator> m_eventCreators;
};

Q_DECLARE_METATYPE(Event);

const Event* castToEvent(const QEvent* event);
Event* castToEvent(QEvent* event);

bool operator==(Event::Type lh, QEvent::Type rh);
bool operator==(QEvent::Type lh, Event::Type rh);

template <typename T>
class EventRegistrator
{
public:
    EventRegistrator() { Event::registerEvent<T>(); }
};

#define REGISTER_EVENT(type)                          \
    namespace                                         \
    {                                                 \
    static EventRegistrator<type> __event_##type##__; \
    }
