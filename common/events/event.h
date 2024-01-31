#pragma once
#include <QEvent>
#include <common/result.h>
#include <QMetaType>

class QJsonObject;

template <typename>
struct to_type_id;

#define TO_TYPE_ID(type, id)                     \
    template <>                                  \
    struct to_type_id<type>                      \
    {                                            \
        static constexpr Event::Type value = id; \
    };

class Event : public QEvent
{
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

    static Event* make(Type t);
    static ResultValue<Event*> fromJson(const QJsonObject& eventJs);
    static ResultValue<Event*> fromRawJson(const QByteArray& rawJson);

    Event(Type t);

    template <typename T>
    T* to()
    {
        if (to_type_id<T>::value != static_cast<Type>(type()))
            return nullptr;
        return static_cast<T*>(this);
    }

    template <typename T>
    const T* to() const
    {
        if (to_type_id<T>::value != static_cast<Type>(type()))
            return nullptr;
        return static_cast<const T*>(this);
    }

    virtual Result parse(const QJsonObject& eventJs) = 0;
    virtual QJsonObject toJson() const;

    QByteArray toRawJson() const;

    void setSource(QObject* source) { m_source = source; }
    QObject* source() const { return m_source; }

private:
    QObject* m_source;
};

const Event* castToEvent(const QEvent* event);
Event* castToEvent(QEvent* event);

bool operator==(Event::Type lh, QEvent::Type rh);
bool operator==(QEvent::Type lh, Event::Type rh);
