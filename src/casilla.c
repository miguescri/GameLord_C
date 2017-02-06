/*
    Module: casilla.c
    Author: Miguel Escribano Perez (miguescri)
    Creation date: 3 December 2015
    Last modification: 13 November 2016
*/

#include "casilla.h"

/*
Pre:
Post: [Ocupante] is the pointer to Pieza stored at the Casilla pointed by [Ocupada]
*/
int WritePieceCasilla(uint16_t m, uint16_t n,uint16_t x, uint16_t y, Casilla square[][n], Pieza *piece){
    if (x >= n)
        return ERROR_X0_OVER_RANGE;
    if (y >= m)
        return ERROR_Y0_OVER_RANGE;

    square[y][x].PiezaOcupante = piece;

    return 0;
}

/*
Pre: [Nombre] contains >= LONGNOMBRE chars
Post: the first LONGNOMBRE chars of [Nombre] are the name of the Casilla pointed by [Ocupada]
*/
int WriteNameCasilla(uint16_t m, uint16_t n,uint16_t x, uint16_t y, Casilla square[][n], const char name[]){
    if (x >= n)
        return ERROR_X0_OVER_RANGE;
    if (y >= m)
        return ERROR_Y0_OVER_RANGE;

    int i;
    for ( i = 0 ; i < LONGNOMBRE ; i++) {
        square[y][x].nombre[i] = name[i];
    }//for

    return 0;
}

/*
Pre:
Post: [tipo] is the type of the Casilla pointed by [Ocupada]
*/
int WriteTypeCasilla(uint16_t m, uint16_t n,uint16_t x, uint16_t y, Casilla square[][n], uint8_t type){
    if (x >= n)
        return ERROR_X0_OVER_RANGE;
    if (y >= m)
        return ERROR_Y0_OVER_RANGE;

    square[y][x].tipo = type;

    return 0;
}

/*
Pre:
Post: ReadPiece = pointer to Pieza in [Ocupada]
*/
int ReadPieceCasilla(uint16_t m, uint16_t n,uint16_t x, uint16_t y, Casilla square[][n], Pieza **piece){
    if (x >= n)
        return ERROR_X0_OVER_RANGE;
    if (y >= m)
        return ERROR_Y0_OVER_RANGE;

    *piece = square[y][x].PiezaOcupante;

    return 0;
}

/*
Pre: [nombre] can store >= LONGNOMBRE chars
Post: the first LONGNOMBRE chars of [nombre] are the name of [Ocupada]
*/
int ReadNameCasilla(uint16_t m, uint16_t n,uint16_t x, uint16_t y, Casilla square[][n], char name[]){
    if (x >= n)
        return ERROR_X0_OVER_RANGE;
    if (y >= m)
        return ERROR_Y0_OVER_RANGE;

    int i;
    for ( i = 0 ; i < LONGNOMBRE ; i++ ) {
        name[i] = square[y][x].nombre[i];
    }//for

    return 0;
}

/*
Pre:
Post: ReadType = type of [square]
*/
int ReadTypeCasilla(uint16_t m, uint16_t n,uint16_t x, uint16_t y, Casilla square[][n], uint8_t *type){
    if(x >= n)
        return ERROR_X0_OVER_RANGE;
    if(y >= m)
        return ERROR_Y0_OVER_RANGE;

    *type = square[y][x].tipo;

    return 0;
}

/*
Pre: [Nombre] contains >= LONGNOMBRE chars
Post: MakeCasilla = Casilla with [Ocupante] as a pointer to Pieza, the first LONGNOMBRE chars of [Nombre] as name, and [tipo] as type
*/
Casilla MakeCasilla(Pieza *piece, const char name[], uint8_t type){
    Casilla nueva;
    WritePieceCasilla(1,1,0,0,&nueva, piece);
    WriteNameCasilla(1,1,0,0,&nueva, name);
    WriteTypeCasilla(1,1,0,0,&nueva, type);

    return nueva;
}

