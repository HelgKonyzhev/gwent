#pragma once
#include <QStateMachine>

class FinalState;
class State;

class StateMachine : public QStateMachine
{
public:
    StateMachine(QObject* parent);

    State* initialState() const { return m_initialState; }
    FinalState* finalState() const { return m_finalState; }

private:
    void onEntry(QEvent* event) override;
    void onExit(QEvent* event) override;

    State* m_initialState;
    FinalState* m_finalState;
};
