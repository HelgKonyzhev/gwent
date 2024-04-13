#include "quick_player.h"
#include <common/events/login_event.h>
#include <common/events/register_event.h>
#include <QStateMachine>
#include <common/lobby_state.h>
#include <common/deck.h>
#include <common/events/add_deck_event.h>
#include <common/events/update_deck_event.h>

class QuickPlayer::EditingDeck : public QObject
{
public:
    EditingDeck(EditionType t, Deck *deck)
        : m_type{t}
        , m_deck{deck}
    {
        m_deck->setParent(this);
    }

    auto type() const { return m_type; }
    Deck *deck() const { return m_deck; }

private:
    EditionType m_type;
    Deck *m_deck;
};

QuickPlayer::QuickPlayer(Socket *socket, QObject *parent)
    : Player{socket, parent}
{
    qRegisterMetaType<QuickPlayer>();

    connect(lobbyState(), &LobbyState::exited, this, [&](QEvent *) { delete m_editingDeck; });
}

void QuickPlayer::login(const QString &username, const QString &password)
{
    postEvent(new LogInEvent{username, password});
}

void QuickPlayer::registerPlayer(const QString &username, const QString &password)
{
    postEvent(new RegisterEvent{username, password});
}

Deck *QuickPlayer::addDeck()
{
    if (!fsm()->configuration().contains(lobbyState()))
        return nullptr;

    delete m_editingDeck;

    m_editingDeck = new EditingDeck{Adding, new Deck{this}};

    {
        m_editingDeck->deck()->setFraction("monsters");
        m_editingDeck->deck()->setLeader("eredin_1");
    }

    return m_editingDeck->deck();
}

Deck *QuickPlayer::updateDeck(const QString &name)
{
    if (!fsm()->configuration().contains(lobbyState()))
        return nullptr;

    const auto it = data()->decks().find(name);
    if (it == data()->decks().end())
        return nullptr;

    delete m_editingDeck;

    m_editingDeck = new EditingDeck{Updating, new Deck{this}};
    m_editingDeck->deck()->copy(it.value());
    return m_editingDeck->deck();
}

void QuickPlayer::cancelEdition() { delete m_editingDeck; }

Deck *QuickPlayer::editingDeck() const { return m_editingDeck->deck(); }

Q_INVOKABLE bool QuickPlayer::saveDeck()
{
    if (!m_editingDeck)
        return false;

    if (m_editingDeck->type() == Adding)
        postEvent(new AddDeckEvent{m_editingDeck->deck()->name(), m_editingDeck->deck()->fraction(),
                                   m_editingDeck->deck()->allNames()});
    else
        postEvent(new UpdateDeckEvent{m_editingDeck->deck()->name(), m_editingDeck->deck()->fraction(),
                                      m_editingDeck->deck()->allNames()});

    return true;
}
