/*
    Module: evento.c
    Author: Miguel Escribano Perez (miguescri)
    Creation date: 24 December 2016
    Last modification: 13 November 2016

*/

#include "evento.h"

bool SearchEventId(Event_list *eventlist, union id_event search_id, Event_list **event){
    bool found = FALSE;
    Event_list *aux = eventlist;

    //If the priority is 0, it looks for the event with lower priority in that position that can be run
    if (search_id.position.priority == 0) {
        union  id_event lower_limit = search_id;
        search_id.position.priority = -1;       //this way. search_id.id is the highest possible for that position
        while (aux != NULL && aux->id_eventlist.id <= search_id.id && !found) {
            if (aux->id_eventlist.id >= lower_limit.id && aux->activated) {
                found = TRUE;
                *event = aux;
            }
            aux = aux->next;
        }

    } else {
        while (aux != NULL && aux->id_eventlist.id <= search_id.id && !found) {
            if (aux->id_eventlist.id == search_id.id){
                found = TRUE;
                *event = aux;
            }
            aux = aux->next;
        }
    }

    return found;
}

int UpdateSpriteInfo(Tablero board, struct level_info info){
    Refresh_GFX();
    return 0;
}

Evento* CreateEvent(enum events type, union event_params param){
    Evento* aux = malloc(sizeof(Evento));
    if (aux == NULL) {
        write(STDERR_FILENO, "\nError when allocating memory for event\n", 40);
        exit(1);
    }

    aux->type = (uint8_t) type;
    aux->params = param;
    aux->chain = NULL;

    return aux;
}

void ChainEvent(Evento *main, Evento *chain){
    Evento *aux = main;

    if ( aux != NULL) {
        while (aux->chain != NULL) {
            aux = aux->chain;
        }
        aux->chain = chain;
    }
}

void DestroyEvent(Evento **event){
    Evento *aux = *event, *aux2;

    while (aux != NULL) {
        aux2 = aux->chain;
        free(aux);
        aux = aux2;
    }
    *event = NULL;
}

void StartEvent(Evento *event, Tablero *board, Event_list *eventlist, struct level_info *info){
    Evento *aux = event, *next;
    Event_list *aux_list;

    while (aux != NULL) {
        next = aux->chain;

        switch (aux->type) {
            case MOVE_FROM_TO:
                MovePiecePositionTablero(board, aux->params.MOVE_FROM_TO.x_init, aux->params.MOVE_FROM_TO.y_init, aux->params.MOVE_FROM_TO.x, aux->params.MOVE_FROM_TO.y);
                break;

            case MOVE_ID_TO:
                MovePieceIdTablero(board, aux->params.MOVE_ID_TO.piece_id, aux->params.MOVE_ID_TO.x, aux->params.MOVE_ID_TO.y);
                break;

            case UNSET_FROM:
                UnsetPiecePositionTablero(board, aux->params.UNSET_FROM.x, aux->params.UNSET_FROM.y);
                break;

            case UNSET_ID:
                UnsetPieceIdTablero(board, aux->params.UNSET_ID.id);
                break;

            case SET_ID:
                SetPieceIdTablero(board, aux->params.SET_ID.id);
                break;

            case START_EVENT:
                aux_list = NULL;
                SearchEventId(eventlist, aux->params.START_EVENT.id, &aux_list);
                if (aux_list != NULL)
                    next = aux_list->event;
                break;

            case ACTIVATE_EVENT:
                ActivateIdEventList(eventlist, aux->params.ACTIVATE_EVENT.id);
                break;

            case DEACTIVATE_EVENT:
                DeactivateIdEventList(eventlist, aux->params.DEACTIVATE_EVENT.id);
                break;

            case CHANGE_TYPE_SQUARE:
                ChangeTypeSquareTablero(board, aux->params.CHANGE_TYPE_SQUARE.x, aux->params.CHANGE_TYPE_SQUARE.y, aux->params.CHANGE_TYPE_SQUARE.type);
                break;

            case SHOW_TEXT:
                print_text(SCREEN_MAIN, aux->params.SHOW_TEXT.text, 64);
                break;

            case HIDE_TEXT:
                print_text(SCREEN_MAIN, "", 1);
                break;

            case WAIT_TIME:
                UpdateSpriteInfo(*board, *info);
                nanosleep(&aux->params.WAIT_TIME.time, NULL);
                break;

            case WAIT_ACCEPT:
                UpdateSpriteInfo(*board, *info);
                WaitInteraction(ANY_BUTTON);
                break;

            case CHANGE_DIRECTION_FROM:
                DirectionPiecePositionTablero(board, aux->params.CHANGE_DIRECTION_FROM.x, aux->params.CHANGE_DIRECTION_FROM.y, aux->params.CHANGE_DIRECTION_FROM.direction);
                break;

            case CHANGE_DIRECTION_ID:
                DirectionPieceIdTablero(board, aux->params.CHANGE_DIRECTION_ID.id, aux->params.CHANGE_DIRECTION_ID.direction);
                break;

            case REFRESH_GRAPHICS:
                UpdateSpriteInfo(*board, *info);
                break;

            case CHANGE_LEVEL:
                break;

            case CHANGE_SUBLEVEL:
                break;

            case SAVE_CHECKPOINT:
                break;

            case SAVE_LEVEL:
                break;
        }

        aux = next;
    }
}