/*
Pre:
Post: IF [p] <= 0 THEN does nothing
    ELSE IF [p] > LONGNOMBRE THEN writes the name of [C] on the stdout
    ELSE writes the first [p] chars of the name of [C] on the stdout
*/
void ShowNameCasilla(Casilla square, int p){
    int i = p;
    if( p > 0 ){
        if( p > LONGNOMBRE ){
            i = LONGNOMBRE;
        }//if
        write(STDOUT_FILENO, square.nombre, i );
    }//if
}

/*
Pre:  [C] contains a pointer [Ptr] to Pieza [Pz]
Post: IF [Ptr] = NULL THEN
            IF [p] <= 0 THEN does nothing
            ELSE IF [p] > LONGNOMBRE THEN writes the name of [C] on the stdout
            ELSE writes the first [p] chars of the name of [C] on the stdout
      ELSE
            IF [p] <= 0 THEN does nothing
            ELSE IF [p] > LONGNOMBRE THEN writes the name of [Pz] on the stdout
            ELSE writes the first [p] chars of the name of [Pz] on the stdout

*/
void ShowContentCasilla(Casilla square, int p){
    Pieza *ptr;
    ReadPieceCasilla(1,1,0,0,&square,&ptr);
    if(ptr == NULL)
        ShowNameCasilla(square, p);
    else
        ShowNamePieza(*ptr, p);
}

/*
Pre: FOR alfa IN [0, m - 1].FOR beta IN [0, n - 1]. THERE IS [Tabla[alfa][beta]] that contains a pointer [Ptr[alfa][beta]] to Pieza [Pz[alfa][beta]]
Post: FOR alfa IN [0, m - 1].FOR beta IN [0, n - 1].
        IF [Ptr[alfa][beta]] = NULL THEN
            IF [p] <= 0 THEN does nothing
            ELSE IF [p] > LONGNOMBRE THEN writes the name of [Tabla[alfa][beta]] on the stdout
            ELSE writes the first [p] chars of the name of [Tabla[alfa][beta]] on the stdout
        ELSE
            IF [p] <= 0 THEN does nothing
            ELSE IF [p] > LONGNOMBRE THEN writes the name of [Pz[alfa][beta]] on the stdout
            ELSE writes the first [p] chars of the name of [Pz[alfa][beta]] on the stdout
    Each value of alfa is written in a new line just below the previous value
*/
void ShowBoard(uint16_t m, uint16_t n, int p, const Casilla matrix[][n]){
    int i, j;

    for(i = 0; i < m; i++){
        for(j = 0; j < n; j++){
            ShowContentCasilla(matrix[i][j], p);
        }
        write(STDOUT_FILENO, "\n", 1);
    }
}

/*
Pre: [matrix] has [m] rows and [n] columns AND [Pz] := Pieza pointed by [moved] AND [x] := X axis postion of [Pz] AND [y] := Y axis position of [Pz]
Post: IF [x] >= n THEN SetPieceOnBoard := -1
        ELSE IF [y] >= m THEN UnSetPieceOnBoard := -2
        ELSE [Cas] := [Tabla[y][x]] AND IF status of [Pz] != piece_out_of_board THEN IF [Cas] contains a piece != [moved] THEN SetPieceOnBoard := 3
                ELSE IF [Cas] contains nothing THEN set [moved] in [Cas] AND SetPieceOnBoard := 2
                ELSE SetPieceOnBoard := 4
        ELSE IF [Cas] doesn't contain a piece THEN set [moved] in [Cas] AND status of [Pz] := piece_on_board AND SetPieceOnBoard := 0
            ELSE SetPieceOnBoard := 1

*/
int SetPieceOnBoard(uint16_t m, uint16_t n, Casilla matrix[][n], Pieza *moved){
    int x = ReadXAxisPieza(*moved);
    int y = ReadYAxisPieza(*moved);

    int status = ReadStatusPieza(*moved);
    Pieza * aux;
    int error = ReadPieceCasilla(m, n, x, y, matrix, &aux);
    if (error != 0)
        return error;

    if(status != PIECE_OUT_OF_BOARD){
        if(aux != moved){
            if(aux == NULL){
                WritePieceCasilla(m, n, x, y, matrix, moved);

                return WARNING_STATUS_SET_BUT_SQUARE_EMPTY;
            }else
                return ERROR_STATUS_SET_BUT_SQUARE_OCCUPIED;
        }else
            return ERROR_PIECE_SET;

    }else{
        if(aux == NULL){
            WritePieceCasilla(m, n, x, y, matrix, moved);
            WriteStatusPieza(moved, PIECE_ON_BOARD);

            return 0;

        }else{
            return ERROR_SQUARE_OCCUPIED;
        }
    }
}

