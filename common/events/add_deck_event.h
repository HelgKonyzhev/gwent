#pragma once
#include <QtQmlIntegration/qqmlintegration.h>
#include <common/events/event.h>

class AddDeckEvent : public Event
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString fraction READ fraction WRITE setFraction)
    Q_PROPERTY(QStringList cards READ cards WRITE setCards)

public:
    AddDeckEvent();
    AddDeckEvent(const QString& name, const QString& fraction, const QStringList& cards);

    void setName(const QString& name) { m_name = name; }
    const QString& name() const { return m_name; }

    void setFraction(const QString& fraction) { m_fraction = fraction; }
    const QString& fraction() const { return m_fraction; }

    void setCards(const QStringList& cards) { m_cards = cards; }
    const QStringList& cards() const { return m_cards; }

private:
    QString m_name;
    QString m_fraction;
    QStringList m_cards;
};

DECLARE_EVENT(AddDeckEvent, Event::AddDeck);

class DeckAddedEvent : public Event
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString name READ name WRITE setName)

public:
    DeckAddedEvent();
    DeckAddedEvent(const AddDeckEvent* event);

    void setName(const QString& name) { m_name = name; }
    const QString& name() const { return m_name; }

    void setFraction(const QString& fraction) { m_fraction = fraction; }
    const QString& fraction() const { return m_fraction; }

    void setCards(const QStringList& cards) { m_cards = cards; }
    const QStringList& cards() const { return m_cards; }

private:
    QString m_name;
    QString m_fraction;
    QStringList m_cards;
};

DECLARE_EVENT(DeckAddedEvent, Event::DeckAdded);

class DeckAddFailedEvent : public Event
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString reason READ reason WRITE setReason)

public:
    DeckAddFailedEvent();
    DeckAddFailedEvent(const QString& name, const QString& reason);

    void setName(const QString& name) { m_name = name; }
    const QString& name() const { return m_name; }

    void setReason(const QString& reason) { m_reason = reason; }
    const QString& reason() const { return m_reason; }

private:
    QString m_name;
    QString m_reason;
};

DECLARE_EVENT(DeckAddFailedEvent, Event::DeckAddFailed);
