#ifndef GFX_INTERFACE_H_INCLUDED
#define GFX_INTERFACE_H_INCLUDED

#include "tablero.h"

#ifndef BOOLEAN
#define BOOLEAN
typedef enum {FALSE = 0,
            TRUE}bool;
#endif // BOOLEAN

enum screen{SCREEN_MAIN = 0,
            SCREEN_SECONDARY
};

enum sprite_orientation{SPRITE_UP = 0,
                        SPRITE_DOWN,
                        SPRITE_RIGHT,
                        SPRITE_LEFT
};

int InitGFX();

int ShutGFX();

int Refresh_GFX();

int SetGrill(enum screen where, uint16_t rows, uint16_t columns);

int ReadGrill(enum screen where, uint16_t *rows, uint16_t *columns);

int print_background(enum screen where, int background_id, uint8_t depth);

int Remove_All_Backgrounds(enum screen where);

int print_text(enum screen where, char text[], int size_buff);

int Create_Sprite(uint32_t sprite_id, int sprite_type, enum sprite_orientation orientation, uint16_t x, uint16_t y, bool visible);

int Move_Sprite(uint32_t sprite_id, uint16_t x, uint16_t y);

int Show_Sprite(uint32_t sprite_id);

int Hide_Sprite(uint32_t sprite_id);

int Remove_Sprite(uint32_t sprite_id);

int Remove_All_Sprites();

#endif // GFX_INTERFACE_H_INCLUDED
