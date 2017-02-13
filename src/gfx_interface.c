#include "gfx_interface.h"

#include "tablero.h"
extern Tablero game_board;



int InitGFX(){
    return 0;
}

int ShutGFX(){
    return 0;
}

int Refresh_GFX(){
    ShowBoard(game_board.filas, game_board.columnas, LONGNOMBRE, game_board.conjunto_casillas);
    printf("\n");
    return 0;
}

int SetGrill(enum screen where, uint16_t rows, uint16_t columns){
    return 0;
}

int ReadGrill(enum screen where, uint16_t *rows, uint16_t *columns){
    return 0;
}

int print_background(enum screen where, int background_id, uint8_t depth){
    return 0;
}

int Remove_All_Backgrounds(enum screen where){
    return 0;
}

int print_text(enum screen where, char text[], int size_buff){
    printf(text);
    return 0;
}

int clear_text(enum screen where){
    return 0;
}

int Create_Sprite(uint32_t sprite_id, int sprite_type, enum sprite_orientation orientation, uint16_t x, uint16_t y, bool visible){
    return 0;
}

int Move_Sprite(uint32_t sprite_id, uint16_t x, uint16_t y){
    return 0;
}

int Show_Sprite(uint32_t sprite_id){
    return 0;
}

int Hide_Sprite(uint32_t sprite_id){
    return 0;
}

int Remove_Sprite(uint32_t sprite_id){
    return 0;
}

int Remove_All_Sprites(){
    return 0;
}
