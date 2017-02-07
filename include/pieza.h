/*
    Module: pieza.h
    Author: Miguel Escribano Perez (miguescri)
    Creation date: 3 December 2015
    Last modification: 13 November 2016

*/

/*
    This library defines the type Pieza and the functions that interact with it.
    Pieza represents a piece of a board game with information about X and Y position, identifier, team, name, type of piece and status.
*/

/*TODO:
    -Change the name representation to C strings both in struct and functions
    -Change showPiece() for it to give a C string as output and not write on STDOUT
    -Update documentation
*/

#ifndef PIEZA_H
#define PIEZA_H

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <inttypes.h>

#define LONGNOMBRE 1

struct pieza{
    uint16_t posicion_x;
    uint16_t posicion_y;
    uint8_t tipo;
    char nombre[LONGNOMBRE+1];
    uint8_t equipo;
    uint8_t estado;
    uint32_t identificador;
    uint8_t direccion;
};
typedef struct pieza Pieza;


/*
Pre:
Post: [x] is the new value for the X axis position of the Pieza pointed by [m]
*/
void WriteXAxisPieza(Pieza *m, uint16_t x);

/*
Pre:
Post: [y] is the new value for the Y axis position of the Pieza pointed by [m]
*/
void WriteYAxisPieza(Pieza *m, uint16_t y);

/*
Pre:
Post: [type] is the new type of the Pieza pointed by [m]
*/
void WriteTypePieza(Pieza *m, uint8_t type);

/*
Pre: [name] contains >= LONGNOMBRE chars
Post: the first LONGNOMBRE chars of [name] are the new name of the Pieza pointed by [m]
*/
void WriteNamePieza( Pieza *m, const char name[]);

/*
Pre:
Post: [team] is the new team of the Pieza pointed by [m]
*/
void WriteTeamPieza( Pieza *m, uint8_t team);

/*
Pre:
Post: [status] is the new status of the Pieza pointed by [m]
*/
void WriteStatusPieza( Pieza *m, uint8_t status);

/*
Pre:
Post: [id] is the new id of the Pieza pointed by [m]
*/
void WriteIdPieza( Pieza *m, uint32_t id);

void WriteDirectionPieza( Pieza *m, uint8_t direction);

/*
Pre:
Post: ReadXAxis = position on X axis of [m]
*/
int ReadXAxisPieza(Pieza m);

/*
Pre:
Post: ReadYAxis = position on Y axis of [m]
*/
int ReadYAxisPieza(Pieza m);

/*
Pre:
Post: ReadType = type of [m]
*/
int ReadTypePieza(Pieza m);

/*
Pre: [name] has space for >= LONGNOMBRE chars
Post: the first LONGNOMBRE chars of [name] are the name of [m]
*/
void ReadNamePieza( Pieza m, char name[]);

/*
Pre:
Post: ReadTeam = team of [m]
*/
int ReadTeamPieza( Pieza m);

/*
Pre:
Post: ReadStatus = status of [m]
*/
int ReadStatusPieza( Pieza m);

/*
Pre:
Post: ReadIdPieza = id of [m]
*/
uint32_t ReadIdPieza( Pieza m);

uint8_t ReadDirectionPieza( Pieza m);

/*
Pre: [name] contains >= LONGNOMBRE chars
Post: MakePieza := Pieza with [x] as X axis position, [y] as Y axis position, [type] as type, the first LONGNOMBRE chars of [name]
    as name, [team] as team, [status] as status and [id] as identifier.
*/
Pieza MakePieza(uint16_t x, uint16_t y, uint8_t type, char name[], uint8_t team, uint8_t status, uint32_t id, uint8_t direction);

/*
Pre:
Post: IF [p] <= 0 THEN does nothing
    ELSE IF [p] > LONGNOMBRE THEN writes the name of [m] on the stdout
    ELSE writes the first [p] chars of the name of [m] on the stdout
*/
void ShowNamePieza(Pieza m, int p);

int WritePiezaFile(FILE *file, Pieza piece);

int ReadPiezaFile(FILE *file, Pieza *piece);

#endif //pieza
