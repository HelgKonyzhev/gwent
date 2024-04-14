#pragma once
#include <QState>

class Event;
class EventTransition;

class State : public QState
{
    Q_OBJECT

public:
    using QState::QState;

signals:
    Q_INVOKABLE void entered(QEvent*);
    Q_INVOKABLE void exited(QEvent*);

private:
    void onEntry(QEvent* event) override;
    void onExit(QEvent* event) override;
};
