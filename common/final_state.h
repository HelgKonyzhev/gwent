#pragma once
#include <QFinalState>

class Event;

class FinalState : public QFinalState
{
    Q_OBJECT

public:
    using QFinalState::QFinalState;

signals:
    void entered(Event *);
    void exited(Event *);

private:
    void onEntry(QEvent *event) override;
    void onExit(QEvent *event) override;
};
