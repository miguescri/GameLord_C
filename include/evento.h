/*
    Module: evento.h
    Author: Miguel Escribano Perez (miguescri)
    Creation date: 24 December 2016
    Last modification: 13 November 2016

*/

/*TODO:
    Add graphic refresing events
    Add file in/out
*/

#ifndef EVENTO_H
#define EVENTO_H

#include <unistd.h>
#include <time.h>

#include "tablero.h"
#include "gfx_interface.h"
#include "input_interface.h"

enum events{MOVE_FROM_TO,
            MOVE_ID_TO,
            UNSET_FROM,
            UNSET_ID,
            SET_ID,
            START_EVENT,
            ACTIVATE_EVENT,
            DEACTIVATE_EVENT,
            CHANGE_TYPE_SQUARE,
            SHOW_TEXT,
            HIDE_TEXT,
            WAIT_TIME,
            WAIT_ACCEPT,
            CHANGE_DIRECTION_FROM,
            CHANGE_DIRECTION_ID,
            REFRESH_GRAPHICS,
            CHANGE_LEVEL,
            CHANGE_SUBLEVEL,
            SAVE_CHECKPOINT,
            SAVE_LEVEL
};

#ifndef BOOLEAN
#define BOOLEAN
typedef enum {FALSE = 0,
            TRUE}bool;
#endif // BOOLEAN

union id_event{
    struct{
        uint16_t priority;
        uint16_t column;
        uint16_t row;
        uint16_t stuffing;
    }position;
    uint64_t id;
};

union event_params{
    struct{
        uint16_t x_init, y_init, x, y;
    }MOVE_FROM_TO;

    struct{
        uint32_t piece_id;
        uint16_t x, y;
    }MOVE_ID_TO;

    struct{
        uint16_t x, y;
    }UNSET_FROM;

    struct{
        uint32_t id;
    }UNSET_ID;

    struct{
        uint32_t id;
    }SET_ID;

    struct{
        union id_event id;
    }START_EVENT;

    struct{
        union id_event id;
    }ACTIVATE_EVENT;

    struct{
        union id_event id;
    }DEACTIVATE_EVENT;

    struct{
        uint16_t x, y;
        uint8_t type;
    }CHANGE_TYPE_SQUARE;

    struct{
        char text[64];
    }SHOW_TEXT;

    struct{
        struct timespec time;
    }WAIT_TIME;

    struct{
        uint16_t x, y;
        enum sprite_orientation direction;
    }CHANGE_DIRECTION_FROM;

    struct{
        uint32_t id;
        enum sprite_orientation direction;
    }CHANGE_DIRECTION_ID;
};

struct level_info{
    int level;
    int sub_level;
    uint16_t visible_horizontal_squares;
    uint16_t visible_vertical_squares;
    uint16_t visible_upper_left_x;
    uint16_t visible_upper_left_y;
};

struct evento{
    enum events type;
    union event_params params;
    struct evento *chain;
};
typedef struct evento Evento;

struct event_list{
    Evento *event;
    struct event_list *next;
    union id_event id_eventlist;
    bool activated;
};
typedef struct event_list Event_list;


int UpdateSpriteInfo(Tablero board, struct level_info info);


//Evento operations

Evento* CreateEvent(enum events type, union event_params param);

void ChainEvent(Evento *main, Evento *chain);

void DestroyEvent(Evento **event);

void StartEvent(Evento *event, Tablero *board, Event_list *eventlist, struct level_info *info);

int WriteEventFile(FILE *file, Evento event);

int WriteChainEventFile(FILE *file, Evento event);

int ReadEventFile(FILE *file, Evento **event);

int ReadChainEventFile(FILE *file, Evento **event);


//Event_list operations

Event_list* CreateEventList(union id_event identificator, bool activated, Evento *event);

int AddEventList(Event_list **eventlist, Event_list *new_event);

int RemoveIdEventList(Event_list *eventlist, union id_event identificator);

void DestroyEventList(Event_list **eventlist);

int ActivateIdEventList(Event_list *eventlist, union id_event identificator);

int DeactivateIdEventList(Event_list *eventlist, union id_event identificator);

int StartIdEventList(Event_list *eventlist, union id_event identificator, Tablero *board, struct level_info *info);

int WriteEventListFile(FILE *file, Event_list eventlist);

int ReadEventListFile(FILE *file, Event_list **eventlist);




#endif // EVENTO_H
