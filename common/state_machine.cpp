#include "state_machine.h"
#include <QFinalState>
#include <common/state.h>
#include <common/final_state.h>

StateMachine::StateMachine(QObject* parent)
    : QStateMachine{parent}
    , m_initialState{new State{this}}
    , m_finalState{new FinalState{this}}
{
    m_initialState->setObjectName("initialState");
    m_finalState->setObjectName("finalState");
    setInitialState(m_initialState);
}

void StateMachine::onEntry(QEvent* event)
{
    qDebug() << __func__ << objectName();
    QStateMachine::onEntry(event);
}

void StateMachine::onExit(QEvent* event)
{
    qDebug() << __func__ << objectName();
    QStateMachine::onExit(event);
}
