#include "gfx_interface.h"
#include "gfx_unifier.h"

#include <nds.h>
#include <maxmod9.h>
#include <assert.h>
#include "sprites.h"

const int num_sprites = 128;
SpriteInfo spriteInfo[num_sprites];
OAMTable *oam;
static const int DMA_CHANNEL = 3;

int ncolumns,
    nrows,
    square_width,
    square_height;
    
const int SCREEN_WIDTH = 256,
        SCREEN_HEIGHT = 192;

void initVideo();
void initBackgrounds();
void initSprites();

int InitGFX(){
    gfx_load();
        //loads all the data generated in .h files by grit
    
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
    
    oam = new OAMTable();
    initOAM(oam);
    
    return 0;
}

int ShutGFX(){
    return 0;
}

int Refresh_GFX(){
    swiWaitForVBlank();
    updateOAM(oam);
    
    return 0;
}

//this should distinguish between screens when grilling
int SetGrill(enum screen where, uint16_t rows, uint16_t columns){
    ncolumns = columns;
    nrows = rows;
    square_width = SCREEN_WIDTH/columns;
    square_height = SCREEN_HEIGHT/rows;
    
    return 0;
}

int ReadGrill(enum screen where, uint16_t *rows, uint16_t *columns){
    *rows = nrows;
    *columns = ncolumns;
    
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

void initSprites() {
    /*  Define some sprite configuration specific constants.
     *
     *  We will use these to compute the proper index into memory for certain
     *  tiles or palettes.
     *
     *  OFFSET_MULTIPLIER is calculated based on the following formula from
     *  GBATEK (http://nocash.emubase.de/gbatek.htm#dsvideoobjs):
     *      TileVramAddress = TileNumber * BoundaryValue
     *  Since SPRITE_GFX is a uint16*, the compiler will increment the address
     *  it points to by 2 for each change in 1 of the array index into
     *  SPRITE_GFX. (The compiler does pointer arithmetic.)
     */
    static const int BYTES_PER_16_COLOR_TILE = 32;
    static const int COLORS_PER_PALETTE = 16;
    static const int BOUNDARY_VALUE = 32; /* This is the default boundary value
                                           * (can be set in REG_DISPCNT) */
    static const int OFFSET_MULTIPLIER = BOUNDARY_VALUE /
                                         sizeof(SPRITE_GFX[0]);

    /* Keep track of the available tiles */
    int nextAvailableTileIdx = 0;

    /* Create the ship sprite. */
    static const int SHUTTLE_OAM_ID = 0;
    assert(SHUTTLE_OAM_ID < SPRITE_COUNT);
    SpriteInfo * shuttleInfo = &spriteInfo[SHUTTLE_OAM_ID];
    SpriteEntry * shuttle = &oam->oamBuffer[SHUTTLE_OAM_ID];

    /* Initialize shuttleInfo */
    shuttleInfo->oamId = SHUTTLE_OAM_ID;
    shuttleInfo->width = 16;
    shuttleInfo->height = 16;
    shuttleInfo->angle = 29568;
    shuttleInfo->entry = shuttle;

    /*
     *  Configure attribute 0.
     *
     *  OBJCOLOR_16 will make a 16-color sprite. We specify that we want an
     *  affine sprite (via isRotateScale) here because we would like to rotate
     *  the ship.
     */
    shuttle->y = SCREEN_HEIGHT / 2 - shuttleInfo->height;
    shuttle->isRotateScale = true;
    /* This assert is a check to see a matrix is available to store the affine
     * transformation matrix for this sprite. Of course, you don't have to have
     * the matrix id match the affine id, but if you do make them match, this
     * assert can be helpful. */
    assert(!shuttle->isRotateScale || (shuttleInfo->oamId < MATRIX_COUNT));
    shuttle->isSizeDouble = false;
    shuttle->blendMode = OBJMODE_NORMAL;
    shuttle->isMosaic = false;
    shuttle->colorMode = OBJCOLOR_16;
    shuttle->shape = OBJSHAPE_SQUARE;

    /*
     *  Configure attribute 1.
     *
     *  rsMatrixId refers to the loation of affine transformation matrix. We
     *  set it to a location computed with a macro. OBJSIZE_64, in our case
     *  since we are making a square sprite, creates a 64x64 sprite.
     */
    shuttle->x = SCREEN_WIDTH / 2 - shuttleInfo->width * 2 +
                    shuttleInfo->width / 2;
    shuttle->rotationIndex = shuttleInfo->oamId;
    shuttle->size = OBJSIZE_64;

    /*
     *  Configure attribute 2.
     *
     *  Configure which tiles the sprite will use, which priority layer it will
     *  be placed onto, which palette the sprite should use, and whether or not
     *  to show the sprite.
     */
    shuttle->gfxIndex = nextAvailableTileIdx;
    nextAvailableTileIdx += orangeShuttleTilesLen / BYTES_PER_16_COLOR_TILE;
    shuttle->priority = OBJPRIORITY_0;
    shuttle->palette = shuttleInfo->oamId;

    /* Rotate the sprite */
    rotateSprite(&oam->matrixBuffer[shuttleInfo->oamId],
                 shuttleInfo->angle);

    /*************************************************************************/

    /* Create the moon sprite. */
    static const int MOON_OAM_ID = 1;
    assert(MOON_OAM_ID < SPRITE_COUNT);
    SpriteInfo * moonInfo = &spriteInfo[MOON_OAM_ID];
    SpriteEntry * moon = &oam->oamBuffer[MOON_OAM_ID];

    /* Initialize moonInfo */
    moonInfo->oamId = MOON_OAM_ID;
    moonInfo->width = 16;
    moonInfo->height = 16;
    moonInfo->angle = 29568;
    moonInfo->entry = moon;

    /*
     *  Configure attribute 0.
     *
     *  OBJCOLOR_16 will make a 16-color sprite. We won't specify that we want
     *  an affine sprite here because we don't want one this time.
     */
    moon->y = SCREEN_WIDTH / 2 + moonInfo->height / 2;
    moon->isRotateScale = false;
    /* This assert is a check to see a matrix is available to store the affine
     * transformation matrix for this sprite. Of course, you don't have to have
     * the matrix id match the affine id, but if you do make them match, this
     * assert can be helpful. */
    assert(!moon->isRotateScale || (moonInfo->oamId < MATRIX_COUNT));
    moon->isHidden = false;
    moon->blendMode = OBJMODE_NORMAL;
    moon->isMosaic = false;
    moon->colorMode = OBJCOLOR_16;
    moon->shape = OBJSHAPE_SQUARE;

    /*
     * Configure attribute 1.
     *
     * OBJSIZE_32 will create a sprite of size 32x32, since we are making a
     * square sprite. Since we are using a non-affine sprite, attribute 1
     * doesn't have an rotationIndex anymore. Instead, it has the ability to flip
     * the sprite vertically or horizontally.
     */
    moon->x = SCREEN_WIDTH / 2 + moonInfo->width + moonInfo->width / 2;
    moon->hFlip = false;
    moon->vFlip = false;
    moon->size = OBJSIZE_32;

    /*
     *  Configure attribute 2.
     *
     *  Configure which tiles the sprite will use, which priority layer it will
     *  be placed onto, which palette the sprite should use, and whether or not
     *  to show the sprite.
     */
    moon->gfxIndex = nextAvailableTileIdx;
    nextAvailableTileIdx += moonTilesLen / BYTES_PER_16_COLOR_TILE;
    moon->priority = OBJPRIORITY_2;
    moon->palette = moonInfo->oamId;

    /*************************************************************************/

    /* Copy over the sprite palettes */
    dmaCopyHalfWords(SPRITE_DMA_CHANNEL,
                     orangeShuttlePal,
                     &SPRITE_PALETTE[shuttleInfo->oamId *
                                     COLORS_PER_PALETTE],
                     orangeShuttlePalLen);
    dmaCopyHalfWords(SPRITE_DMA_CHANNEL,
                     moonPal,
                     &SPRITE_PALETTE[moonInfo->oamId * COLORS_PER_PALETTE],
                     moonPalLen);

    /* Copy the sprite graphics to sprite graphics memory */
    dmaCopyHalfWords(SPRITE_DMA_CHANNEL,
                     orangeShuttleTiles,
                     &SPRITE_GFX[shuttle->gfxIndex * OFFSET_MULTIPLIER],
                     orangeShuttleTilesLen);
    dmaCopyHalfWords(SPRITE_DMA_CHANNEL,
                     moonTiles,
                     &SPRITE_GFX[moon->gfxIndex * OFFSET_MULTIPLIER],
                     moonTilesLen);
}
