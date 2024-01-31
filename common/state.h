#pragma once
#include <QState>

class Event;

class State : public QState
{
    Q_OBJECT

public:
    using QState::QState;

signals:
    void entered(Event *);
    void exited(Event *);

private:
    void onEntry(QEvent *event) override;
    void onExit(QEvent *event) override;
};