/*
Pre: [matrix] has [m] rows and [n] columns AND [Pz] := Pieza pointed by [moved] AND [x] := X axis postion of [Pz] AND [y] := Y axis position of [Pz]
    AND
Post: IF [x] >= n THEN UnSetPieceOfBoard := -1
        ELSE IF [y] >= m THEN UnSetPieceOfBoard := -2
        ELSE IF status of [Pz] = piece_out_of_board THEN UnSetPieceOfBoard := 2
        ELSE [Cas] := [Tabla[y][x]] AND IF [moved] is set on [Cas] THEN [Cas] doesn't have a piece set AND status of [PZ] := piece_out_of_board AND UnSetPieceOfBoard := 0
            ELSE status of [PZ] := piece_out_of_board AND UnSetPieceOfBoard := 1

*/
int UnSetPieceOfBoard(uint16_t m, uint16_t n, Casilla matrix[][n], Pieza *moved){
    int x = ReadXAxisPieza(*moved);
    int y = ReadYAxisPieza(*moved);

    int status = ReadStatusPieza(*moved);
    if (status == PIECE_OUT_OF_BOARD)
        return ERROR_PIECE_UNSET;

    WriteStatusPieza(moved, PIECE_OUT_OF_BOARD);
    Pieza *aux;
    int error = ReadPieceCasilla(m, n, x, y, matrix, &aux);
    if (error != 0)
        return error;

    if (aux == moved) {
        WritePieceCasilla(m, n, x,y, matrix, NULL);

        return 0;
    }

    return ERROR_STATUS_SET_BUT_SQUARE_OCCUPIED;
}

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
int MovePieceOnBoard(uint16_t m, uint16_t n, Casilla matrix[][n], Pieza *moved, uint16_t x, uint16_t y){
    if(ReadStatusPieza(*moved)== PIECE_OUT_OF_BOARD)
        return ERROR_PIECE_UNSET;

    int x0 = ReadXAxisPieza(*moved);
    if (x0 >= n)
        return ERROR_X0_OVER_RANGE;
    int y0 = ReadYAxisPieza(*moved);
    if (y0 >= m)
        return ERROR_Y0_OVER_RANGE;
    if (x >= n)
        return ERROR_X1_OVER_RANGE;
    if (y >= m)
        return ERROR_Y1_OVER_RANGE;

    Pieza *aux;
    ReadPieceCasilla(m, n, x, y, matrix, &aux);

    if (aux != NULL)
        return ERROR_SQUARE_OCCUPIED;

    ReadPieceCasilla(m, n, x0, y0, matrix, &aux);
    if (aux != moved) {
        return ERROR_STATUS_SET_BUT_SQUARE_OCCUPIED;
    }else if (aux == NULL)
        return WARNING_STATUS_SET_BUT_SQUARE_EMPTY;

    WritePieceCasilla(m, n, x0,y0, matrix, NULL);
    WritePieceCasilla(m, n, x,y, matrix, moved);
    WriteXAxisPieza(moved, x);
    WriteYAxisPieza(moved, y);

    return 0;
}

int ReadBoardFile(FILE *file, uint16_t m, uint16_t n, Casilla matrix[m][n]){
    char *line, name[LONGNOMBRE];
    ssize_t buff_size, chars_read;

    int i, j, k;
    for (i = 0; i < m; i++) {
        if ((chars_read = getline(&line, &buff_size, file)) != -1 && chars_read  > n) {
            for (j = 0; j < n && j < chars_read; j++) {
                WriteTypeCasilla(m, n, j, i, matrix, line[j]);
                for (k = 0; k < LONGNOMBRE; k++)
                    name[k] = line[j];
                WriteNameCasilla(m, n,j, i, matrix, name);
            }
        } else{
            free(line);
            return -1;
        }
    }

    free(line);
    return 0;
}