Event_list* CreateEventList(union id_event identificator, bool activated, Evento *event){
    Event_list* aux = malloc(sizeof(Event_list));
    if (aux == NULL) {
        write(STDERR_FILENO, "\nError when allocating memory for eventlist\n", 40);
        exit(1);
    }

    aux->id_eventlist = identificator;
    aux->activated = activated;
    aux->event = event;
    aux->next = NULL;

    return aux;
}

int AddEventList(Event_list **eventlist, Event_list *new_event){
    Event_list *aux = *eventlist;

    if (new_event->id_eventlist.id == aux->id_eventlist.id)
        return FALSE;
    else if (new_event->id_eventlist.id == aux->id_eventlist.id) {
        *eventlist = new_event;
        new_event->next = aux;
        return TRUE;
    } else {
        while (aux->next != NULL && aux->next->id_eventlist.id <= new_event->id_eventlist.id)
            aux = aux->next;

        if (aux->next != NULL && aux->next->id_eventlist.id == new_event->id_eventlist.id)
            return FALSE;       //the id is already at the list

        new_event->next = aux->next;
        aux->next = new_event;

        return TRUE;
    }
}

int RemoveIdEventList(Event_list *eventlist, union id_event identificator);

void DestroyEventList(Event_list **eventlist){
    Event_list *aux = *eventlist, *aux2;

    while (aux != NULL) {
        DestroyEvent(&aux->event);
        aux2 = aux->next;
        free(aux);
        aux = aux2;
    }
    *eventlist = NULL;
}

int ActivateIdEventList(Event_list *eventlist, union id_event identificator){
    Event_list *aux_list = NULL;
    SearchEventId(eventlist, identificator, &aux_list);
    if (aux_list != NULL) {
        aux_list->activated = TRUE;
        return TRUE;
    } else {
        return FALSE;
    }
}

int DeactivateIdEventList(Event_list *eventlist, union id_event identificator){
    Event_list *aux_list = NULL;
    SearchEventId(eventlist, identificator, &aux_list);
    if (aux_list != NULL) {
        aux_list->activated = FALSE;
        return TRUE;
    } else {
        return FALSE;
    }
}

int StartIdEventList(Event_list *eventlist, union id_event identificator, Tablero *board, struct level_info *info){
    Event_list *aux_list = NULL;
    SearchEventId(eventlist, identificator, &aux_list);
    if (aux_list != NULL) {
        StartEvent(aux_list->event, board, eventlist, info);
        return TRUE;
    } else {
        return FALSE;
    }
}

int WriteEventFile(FILE *file, Evento event){
    return 0;
}

int WriteChainEventFile(FILE *file, Evento event){
    Evento *aux = event.chain;

    WriteEventFile(file, event);

    while (aux != NULL) {
        fprintf(file, "\n>>>>>");
        WriteEventFile(file, *aux);
        aux = aux->chain;
    }
    fprintf(file, "\n*****");

    return 0;
}

int WriteEventListFile(FILE *file, Event_list eventlist){
    Event_list *aux = &eventlist;

    while (aux != NULL) {
        fprintf(file, "\n###EVENT LIST###\n#column, row and priority for the event chain to happen\n%du %du %du", aux->id_eventlist.position.column, aux->id_eventlist.position.row, aux->id_eventlist.position.priority);
        fprintf(file, "\n#Chain activated (1 = activated, 0 = non-activated)\n%du", aux->activated);
        fprintf(file, "\n#List of chainned events:\n");

        WriteChainEventFile(file, *aux->event);

        fprintf(file, "\n#End of chain");

        aux = aux->next;
    }

    return 0;
}

int ReadEventFile(FILE *file, Evento *event){

    return 0;
}

int ReadChainEventFile(FILE *file, Evento *event){
    Evento *aux;
    char *text;
    int error;
    size_t n;

    ReadEventFile(file, event);

    do{
        if ( (error = getline(&text, &n, file)) == -1) {
            free(text);
            return error;
        }
    } while (text[0] != '>' && text[0] != '*');

    while (text[0] == '>') {
        if ((aux = calloc(1, sizeof(Evento))) == NULL) {
            write(STDERR_FILENO, "\nError when allocating memory for read event\n", 47);
            exit(1);
        }
        ReadEventFile(file, aux);
        ChainEvent(event, aux);

        do{
            if ( (error = getline(&text, &n, file)) == -1) {
                free(text);
                return error;
            }
        } while (text[0] != '>' && text[0] != '*');
    }

    free(text);
    return 0;
}

int ReadEventListFile(FILE *file, Event_list *eventlist){
    Event_list *aux;
    char *text;
    int error;
    size_t n;

    fscanf(file, "%u %u %u\n", &eventlist->id_eventlist.position.column, &eventlist->id_eventlist.position.row, &eventlist->id_eventlist.position.priority );
    fscanf(file, "%u\n", &eventlist->activated);

    ReadChainEventFile(file, eventlist->event);

    do{
        if ( (error = getline(&text, &n, file)) == -1) {
            free(text);
            return error;
        }
    } while (text[0] != '*');

    while (text[0] == '*') {
        if ((aux = calloc(1, sizeof(Event_list))) == NULL) {
            write(STDERR_FILENO, "\nError when allocating memory for read event\n", 47);
            exit(1);
        }
        ReadChainEventFile(file, aux->event);
        AddEventList(&eventlist, aux);

        do{
            if ( (error = getline(&text, &n, file)) == -1) {
                free(text);
                return error;
            }
        } while (text[0] != '*');
    }

    free(text);
    return 0;
}
