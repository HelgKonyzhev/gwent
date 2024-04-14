#include "doorstep_state.h"
#include <common/events/login_event.h>
#include <common/events/register_event.h>
#include <common/states/state.h>
#include <common/event_transition.h>

DoorstepState::DoorstepState(Socket *socket, QState *parent)
    : State{parent}
    , m_socket{socket}
{
    qRegisterMetaType<DoorstepState>();

    setObjectName("thresholdState");

    auto initialState = new State{this};
    initialState->setObjectName("initialState");

    setInitialState(initialState);

    auto registrationState = new State{this};
    registrationState->setObjectName("registrationState");

    auto loggingInState = new State{this};
    loggingInState->setObjectName("loggingInState");

    addTransition(new EventTransition{m_socket, Event::Register, registrationState});
    addTransition(new EventTransition{m_socket, Event::LogIn, loggingInState});

    registrationState->addTransition(new EventTransition{m_socket, Event::RegistrationFailed, initialState});
    registrationState->addTransition(new EventTransition{m_socket, Event::Registered, initialState});

    loggingInState->addTransition(new EventTransition{m_socket, Event::LogInFailed, initialState});
}
