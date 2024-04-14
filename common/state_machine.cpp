#include "state_machine.h"

QList<EventTransition *> StateMachine::findAllTransitions(QAbstractState *state, Event::Type eventType)
{
    QList<EventTransition *> eventTrns;
    auto allTrns = state->findChildren<EventTransition *>();
    if (allTrns.isEmpty())
        return eventTrns;

    for (auto trn : allTrns)
    {
        if (trn->eventType() == eventType)
            eventTrns.append(trn);
    }
    return eventTrns;
}

EventTransition *StateMachine::findTransition(QAbstractState *state, Event::Type eventType)
{
    const auto trns = findAllTransitions(state, eventType);
    return trns.isEmpty() ? nullptr : trns.front();
}

QList<EventTransition *> StateMachine::findAllTransitions(Event::Type eventType)
{
    return findAllTransitions(this, eventType);
}

EventTransition *StateMachine::findTransition(Event::Type eventType)
{
    return findTransition(this, eventType);
}
