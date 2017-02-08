#ifndef GFX_UNIFIER_H
#define GFX_UNIFIER_H

////LEVELS/////////////////////////////////////////////////////////////////////
const int gfx_num_levels = 2;
unsigned int *gfx_levels[gfx_num_levels];
unsigned int gfx_levelsBitmapLen[gfx_num_levels];

#include "level1.h"
#include "level2.h"
int gfx_loadlevels(){
    gfx_levels[0] = (unsigned int *)level1Bitmap;
    gfx_levelsBitmapLen[0] = level1BitmapLen;


    gfx_levels[1] = (unsigned int *)level2Bitmap;
    gfx_levelsBitmapLen[1] = level2BitmapLen;
}

//SUBLEVELS////////////////////////////////////////////////////////////////////
const int gfx_num_sublevels = 2;
unsigned int *gfx_sublevels[gfx_num_sublevels];
unsigned int gfx_sublevelsBitmapLen[gfx_num_sublevels];

#include "sublevel1.h"
#include "sublevel2.h"
int gfx_loadsublevels(){
    gfx_sublevels[0] = (unsigned int *)sublevel1Bitmap;
    gfx_sublevelsBitmapLen[0] = sublevel1BitmapLen;


    gfx_sublevels[1] = (unsigned int *)sublevel2Bitmap;
    gfx_sublevelsBitmapLen[1] = sublevel2BitmapLen;
}

////PIECES/////////////////////////////////////////////////////////////////////
const int gfx_num_pieces = 2;
unsigned int *gfx_piecesTiles[gfx_num_pieces];
unsigned int gfx_piecesTilesLen[gfx_num_pieces];
unsigned short *gfx_piecesPal[gfx_num_pieces];
unsigned int gfx_piecesPalLen[gfx_num_pieces];

#include "piece1.h"
#include "piece2.h"
int gfx_loadpieces(){
    gfx_piecesTiles[0] = (unsigned int *)piece1Tiles;
    gfx_piecesTilesLen[0] = piece1TilesLen;
    gfx_piecesPal[0] = (unsigned short *)piece1Pal;
    gfx_piecesPalLen[0] = piece1PalLen;


    gfx_piecesTiles[1] = (unsigned int *)piece2Tiles;
    gfx_piecesTilesLen[1] = piece2TilesLen;
    gfx_piecesPal[1] = (unsigned short *)piece2Pal;
    gfx_piecesPalLen[1] = piece2PalLen;
}


int gfx_load(){
    gfx_loadlevels();
    gfx_loadsublevels();
    gfx_loadpieces();
}
#endif
