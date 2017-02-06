/*
    Module: casilla.h
    Author: Miguel Escribano Perez (miguescri)
    Creation date: 3 December 2015
    Last modification: 13 November 2016

*/

/*
    This module defines the type Casilla and the functions to interact with it.
    Casilla represents one of the squares in which is divided a board such as the one from chess, with information about
    the type of square, its name and the piece Pieza that is on it.

    As in a board game, a piece can only move around the squares if it is on game. This info is stored in the status of the piece.

    Casilla is designed to be used in groups inside a matrix to create boards. This sizes the information about X and Y positioning that contains Pieza.
*/

/*TO DO:
    -Change name to C strings both in struct and functions
    -Change the STDOUT writer functions for them to give a string as output
    -Update documentation
*/


#ifndef CASILLA_H
#define CASILLA_H

#include "pieza.h"
#include <stdio.h>

enum board_status{PIECE_OUT_OF_BOARD = 0,
    PIECE_ON_BOARD
};
enum board_error{ERROR_X0_OVER_RANGE = 10,
    ERROR_X1_OVER_RANGE,
    ERROR_Y0_OVER_RANGE,
    ERROR_Y1_OVER_RANGE,
    ERROR_PIECE_SET,
    ERROR_PIECE_UNSET,
    ERROR_SQUARE_EMPTY,
    ERROR_SQUARE_OCCUPIED,
    ERROR_STATUS_SET_BUT_SQUARE_OCCUPIED,
    WARNING_STATUS_SET_BUT_SQUARE_EMPTY
};

struct casilla{
    Pieza *PiezaOcupante;
    char nombre[LONGNOMBRE];
    uint8_t tipo;
};
typedef struct casilla Casilla;

/*
Pre:
Post: [piece] is the pointer to Pieza stored at the Casilla pointed by [square]
*/
int WritePieceCasilla(uint16_t m, uint16_t n,uint16_t x, uint16_t y, Casilla square[][n], Pieza *piece);

/*
Pre: [name] contains >= LONGNOMBRE chars
Post: the first LONGNOMBRE chars of [name] are the name of the Casilla pointed by [square]
*/
int WriteNameCasilla(uint16_t m, uint16_t n,uint16_t x, uint16_t y, Casilla square[][n], const char name[]);

/*
Pre:
Post: [type] is the type of the Casilla pointed by [square]
*/
int WriteTypeCasilla(uint16_t m, uint16_t n,uint16_t x, uint16_t y, Casilla square[][n], uint8_t type);

/*
Pre:
Post: ReadPiece = pointer to Pieza in [square]
*/
int ReadPieceCasilla(uint16_t m, uint16_t n,uint16_t x, uint16_t y, Casilla square[][n], Pieza **piece);

/*
Pre: [name] can store >= LONGNOMBRE chars
Post: the first LONGNOMBRE chars of [name] are the name of [square]
*/
int ReadNameCasilla(uint16_t m, uint16_t n,uint16_t x, uint16_t y, Casilla square[][n], char name[]);

/*
Pre:
Post: ReadType = type of [square]
*/
int ReadTypeCasilla(uint16_t m, uint16_t n,uint16_t x, uint16_t y, Casilla square[][n], uint8_t *type);

/*
Pre: [name] contains >= LONGNOMBRE chars
Post: MakeCasilla := Casilla with [piece] as a pointer to Pieza, the first LONGNOMBRE chars of [name] as name, and [type] as type
*/
Casilla MakeCasilla(Pieza *piece, const char name[], uint8_t type);

/*
Pre:
Post: IF [p] <= 0 THEN does nothing
    ELSE IF [p] > LONGNOMBRE THEN writes the name of [square] on the stdout
    ELSE writes the first [p] chars of the name of [square] on the stdout
*/
void ShowNameCasilla(Casilla square, int p);

/*
Pre:
Post: IF [square] doesn't contain a piece THEN
            IF [p] <= 0 THEN does nothing
            ELSE IF [p] > LONGNOMBRE THEN writes the name of [square] on the stdout
            ELSE writes the first [p] chars of the name of [square] on the stdout
      ELSE
            [Pz] := piece set in [square]
            IF [p] <= 0 THEN does nothing
            ELSE IF [p] > LONGNOMBRE THEN writes the name of [Pz] on the stdout
            ELSE writes the first [p] chars of the name of [Pz] on the stdout

*/
void ShowContentCasilla(Casilla square, int p);

