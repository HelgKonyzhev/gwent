QT += core websockets statemachine

SOURCES += \
    $$PWD/cards_data/card_data.cpp \
    $$PWD/cards_data/cards_data.cpp \
    $$PWD/cards_data/cards_set.cpp \
    $$PWD/deck.cpp \
    $$PWD/doorstep_state.cpp \
    $$PWD/event_transition.cpp \
    $$PWD/events/add_deck_event.cpp \
    $$PWD/events/event.cpp \
    $$PWD/events/erase_deck_event.cpp \
    $$PWD/events/lobby_updated_event.cpp \
    $$PWD/events/login_event.cpp \
    $$PWD/events/register_event.cpp \
    $$PWD/events/error_event.cpp \
    $$PWD/events/request_game_event.cpp \
    $$PWD/events/start_game_event.cpp \
    $$PWD/events/update_deck_event.cpp \
    $$PWD/final_state.cpp \
    $$PWD/lobby_state.cpp \
    $$PWD/player.cpp \
    $$PWD/player_data.cpp \
    $$PWD/result.cpp \
    $$PWD/socket.cpp \
    $$PWD/state.cpp

HEADERS += \
    $$PWD/cards_data/card_data.h \
    $$PWD/cards_data/cards_data.h \
    $$PWD/cards_data/cards_set.h \
    $$PWD/deck.h \
    $$PWD/doorstep_state.h \
    $$PWD/event_transition.h \
    $$PWD/events/add_deck_event.h \
    $$PWD/events/event.h \
    $$PWD/events/erase_deck_event.h \
    $$PWD/events/lobby_updated_event.h \
    $$PWD/events/login_event.h \
    $$PWD/events/register_event.h \
    $$PWD/events/error_event.h \
    $$PWD/events/request_game_event.h \
    $$PWD/events/start_game_event.h \
    $$PWD/events/update_deck_event.h \
    $$PWD/final_state.h \
    $$PWD/lobby_state.h \
    $$PWD/player.h \
    $$PWD/player_data.h \
    $$PWD/socket.h \
    $$PWD/state.h \
    $$PWD/result.h

INCLUDEPATH += $$PWD
