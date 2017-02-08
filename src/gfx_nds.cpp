#include "gfx_interface.h"

#include <nds.h>
#include <maxmod9.h>
#include <assert.h>
#include "sprites.h"

void initVideo();
void initBackgrounds();

int InitGFX(){
    /*  Turn on the 2D graphics core. */
    powerOn(POWER_ALL_2D);

    /*
     *  Configure the VRAM and background control registers.
     *
     *  Place the main screen on the bottom physical screen. Then arrange the
     *  VRAM banks. Next, confiure the background control registers.
     */
    lcdMainOnBottom();
    initVideo();
    initBackgrounds();
    
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

void initVideo() {
    /*
     *  Map VRAM to display a background on the main and sub screens.
     *
     *  The vramSetMainBanks function takes four arguments, one for each of the
     *  major VRAM banks. We can use it as shorthand for assigning values to
     *  each of the VRAM bank's control registers.
     *
     *  We map banks A and B to main screen  background memory. This gives us
     *  256KB, which is a healthy amount for 16-bit graphics.
     *
     *  We map bank C to sub screen background memory.
     *
     *  We map bank D to LCD. This setting is generally used for when we aren't
     *  using a particular bank.
     *
     *  We map bank E to main screen sprite memory (aka object memory).
     */
    vramSetMainBanks(VRAM_A_MAIN_BG_0x06000000,
                     VRAM_B_MAIN_BG_0x06020000,
                     VRAM_C_SUB_BG_0x06200000,
                     VRAM_D_LCD);

    vramSetBankE(VRAM_E_MAIN_SPRITE);

    /*  Set the video mode on the main screen. */
    videoSetMode(MODE_5_2D | // Set the graphics mode to Mode 5
                 DISPLAY_BG2_ACTIVE | // Enable BG2 for display
                 DISPLAY_BG3_ACTIVE | // Enable BG3 for display
                 DISPLAY_SPR_ACTIVE | // Enable sprites for display
                 DISPLAY_SPR_1D       // Enable 1D tiled sprites
                 );

    /*  Set the video mode on the sub screen. */
    videoSetModeSub(MODE_5_2D | // Set the graphics mode to Mode 5
                    DISPLAY_BG3_ACTIVE); // Enable BG3 for display
}

void initBackgrounds() {
    /*  Set up affine background 3 on main as a 16-bit color background. */
    REG_BG3CNT = BG_BMP16_256x256 |
                 BG_BMP_BASE(0) | // The starting place in memory
                 BG_PRIORITY(3); // A low priority

    /*  Set the affine transformation matrix for the main screen background 3
     *  to be the identity matrix.
     */
    REG_BG3PA = 1 << 8;
    REG_BG3PB = 0;
    REG_BG3PC = 0;
    REG_BG3PD = 1 << 8;

    /*  Place main screen background 3 at the origin (upper left of the
     *  screen).
     */
    REG_BG3X = 0;
    REG_BG3Y = 0;

    /*  Set up affine background 2 on main as a 16-bit color background. */
    REG_BG2CNT = BG_BMP16_128x128 |
                 BG_BMP_BASE(8) | // The starting place in memory
                 BG_PRIORITY(2);  // A higher priority

    /*  Set the affine transformation matrix for the main screen background 3
     *  to be the identity matrix.
     */
    REG_BG2PA = 1 << 8;
    REG_BG2PB = 0;
    REG_BG2PC = 0;
    REG_BG2PD = 1 << 8;

    /*  Place main screen background 2 in an interesting place. */
    REG_BG2X = -(SCREEN_WIDTH / 2 - 32) << 8;
    REG_BG2Y = -32 << 8;

    /*  Set up affine background 3 on the sub screen as a 16-bit color
     *  background.
     */
    REG_BG3CNT_SUB = BG_BMP16_256x256 |
                     BG_BMP_BASE(0) | // The starting place in memory
                     BG_PRIORITY(3); // A low priority

    /*  Set the affine transformation matrix for the sub screen background 3
     *  to be the identity matrix.
     */
    REG_BG3PA_SUB = 1 << 8;
    REG_BG3PB_SUB = 0;
    REG_BG3PC_SUB = 0;
    REG_BG3PD_SUB = 1 << 8;

    /*
     *  Place main screen background 3 at the origin (upper left of the screen)
     */
    REG_BG3X_SUB = 0;
    REG_BG3Y_SUB = 0;
}
