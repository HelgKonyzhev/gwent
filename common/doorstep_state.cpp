#include "doorstep_state.h"
#include <common/events/login_event.h>
#include <common/events/register_event.h>
#include <common/state.h>
#include <common/event_transition.h>
#include <common/final_state.h>

DoorstepState::DoorstepState(Socket *socket, QState *parent)
    : State{parent}
    , m_socket{socket}
{
    setObjectName("thresholdState");

    auto initialState = new State{this};
    initialState->setObjectName("initialState");

    auto finalState = new FinalState{this};
    finalState->setObjectName("finalState");

    setInitialState(initialState);

    connect(initialState, &State::entered, this,
            [this](Event *event)
            {
                switch (static_cast<Event::Type>(event->type()))
                {
                    case Event::Registered:
                        emit registered(event->to<RegisteredEvent>());
                        break;
                    case Event::RegistrationFailed:
                        emit registrationFailed(event->to<RegistrationFailedEvent>());
                        break;
                    case Event::LogInFailed:
                        emit loginFailed(event->to<LogInFailedEvent>());
                        break;
                    default:
                        qDebug() << "unexpected event" << event->type();
                        break;
                }
            });

    //    connect(finalState, &FinalState::entered, this,
    //            [this](Event *event) { emit loggedIn(event->to<LoggedInEvent>()); });

    auto registrationState = new State{this};
    registrationState->setObjectName("registrationState");

    connect(registrationState, &State::entered, this,
            [this](Event *event) { emit registration(event->to<RegisterEvent>()); });

    auto loggingInState = new State{this};
    loggingInState->setObjectName("loggingInState");

    connect(loggingInState, &State::entered, this,
            [this](Event *event) { emit logging(event->to<LogInEvent>()); });

    addTransition(new EventTransition{m_socket, Event::Register, registrationState});
    addTransition(new EventTransition{m_socket, Event::LogIn, loggingInState});

    registrationState->addTransition(new EventTransition{m_socket, Event::RegistrationFailed, initialState});
    registrationState->addTransition(new EventTransition{m_socket, Event::Registered, initialState});

    //    loggingInState->addTransition(new EventTransition{m_socket, Event::LoggedIn, finalState});
    loggingInState->addTransition(new EventTransition{m_socket, Event::LogInFailed, initialState});
}
