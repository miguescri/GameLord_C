/*
    Module: pieza.c
    Author: Miguel Escribano Perez (miguescri)
    Creation date: 3 December 2015
    Last modification: 13 November 2016

*/

#include "pieza.h"


/*
Pre:
Post: [x] is the new value for the X axis position of the Pieza pointed by [m]
*/
void WriteXAxisPieza(Pieza *m, uint16_t x){
    m->posicion_x = x;
}

/*
Pre:
Post: [y] is the new value for the Y axis position of the Pieza pointed by [m]
*/
void WriteYAxisPieza(Pieza *m, uint16_t y){
    m->posicion_y = y;
}

/*
Pre:
Post: [tipo] is the new type of the Pieza pointed by [m]
*/
void WriteTypePieza(Pieza *m, uint8_t type){
    m->tipo = type;
}

/*
Pre: [nombre] contains >= LONGNOMBRE chars
Post: the first LONGNOMBRE chars of [nombre] are the new name of the Pieza pointed by [m]
*/
void WriteNamePieza( Pieza *m, const char name[]){
    int i;
    for( i = 0 ; i < LONGNOMBRE ; i++){
        m->nombre[i] = name[i];
    }//for
    m->nombre[LONGNOMBRE] = '\0';
}

/*
Pre:
Post: [equipo] is the new team of the Pieza pointed by [m]
*/
void WriteTeamPieza( Pieza *m, uint8_t team){
    m->equipo = team;
}

/*
Pre:
Post: [estado] is the new status of the Pieza pointed by [m]
*/
void WriteStatusPieza( Pieza *m, uint8_t status){
    m->estado = status;
}

/*
Pre:
Post: [identificador] is the new id of the Pieza pointed by [m]
*/
void WriteIdPieza( Pieza *m, uint32_t id){
    m->identificador = id;
}

void WriteDirectionPieza( Pieza *m, uint8_t direction){
    m->direccion = direction;
}

/*
Pre:
Post: ReadXAxis = position on X axis of [m]
*/
int ReadXAxisPieza(Pieza m){
    return m.posicion_x;
}

/*
Pre:
Post: ReadYAxis = position on Y axis of [m]
*/
int ReadYAxisPieza(Pieza m){
    return m.posicion_y;
}

/*
Pre:
Post: ReadType = type of [m]
*/
int ReadTypePieza(Pieza m){
    return m.tipo;
}

/*
Pre: [nombre] has space for >= LONGNOMBRE chars
Post: the first LONGNOMBRE chars of [nombre] are the name of [m]
*/
void ReadNamePieza( Pieza m, char name[]){
    int i;
    for( i = 0 ; i <= LONGNOMBRE ; i++ ){
        name[i] = m.nombre[i];
    }//for
}

/*
Pre:
Post: ReadTeam = team of [m]
*/
int ReadTeamPieza( Pieza m){
    return m.equipo;
}

/*
Pre:
Post: ReadStatus = status of [m]
*/
int ReadStatusPieza( Pieza m){
    return m.estado;
}

/*
Pre:
Post: ReadIdPieza = id of [m]
*/
uint32_t ReadIdPieza( Pieza m){
    return m.identificador;
}

uint8_t ReadDirectionPieza( Pieza m){
    return m.direccion;
}


/*
Pre: [name] contains >= LONGNOMBRE chars
Post: MakePieza = Pieza with [x] as X axis position, [y] as Y axis position, [type] as type, the first LONGNOMBRE chars of [name]
    as name, [team] as team, [status] as status and [id] as identifier.
*/
Pieza MakePieza(uint16_t x, uint16_t y, uint8_t type, char name[], uint8_t team, uint8_t status, uint32_t id, uint8_t direction){
    Pieza nueva;

    WriteXAxisPieza(&nueva, x);
    WriteYAxisPieza(&nueva, y);
    WriteTypePieza(&nueva, type);
    WriteNamePieza(&nueva, name);
    WriteTeamPieza(&nueva, team);
    WriteStatusPieza(&nueva, status);
    WriteIdPieza(&nueva, id);
    WriteDirectionPieza(&nueva, direction);

    return nueva;
}

/*
Pre:
Post: IF [p] <= 0 THEN does nothing
    ELSE IF [p] > LONGNOMBRE THEN writes the name of [m] on the stdout
    ELSE writes the first [p] chars of the name of [m] on the stdout
*/
void ShowNamePieza(Pieza m, int p){
    int i = p;
    if( p > 0 ){
        if( p > LONGNOMBRE ){
            i = LONGNOMBRE;
        }//if
        write(STDOUT_FILENO, m.nombre, i );
    }//if
}

int WritePiezaFile(FILE *file, Pieza piece){
    fprintf(file, "\n# Piece ID\n%ju", piece.identificador);
    fprintf(file, "\n# Name\n%s", piece.nombre);
    fprintf(file, "\n# Type\n%u", piece.tipo);
    fprintf(file, "\n# X and Y position\n%u %u", piece.posicion_x, piece.posicion_y);
    fprintf(file, "\n# Status (0 = out of the game board)\n%u", piece.estado);
    fprintf(file, "\n# Team\n%ju", piece.equipo);

    return 0;
}

int ReadPiezaFile(FILE *file, Pieza *piece){
    char cadena[LONGNOMBRE];
    fscanf(file, "%ju\n%s\n%u\n%u %u\n%u\n%ju", &piece->identificador, cadena,
           &piece->tipo, &piece->identificador, &piece->posicion_x,
           &piece->posicion_y, &piece->estado, &piece->equipo);
    WriteNamePieza(piece, cadena);

    return 0;
}


