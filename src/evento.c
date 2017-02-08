/*
    Module: evento.c
    Author: Miguel Escribano Perez (miguescri)
    Creation date: 24 December 2016
    Last modification: 7 February 2017

*/

#include "evento.h"

bool SearchEventId(Event_list *eventlist, union id_event search_id, Event_list **event){
    bool found = FALSE;
    Event_list *aux = eventlist;

    //If the priority is 0, it looks for the event with lower priority in that
    //position that can be run
    if (search_id.position.priority == 0) {
        union  id_event lower_limit = search_id;
        search_id.position.priority = -1;
            //this way search_id.id is the highest possible for that position
        while (aux != NULL && aux->id_eventlist.id < search_id.id && !found) {
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
    Pieza aux;

    //esto se puede hacer de forma mucho más eficiente guardando la info de
    //sprites existentes (posible función para gfx_interface) y modificando aquellos
    //que hayan cambiado, en vez de borrar todo y empezar de 0
    Remove_All_Sprites();
    InitPieceIterator(&board);
    while (IteratePiece(&board, &aux) != -1) {
        uint16_t first_seen_column = info.visible_upper_left_x,
            first_seen_row  = info.visible_upper_left_y,
            last_seen_column = info.visible_upper_left_x + info.visible_horizontal_squares,
            last_seen_row   = info.visible_upper_left_x + info.visible_vertical_squares,
            x_piece         = ReadXAxisPieza(aux),
            y_piece         = ReadYAxisPieza(aux);

        if (ReadStatusPieza(aux) != PIECE_OUT_OF_BOARD &&
            x_piece >= first_seen_column &&
            x_piece < last_seen_column &&
            y_piece >= first_seen_row &&
            y_piece < last_seen_row) {
            Create_Sprite(ReadIdPieza(aux), ReadTypePieza(aux),
                ReadDirectionPieza(aux), x_piece - first_seen_column,
                y_piece - first_seen_row, TRUE);
                //el gestor de sprites necesita la posición relativa
                //de la pieza dentro de la ventana visible
        }
    }

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

void StartEvent(Evento *event, Tablero *board, Event_list **eventlist, struct level_info *info){
    Evento *aux = event, *next;
    Event_list *aux_list;
    FILE *file_board, *file_pieces, *file_event;
    char *board_sufix = "x.maps",
        *piece_sufix = "x.pieces",
        *event_sufix = "x.events";

    while (aux != NULL) {
        next = aux->chain;

        switch (aux->type) {
            case MOVE_FROM_TO:
                MovePiecePositionTablero(board, aux->params.MOVE_FROM_TO.x_init,
                    aux->params.MOVE_FROM_TO.y_init, aux->params.MOVE_FROM_TO.x, aux->params.MOVE_FROM_TO.y);
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
                SearchEventId(*eventlist, aux->params.START_EVENT.id, &aux_list);
                if (aux_list != NULL)
                    next = aux_list->event;
                break;

            case ACTIVATE_EVENT:
                ActivateIdEventList(*eventlist, aux->params.ACTIVATE_EVENT.id);
                break;

            case DEACTIVATE_EVENT:
                DeactivateIdEventList(*eventlist, aux->params.DEACTIVATE_EVENT.id);
                break;

            case CHANGE_TYPE_SQUARE:
                ChangeTypeSquareTablero(board, aux->params.CHANGE_TYPE_SQUARE.x,
                    aux->params.CHANGE_TYPE_SQUARE.y, aux->params.CHANGE_TYPE_SQUARE.type);
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
                DirectionPiecePositionTablero(board, aux->params.CHANGE_DIRECTION_FROM.x,
                    aux->params.CHANGE_DIRECTION_FROM.y, aux->params.CHANGE_DIRECTION_FROM.direction);
                break;

            case CHANGE_DIRECTION_ID:
                DirectionPieceIdTablero(board, aux->params.CHANGE_DIRECTION_ID.id,
                    aux->params.CHANGE_DIRECTION_ID.direction);
                break;

            case REFRESH_GRAPHICS:
                UpdateSpriteInfo(*board, *info);
                break;

            case CHANGE_LEVEL:
                DestroyTablero(board);
                DestroyEventList(eventlist);

                //esta solución no es escalable a más de 10 niveles
                board_sufix[0] = aux->params.CHANGE_LEVEL.level + '0';
                file_board = fopen(board_sufix, "r");
                piece_sufix[0] = aux->params.CHANGE_LEVEL.level + '0';
                file_pieces = fopen(piece_sufix, "r");
                event_sufix[0] = aux->params.CHANGE_LEVEL.level + '0';
                file_event = fopen(event_sufix, "r");

                ReadTableroFile(file_board, file_pieces, board);
                ReadEventListFile(file_event, eventlist);

                fclose(file_board);
                fclose(file_pieces);
                fclose(file_event);

                info->level = aux->params.CHANGE_LEVEL.level;
                info->sub_level = aux->params.CHANGE_LEVEL.sublevel;
                info->visible_upper_left_x = aux->params.CHANGE_LEVEL.visible_upper_left_x;
                info->visible_upper_left_y = aux->params.CHANGE_LEVEL.visible_upper_left_y;

                print_background(SCREEN_SECONDARY, aux->params.CHANGE_LEVEL.level, 1);
                print_background(SCREEN_MAIN, aux->params.CHANGE_LEVEL.sublevel, 1);
                UpdateSpriteInfo(*board, *info);
                break;

            case CHANGE_SUBLEVEL:
                info->sub_level = aux->params.CHANGE_SUBLEVEL.sublevel;
                info->visible_upper_left_x = aux->params.CHANGE_SUBLEVEL.visible_upper_left_x;
                info->visible_upper_left_y = aux->params.CHANGE_SUBLEVEL.visible_upper_left_y;
                print_background(SCREEN_MAIN, aux->params.CHANGE_SUBLEVEL.sublevel, 1);
                UpdateSpriteInfo(*board,*info);
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

int StartIdEventList(Event_list **eventlist, union id_event identificator, Tablero *board, struct level_info *info){
    Event_list *aux_list = NULL;
    SearchEventId(*eventlist, identificator, &aux_list);
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
        fprintf(file, "\n>");
        WriteEventFile(file, *aux);
        aux = aux->chain;
    }
    fprintf(file, "\n*");

    return 0;
}

int WriteEventListFile(FILE *file, Event_list eventlist){
    Event_list *aux = &eventlist;

    while (aux != NULL) {
        fprintf(file, "\n###EVENT LIST###\n#column, row and priority for the event chain to happen\n%"
                SCNu16 " %" SCNu16 " %" SCNu16, aux->id_eventlist.position.column,
                aux->id_eventlist.position.row, aux->id_eventlist.position.priority);
        fprintf(file, "\n#Chain activated (1 = activated, 0 = non-activated)\n%d", aux->activated);
        fprintf(file, "\n#List of chainned events:\n");

        WriteChainEventFile(file, *aux->event);

        fprintf(file, "\n#End of chain");

        aux = aux->next;
    }

    return 0;
}

int ReadEventFile(FILE *file, Evento **event){
    union event_params param;
    enum events type;
    int auxnum;

    fscanf(file, "%u", &type);

    switch (type) {
        case MOVE_FROM_TO:
            fscanf(file, "%" SCNu16 " %" SCNu16 " %" SCNu16 " %" SCNu16,
                &param.MOVE_FROM_TO.x_init, &param.MOVE_FROM_TO.y_init,
                &param.MOVE_FROM_TO.x, &param.MOVE_FROM_TO.y);
            break;

        case UNSET_ID:
            fscanf(file, "%" SCNu32, &param.UNSET_ID.id);
            break;

        case SET_ID:
            fscanf(file, "%" SCNu32, &param.SET_ID.id);
            break;

        case START_EVENT:
            param.START_EVENT.id.id = 0;
            fscanf(file, "%" SCNu16 " %" SCNu16 " %" SCNu16,
                &param.START_EVENT.id.position.column, &param.START_EVENT.id.position.row,
                &param.START_EVENT.id.position.priority);
            break;

        case SHOW_TEXT:
            fscanf(file, "%s", param.SHOW_TEXT.text);
            break;

        case HIDE_TEXT:
            //no params
            break;

        case WAIT_TIME:
            param.WAIT_TIME.time.tv_nsec = 0;
            fscanf(file, "%d", &auxnum);
            param.WAIT_TIME.time.tv_sec = auxnum;
            break;

        case WAIT_ACCEPT:
            //no params
            break;

        case REFRESH_GRAPHICS:
            //no params
            break;

        case CHANGE_LEVEL:
            fscanf(file, "%" SCNu16, &param.CHANGE_LEVEL.level);
            fscanf(file, "%" SCNu16, &param.CHANGE_LEVEL.sublevel);
            fscanf(file, "%" SCNu16, &param.CHANGE_LEVEL.visible_upper_left_x);
            fscanf(file, "%" SCNu16, &param.CHANGE_LEVEL.visible_upper_left_y);
            break;

        case CHANGE_SUBLEVEL:
            fscanf(file, "%" SCNu16, &param.CHANGE_SUBLEVEL.sublevel);
            fscanf(file, "%" SCNu16, &param.CHANGE_SUBLEVEL.visible_upper_left_x);
            fscanf(file, "%" SCNu16, &param.CHANGE_SUBLEVEL.visible_upper_left_y);
            break;

        default:
            break;
    }

    *event = CreateEvent(type, param);

    return 0;
}

int ReadChainEventFile(FILE *file, Evento **event){
    Evento *aux = NULL;
    char control;

    if (!feof(file)) {
        ReadEventFile(file, event);
        fscanf(file, "\n%c", &control);
    }

    while (control == '>' && !feof(file)) {
        ReadEventFile(file, &aux);
        ChainEvent(*event, aux);
        fscanf(file, "\n%c", &control);
    }

    return 0;
}

int ReadEventListFile(FILE *file, Event_list **eventlist){
    Event_list *aux = NULL;
    Evento *eve = NULL;
    union id_event id; id.id = 0;
        //si no se inicializan los 64 bits, queda basura en los de más
        //peso imposibilitando la búsqueda por id
    bool activated;

    FILE *tmp = fopen("aux.txt", "w+");
    comment_eraser(file, tmp);
    fseek(tmp, 0, SEEK_SET);

    if (!feof(tmp)) {
        fscanf(tmp, "%" SCNu16 " %" SCNu16 " %" SCNu16, &id.position.column,
               &id.position.row, &id.position.priority );
        fscanf(tmp, "%u", &activated);

        ReadChainEventFile(tmp, &eve);
        *eventlist = CreateEventList(id, activated, eve);
    }

    while (!feof(tmp)) {
        fscanf(tmp, "%" SCNu16 " %" SCNu16 " %" SCNu16, &id.position.column,
               &id.position.row, &id.position.priority );
        fscanf(tmp, "%u", &activated);

        ReadChainEventFile(tmp, &eve);
        aux = CreateEventList(id, activated, eve);
        AddEventList(eventlist, aux);
    }

    fclose(tmp);
        //hay que encontrar una forma de eliminar el fichero temporal

    return 0;
}
