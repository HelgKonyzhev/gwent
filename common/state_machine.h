#pragma once
#include <QStateMachine>
#include <common/events/event.h>
#include <common/states/state.h>
#include <common/event_transition.h>

class StateMachine : public QStateMachine
{
public:
    using QStateMachine::QStateMachine;

    static QList<EventTransition*> findAllTransitions(QAbstractState* state, Event::Type eventType);
    static EventTransition* findTransition(QAbstractState* state, Event::Type eventType);

    QList<EventTransition*> findAllTransitions(Event::Type eventType);
    EventTransition* findTransition(Event::Type eventType);

    template <typename EventT, typename CallableT>
    bool onTransition(CallableT slot)
    {
        auto trn = findTransition(event_type_info<EventT>::id);
        if (!trn)
            return false;

        connect(trn, &EventTransition::triggered,
                [slot](Event* event)
                {
                    auto e = event->to<EventT>();
                    if (e)
                        std::invoke(slot, e);
                });

        return true;
    }

    template <typename EventT, typename RecieverT, typename CallableT,
              std::enable_if_t<std::is_member_function_pointer_v<CallableT>, bool> = true>
    bool onTransition(RecieverT* reciever, CallableT slot)
    {
        auto trn = findTransition(event_type_info<EventT>::id);
        if (!trn)
            return false;

        connect(trn, &EventTransition::triggered, reciever,
                [reciever, slot](Event* event)
                {
                    auto e = event->to<EventT>();
                    if (e)
                        std::invoke(slot, reciever, e);
                });

        return true;
    }

    template <typename EventT, typename RecieverT, typename CallableT,
              std::enable_if_t<!std::is_member_function_pointer_v<CallableT>, bool> = true>
    bool onTransition(RecieverT* reciever, CallableT slot)
    {
        auto trn = findTransition(event_type_info<EventT>::id);
        if (!trn)
            return false;

        connect(trn, &EventTransition::triggered, reciever,
                [slot](Event* event)
                {
                    auto e = event->to<EventT>();
                    if (e)
                        std::invoke(slot, e);
                });

        return true;
    }
};
