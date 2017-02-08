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
    initializeAll();
    Refresh_GFX();

    while(continue_game){
        inputHandler();
        actionHandler();
        UpdateSpriteInfo(game_board, game_info);
    }

    endAll();


    return 0;
}

////////////////////////////////////////////////////////////////////////////
void initializeAll(){
    game_info.level = 1;
    game_info.sub_level = 2;
    game_info.visible_horizontal_squares = 16;
    game_info.visible_vertical_squares = 12;
    game_info.visible_upper_left_x = 16;
    game_info.visible_upper_left_y = 0;

    CreateTablero(&game_board, 24, 32);

    FILE *file_map, *file_piece, *file_event;

    if ((file_map = fopen("data/maps/1.maps", "r")) == NULL)
        exit(2);

    if ((file_piece = fopen("data/pieces/1.pieces", "r")) == NULL)
        exit(3);

    if ((file_event = fopen("data/events/1.events", "r")) == NULL)
        exit(4);

    ReadTableroFile(file_map, file_piece, &game_board);
    ReadEventListFile(file_event, &game_events);

    InitGFX();
    SetGrill(SCREEN_MAIN, game_info.visible_vertical_squares,
        game_info.visible_horizontal_squares);
    print_background(SCREEN_SECONDARY, game_info.level, 1);
    print_background(SCREEN_MAIN, game_info.sub_level, 1);
    UpdateSpriteInfo(game_board, game_info);
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
                //event and jump fall through the switch

            //speaker is a event in which you can not occupy the position
            case speaker:
                event_position.id = 0;
                    //cleans the bits that will not be initialized in next lines
                event_position.position.column = x_destiny;
                event_position.position.row = y_destiny;
                event_position.position.priority = 1;
                StartIdEventList(&game_events, event_position, &game_board, &game_info);
                break;

        }
    }
}
////////////////////////////////////////////////////////////////////////////
void endAll(){
    DestroyTablero(&game_board);
    DestroyEventList(&game_events);
    Remove_All_Sprites();
    Remove_All_Backgrounds(SCREEN_MAIN);
    Remove_All_Backgrounds(SCREEN_SECONDARY);
    ShutGFX();
}













