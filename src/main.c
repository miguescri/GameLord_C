#include <stdio.h>
#include <stdlib.h>
#include "tablero.h"
#include "evento.h"
#include "gfx_interface.h"
#include "input_interface.h"
#include "comment_eraser.h"

#ifndef BOOLEAN
#define BOOLEAN
typedef enum {FALSE = 0,
            TRUE}bool;
#endif // BOOLEAN

enum characters{main_character,
        secondary_character,
        special_item
};

enum floor_type{normal = (int)' ',
        wall = (int)'m',
        speaker = (int)'A',
        event = (int)'a',
        jump = (int)'o',
        water = (int)'~'
};


void initializeAll();
void inputHandler();
void actionHandler();
void endAll();

Tablero game_board;
struct level_info game_info;
Event_list *game_events;
int x_offset, y_offset;
bool continue_game = TRUE;

//////MAIN//////////////////////////////////////////////////////////////////
int main(){
 /*   initializeAll();
    Refresh_GFX();

    while(continue_game){
        inputHandler();
        actionHandler();
        UpdateSpriteInfo(game_board, game_info);
    }

    endAll();
*/
Pieza pie;
FILE *fi2 = fopen("pieza2.piece", "r");
ReadPiezaFile(fi2, &pie);
fclose(fi2);

FILE *fi = fopen("pieza.piece", "w");
WritePiezaFile(fi, pie);
fclose(fi);
    return 0;
}

////////////////////////////////////////////////////////////////////////////
void initializeAll(){
    CreateTablero(&game_board, 24, 32);

    FILE *file;

    if ((file = fopen("/home/miguescri/Documentos/Codigos_CB_c/1.map", "r")) == NULL)
        exit(1);

    ReadBoardFile(file, game_board.filas, game_board.columnas, game_board.conjunto_casillas);

    Pieza p = MakePieza(30, 10, 6, "xXx", 1, PIECE_ON_BOARD, main_character, 1);
    AddPieceTablero(&game_board, p);
    SetPieceIdTablero(&game_board, main_character);

    union event_params params;
    params.MOVE_FROM_TO.x_init = 16;
    params.MOVE_FROM_TO.y_init = 9;
    params.MOVE_FROM_TO.x = 10;
    params.MOVE_FROM_TO.y = 14;

    Evento *aux_event = CreateEvent(MOVE_FROM_TO, params);
    params.WAIT_TIME.time.tv_nsec = 0;
    params.WAIT_TIME.time.tv_sec = 2;
    ChainEvent(aux_event, CreateEvent(WAIT_TIME,params));
    params.MOVE_FROM_TO.x = 16;
    params.MOVE_FROM_TO.y = 9;
    params.MOVE_FROM_TO.x_init = 10;
    params.MOVE_FROM_TO.y_init = 14;
    ChainEvent(aux_event, CreateEvent(MOVE_FROM_TO,params));

    union id_event id;
    id.position.column = 16;
    id.position.row = 9;
    id.position.priority = 1;
    game_events = CreateEventList(id, TRUE, aux_event);

}

////////////////////////////////////////////////////////////////////////////
void inputHandler(){
    x_offset = 0;
    y_offset = 0;

    switch(ReadLastInteraction()){
        case A_BUTTON:
            SetPieceIdTablero(&game_board, main_character);
            break;

        case B_BUTTON:
            UnsetPieceIdTablero(&game_board, main_character);
            break;

        case UP_BUTTON:
            x_offset = 0;
            y_offset = -1;
            break;

        case LEFT_BUTTON:
            x_offset = -1;
            y_offset = 0;
            break;

        case DOWN_BUTTON:
            x_offset = 0;
            y_offset = 1;
            break;

        case RIGHT_BUTTON:
            x_offset = 1;
            y_offset = 0;
            break;

        default:
            continue_game = FALSE;
    }
}

////////////////////////////////////////////////////////////////////////////
void actionHandler(){
    Pieza aux;
    uint8_t type;
    union id_event event_position;

    if (x_offset != 0) {
        if (x_offset == 1) {
            DirectionPieceIdTablero(&game_board, main_character, SPRITE_RIGHT);
        } else {
            DirectionPieceIdTablero(&game_board, main_character, SPRITE_LEFT);
        }
    } else if (y_offset != 0) {
        if (y_offset == 1) {
            DirectionPieceIdTablero(&game_board, main_character, SPRITE_DOWN);
        } else {
            DirectionPieceIdTablero(&game_board, main_character, SPRITE_UP);
        }
    }

    ReadPieceIdTablero(game_board, main_character, &aux);

    uint16_t x_destiny = ReadXAxisPieza(aux) + x_offset, y_destiny = ReadYAxisPieza(aux) + y_offset;

    if (ReadTypeSquareTablero(game_board, x_destiny, y_destiny, &type) == 0) {
        switch (type) {
            default:
            case normal:
                MovePieceIdTablero(&game_board, main_character, x_destiny, y_destiny);
                break;

            case event:
            case jump:
                MovePieceIdTablero(&game_board, main_character, x_destiny, y_destiny);
            //speaker is a event in which you can not occupy the position
            case speaker:
                event_position.position.column = x_destiny;
                event_position.position.row = y_destiny;
                event_position.position.priority = 0;
                StartIdEventList(game_events, event_position, &game_board, &game_info);
                break;

        }
    }
}
////////////////////////////////////////////////////////////////////////////
void endAll(){
    DestroyTablero(&game_board);
    DestroyEventList(&game_events);
}