/*
Pre: [matrix] has [m] rows and [n] columns
Post: FOR alfa IN [0, m - 1].FOR beta IN [0, n - 1].
        IF [matrix[alfa][beta]] doesn't contain a piece THEN
            IF [p] <= 0 THEN does nothing
            ELSE IF [p] > LONGNOMBRE THEN writes the name of [matrix[alfa][beta]] on the stdout
            ELSE writes the first [p] chars of the name of [matrix[alfa][beta]] on the stdout
        ELSE
            [Pz] := piece set in [matrix[alfa][beta]]
            IF [p] <= 0 THEN does nothing
            ELSE IF [p] > LONGNOMBRE THEN writes the name of [Pz] on the stdout
            ELSE writes the first [p] chars of the name of [Pz] on the stdout
    Each value of alfa is written in a new line just below the previous value
*/
void ShowBoard(uint16_t m, uint16_t n, int p, const Casilla matrix[][n]);

/*
Pre: [matrix] has [m] rows and [n] columns AND [Pz] := Pieza pointed by [moved] AND [x] := X axis position of [Pz] AND [y] := Y axis position of [Pz]
Post: IF [x] >= n THEN SetPieceOnBoard := -1
        ELSE IF [y] >= m THEN UnSetPieceOnBoard := -2
        ELSE [Cas] := [matrix[y][x]] AND IF status of [Pz] != piece_out_of_board THEN IF [Cas] contains a piece != [moved] THEN SetPieceOnBoard := 3
                ELSE IF [Cas] contains nothing THEN set [moved] in [Cas] AND SetPieceOnBoard := 2
                ELSE SetPieceOnBoard := 4
        ELSE IF [Cas] doesn't contain a piece THEN set [moved] in [Cas] AND status of [Pz] := piece_on_board AND SetPieceOnBoard := 0
        ELSE SetPieceOnBoard := 1

*/
int SetPieceOnBoard(uint16_t m, uint16_t n, Casilla matrix[][n], Pieza *moved);

/*
Pre: [matrix] has [m] rows and [n] columns AND [Pz] := Pieza pointed by [moved] AND [x] := X axis postion of [Pz] AND [y] := Y axis position of [Pz]
    AND
Post: IF [x] >= n THEN UnSetPieceOfBoard := -1
        ELSE IF [y] >= m THEN UnSetPieceOfBoard := -2
        ELSE IF status of [Pz] = piece_out_of_board THEN UnSetPieceOfBoard := 2
        ELSE [Cas] := [matrix[y][x]] AND IF [moved] is set on [Cas] THEN [Cas] doesn't have a piece set AND status of [PZ] := piece_out_of_board AND UnSetPieceOfBoard := 0
            ELSE status of [PZ] := piece_out_of_board AND UnSetPieceOfBoard := 1

*/
int UnSetPieceOfBoard(uint16_t m, uint16_t n, Casilla matrix[][n], Pieza *moved);

/*
Pre: [matrix] has [m] rows and [n] columns AND [Pz] := Pieza pointed by [PiezaMovida] AND [x0] := X axis postion of [Pz] AND [y0] := Y axis position of [Pz]
Post: IF status of [Pz] = piece_out_of_board THEN MovePieceOnBoard := 2
        ELSE [x0] >= n THEN MovePieceOnBoard := -1
        ELSE IF [y0] >= m THEN MovePieceOnBoard := -2
        ELSE IF [x] >= n THEN MovePieceOnBoard := -3
        ELSE IF [y] >= m THEN MovePieceOnBoard := -4
        ELSE [Cas] := [matrix[y][x]] AND IF [Cas] contains a piece THEN MovePieceOnBoard := 1
        ELSE [Cas0] := [matrix[y0][x0]] AND IF [Cas0] contains a piece != [moved] OR [Cas] contains nothing THEN MovePieceOnBoard := 3
        ELSE [Cas0] contains nothing AND [Cas] contains [moved] AND position of [Pz] := (x,y) AND MovePieceOnBoard := 0
*/
int MovePieceOnBoard(uint16_t m, uint16_t n, Casilla matrix[][n], Pieza *moved, uint16_t x, uint16_t y);


int ReadBoardFile(FILE *file, uint16_t m, uint16_t n, Casilla matrix[][n]);



#endif // CASILLA






