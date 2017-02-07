/*
    Module: tablero.h
    Author: Miguel Escribano Perez (miguescri)
    Creation date: 14 November 2016
    Last modification: 14 November 2016

*/




#ifndef TABLERO_H
#define TABLERO_H

#include <stdlib.h>
#include "casilla.h"
#include "pieza.h"
#include "comment_eraser.h"


#define MAX_PIECES 32

enum tablero_error{ERROR_ID_USED = 30,
    ERROR_ID_UNUSED,
    ERROR_INVALID_STATUS_CHANGE,
    ERROR_NO_SPACE
};


struct tablero{
    Pieza *conjunto_piezas;
    uint32_t piezas_guardadas, iterador_piezas;
    Casilla *conjunto_casillas;
    uint16_t filas, columnas;
};
typedef struct tablero Tablero;

/*
Pre: [board] has not been previously initialized with CreateTablero OR [board] was initialized with CreateTablero and later destroyed with DestroyTablero
    OTHERWISE undefined behaviour
Post: IF there is a problem while allocating memory, an error message is written on the STDERR AND the program exits
    ELSE [board] has memory filled with zeros for [number_pieces] pieces and [number_rows]*[number_columns] squares AND [board] contains info about the number of pieces and squares
*/
void CreateTablero(Tablero *board, uint16_t number_rows, uint16_t number_columns);

/*
Pre: [board] has been at least once initiallized with CreateTablero OTHERWISE undefined behaviour
Post: [board] has no memory allocated and cannot access the memory that previously had.
*/
void DestroyTablero(Tablero *board);

void infoTablero(Tablero board, int *number_pieces, int *number_rows, int *number_columns);

/*
Pre: [board] has been initiallized with CreateTablero OTHERWISE undefined behaviour
Post: IF there already is a piece with the identifier of [piece] THEN AddPieceTablero := 1
*/
int AddPieceTablero(Tablero *board, Pieza piece);

int ChangeTypeSquareTablero(Tablero *board, uint16_t x, uint16_t y, uint8_t type);

int ChangeNameSquareTablero(Tablero *board, uint16_t x, uint16_t y, char name[]);

int RemovePieceTablero(Tablero *board, uint32_t id);

int ReadPieceIdTablero(Tablero board, uint32_t id, Pieza *piece);
int ReadPiecePositionTablero(Tablero board, uint16_t x, uint16_t y, Pieza *piece);

int ReadTypeSquareTablero(Tablero board, uint16_t x, uint16_t y, uint8_t *type);

int ReadNameSquareTablero(Tablero board, uint16_t x, uint16_t y, char name[]);

/*Doesn't update position, in/out board status (it does change between different in statuses) nor id*/
int UpdatePieceIdTablero(Tablero *board, uint32_t id, Pieza updated);
int UpdatePiecePositionTablero(Tablero *board, uint16_t x, uint16_t y, Pieza updated);

int MovePieceIdTablero(Tablero *board, uint32_t id, int16_t x, uint16_t y);
int MovePiecePositionTablero(Tablero *board, uint16_t x_init, uint16_t y_init, uint16_t x, uint16_t y);

int DirectionPieceIdTablero(Tablero *board, uint32_t id, uint8_t direction);
int DirectionPiecePositionTablero(Tablero *board, uint16_t x_init, uint16_t y_init, uint8_t direction);

int SetPieceIdTablero(Tablero *board, uint32_t id);

int UnsetPieceIdTablero(Tablero *board, uint32_t id);
int UnsetPiecePositionTablero(Tablero *board, uint16_t x, uint16_t y);

int ReadTableroFile(FILE *file_board, FILE *file_pieces, Tablero *board);

/*TODO: writetablerofile*/





#endif // TABLERO_H
