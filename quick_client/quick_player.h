#pragma once
#include <common/player.h>
#include <QPointer>

class Deck;

Q_DECLARE_OPAQUE_POINTER(Deck*);

class QuickPlayer : public Player
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(Deck* editingDeck READ editingDeck CONSTANT)

    enum EditionType
    {
        Adding,
        Updating
    };

    class EditingDeck;

public:
    QuickPlayer(Socket* socket, QObject* parent = nullptr);

    Q_INVOKABLE void login(const QString& username, const QString& password);
    Q_INVOKABLE void registerPlayer(const QString& username, const QString& password);
    Q_INVOKABLE Deck* addDeck();
    Q_INVOKABLE Deck* updateDeck(const QString& name);
    Q_INVOKABLE void cancelEdition();
    Deck* editingDeck() const;
    Q_INVOKABLE bool saveDeck();

private:
    QPointer<EditingDeck> m_editingDeck;
};

Q_DECLARE_METATYPE(QuickPlayer);
