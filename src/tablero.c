/*
    Module: tablero.c
    Author: Miguel Escribano Perez (miguescri)
    Creation date: 14 November 2016
    Last modification: 14 November 2016

*/


#include "tablero.h"

int SearchPieceId(Tablero *board, uint32_t id, Pieza **piece){
    Pieza *aux = NULL;
    int i;
    for (i = 0; i < board->piezas_guardadas; i++) {
        if (ReadIdPieza(board->conjunto_piezas[i]) == id){
            aux = &board->conjunto_piezas[i];
        }
    }
    if (aux == NULL)
        return ERROR_ID_UNUSED;

    *piece = aux;

    return 0;
}

/*
Pre: [board] has not been previously initialized with CreateTablero OR [board] was initialized with CreateTablero and later destroyed with DestroyTablero
    OTHERWISE undefined behaviour
Post: IF there is a problem while allocating memory, an error message is written on the STDERR AND the program exits
    ELSE [board] has memory filled with zeros for [number_pieces] pieces and [number_rows]*[number_columns] squares AND [board] contains info about the number of pieces and squares
*/
void CreateTablero(Tablero *board, uint16_t number_rows, uint16_t number_columns){
    board->piezas_guardadas = 0;
    board->columnas = number_columns;
    board->filas = number_rows;
    if ((board->conjunto_piezas = calloc(MAX_PIECES, sizeof(Pieza))) == NULL) {
        write(STDERR_FILENO, "\nError when allocating memory for pieces\n", 41);
        exit(1);
    }
    if ((board->conjunto_casillas = calloc(number_rows*number_columns, sizeof(Casilla))) == NULL) {
        write(STDERR_FILENO, "\nError when allocating memory for squares\n", 42);
        exit(1);
    }
}

/*
Pre: [board] has been at least once initiallized with CreateTablero OTHERWISE undefined behaviour
Post: [board] has no memory allocated and cannot access the memory that previously had.
*/
void DestroyTablero(Tablero *board){
    free(board->conjunto_piezas);
    board->conjunto_piezas = NULL;
    free(board->conjunto_casillas);
    board->conjunto_casillas = NULL;
    board->piezas_guardadas = 0;
    board->filas = 0;
    board->columnas = 0;
}

void infoTablero(Tablero board, int *number_pieces, int *number_rows, int *number_columns){
    *number_pieces = board.piezas_guardadas;
    *number_rows = board.filas;
    *number_columns = board.columnas;
}


int AddPieceTablero(Tablero *board, Pieza piece){
    if (board->piezas_guardadas >= MAX_PIECES)
        return ERROR_NO_SPACE;

    int i;
    for (i = 0; i < board->piezas_guardadas; i++) {
        if (ReadIdPieza(board->conjunto_piezas[i]) == ReadIdPieza(piece))
            return ERROR_ID_USED;
    }

    board->conjunto_piezas[board->piezas_guardadas] = piece;
    board->piezas_guardadas++;

    return 0;
}

int InitPieceIterator(Tablero *board){
    if (board->piezas_guardadas > 0) {
        board->iterador_piezas = 0;
    } else {
        board->iterador_piezas = -1;
    }
}

int IteratePiece(Tablero *board, Pieza *piece){
    if (board->iterador_piezas >= 0 && board->iterador_piezas < board->piezas_guardadas) {
        *piece = board->conjunto_piezas[board->iterador_piezas];
        if (board->iterador_piezas+1 >= board->piezas_guardadas) {
            board->iterador_piezas = -1;
        } else {
            board->iterador_piezas += 1;
        }

        return 0;
    } else {
        return -1;
    }
}

int ChangeTypeSquareTablero(Tablero *board, uint16_t x, uint16_t y, uint8_t type){
    return WriteTypeCasilla(board->filas, board->columnas, x, y, board->conjunto_casillas, type);
}

int ChangeNameSquareTablero(Tablero *board, uint16_t x, uint16_t y, char name[]){
    return WriteNameCasilla(board->filas, board->columnas, x, y, board->conjunto_casillas, name);
}

int RemovePieceTablero(Tablero *board, uint32_t id){
    int i;
    for (i = 0; i < board->piezas_guardadas; i++) {
        if (ReadIdPieza(board->conjunto_piezas[i]) == id) {
            UnSetPieceOfBoard(board->filas, board->columnas, board->conjunto_casillas, &board->conjunto_piezas[i]);

            board->conjunto_piezas[i] = board->conjunto_piezas[board->piezas_guardadas]; //move last one to the erased position
            board->piezas_guardadas--;
            return 0;
        }
    }
    return ERROR_ID_UNUSED;
}

int ReadPieceIdTablero(Tablero board, uint32_t id, Pieza *piece){
    Pieza *aux;
    int error = SearchPieceId(&board, id, &aux);

    if (error != 0)
        return error;

    *piece = *aux;

    return 0;
}
int ReadPiecePositionTablero(Tablero board, uint16_t x, uint16_t y, Pieza *piece){
    Pieza *aux;
    int error = ReadPieceCasilla(board.filas, board.columnas, x, y, board.conjunto_casillas, &aux);
    if (error != 0)
        return error;

    if (aux == NULL)
        return ERROR_SQUARE_EMPTY;

    *piece = *aux;

    return 0;
}

int ReadTypeSquareTablero(Tablero board, uint16_t x, uint16_t y, uint8_t *type){
    return ReadTypeCasilla(board.filas, board.columnas, x, y, board.conjunto_casillas, type);
}

int ReadNameSquareTablero(Tablero board, uint16_t x, uint16_t y, char name[]){
    return ReadNameCasilla(board.filas, board.columnas, x, y, board.conjunto_casillas, name);
}

int UpdatePieceIdTablero(Tablero *board, uint32_t id, Pieza updated){
    Pieza *aux;

    int error = SearchPieceId(board, id, &aux);

    if (error != 0)
        return error;

    //saves critical info
    uint16_t x = ReadXAxisPieza(*aux);
    uint16_t y = ReadYAxisPieza(*aux);
    uint8_t status = ReadStatusPieza(*aux);

    *aux = updated;  //updates

    //restores critical info
    WriteXAxisPieza(aux, x);
    WriteYAxisPieza(aux, y);
    WriteIdPieza(aux, id);

    if ((ReadStatusPieza(updated) == PIECE_OUT_OF_BOARD && status != PIECE_OUT_OF_BOARD) || (ReadStatusPieza(updated) != PIECE_OUT_OF_BOARD && status == PIECE_OUT_OF_BOARD)) {
        WriteStatusPieza(aux, status);          //if the user tries to set or unset the piece with the function, it is avoided
        return ERROR_INVALID_STATUS_CHANGE;
    }

    return 0;
}

int UpdatePiecePositionTablero(Tablero *board, uint16_t x, uint16_t y, Pieza updated){
    Pieza *aux;
    int error = ReadPieceCasilla(board->filas, board->columnas, x, y, board->conjunto_casillas, &aux);
    if (error != 0)
        return error;

    if (aux == NULL)
        return ERROR_SQUARE_EMPTY;

    //saves critical info
    uint32_t id = ReadIdPieza(*aux);
    uint8_t status = ReadStatusPieza(*aux);

    *aux = updated;  //updates

    //restores critical info
    WriteXAxisPieza(aux, x);
    WriteYAxisPieza(aux, y);
    WriteIdPieza(aux, id);

    if ((ReadStatusPieza(updated) == PIECE_OUT_OF_BOARD && status != PIECE_OUT_OF_BOARD) || (ReadStatusPieza(updated) != PIECE_OUT_OF_BOARD && status == PIECE_OUT_OF_BOARD)) {
        WriteStatusPieza(aux, status);          //if the user tries to set or unset the piece with the function, it is avoided
        return ERROR_INVALID_STATUS_CHANGE;
    }

    return 0;
}

int MovePieceIdTablero(Tablero *board, uint32_t id, int16_t x, uint16_t y){
    Pieza *aux = NULL;
    int error = SearchPieceId(board, id, &aux);

    if (error != 0)
        return error;

    return MovePieceOnBoard(board->filas, board->columnas, board->conjunto_casillas, aux, x, y);
}

int MovePiecePositionTablero(Tablero *board, uint16_t x_init, uint16_t y_init, uint16_t x, uint16_t y){
    Pieza *aux;
    int error = ReadPieceCasilla(board->filas, board->columnas, x_init, y_init, board->conjunto_casillas, &aux);
    if (error != 0)
        return error;

    if (aux == NULL)
        return ERROR_SQUARE_EMPTY;

    return MovePieceOnBoard(board->filas, board->columnas, board->conjunto_casillas, aux, x, y);
}

int DirectionPieceIdTablero(Tablero *board, uint32_t id, uint8_t direction){
    Pieza *aux = NULL;
    int error = SearchPieceId(board, id, &aux);

    if (error != 0)
        return error;

    WriteDirectionPieza(aux, direction);

    return UpdatePieceIdTablero(board, id, *aux);
}

int DirectionPiecePositionTablero(Tablero *board, uint16_t x_init, uint16_t y_init, uint8_t direction){
    Pieza *aux;
    int error = ReadPieceCasilla(board->filas, board->columnas, x_init, y_init, board->conjunto_casillas, &aux);
    if (error != 0)
        return error;

    if (aux == NULL)
        return ERROR_SQUARE_EMPTY;

    WriteDirectionPieza(aux, direction);

    return UpdatePiecePositionTablero(board, x_init, y_init, *aux);
}

int SetPieceIdTablero(Tablero *board, uint32_t id){
    Pieza *aux = NULL;
    int error = SearchPieceId(board, id, &aux);

    if (error != 0)
        return error;

    return SetPieceOnBoard(board->filas, board->columnas, board->conjunto_casillas, aux);
}

int UnsetPieceIdTablero(Tablero *board, uint32_t id){
    Pieza *aux = NULL;
    int error = SearchPieceId(board, id, &aux);

    if (error != 0)
        return error;

    return UnSetPieceOfBoard(board->filas, board->columnas, board->conjunto_casillas, aux);
}

int UnsetPiecePositionTablero(Tablero *board, uint16_t x, uint16_t y){
    Pieza *aux;
    int error = ReadPieceCasilla(board->filas, board->columnas, x, y, board->conjunto_casillas, &aux);
    if (error != 0)
        return error;

    if (aux == NULL)
        return ERROR_SQUARE_EMPTY;

    return UnSetPieceOfBoard(board->filas, board->columnas, board->conjunto_casillas, aux);
}

int ReadTableroFile(FILE *file_board, FILE *file_pieces, Tablero *board){
    ReadBoardFile(file_board, board->filas, board->columnas, board->conjunto_casillas);
    Pieza aux;
    FILE *tmp = fopen("aux.txt", "w+");
    comment_eraser(file_pieces, tmp);
    fseek(tmp, 0, SEEK_SET);
    while (!feof(tmp)) {
        ReadPiezaFile(tmp, &aux);
        AddPieceTablero(board, aux);
        SetPieceIdTablero(board, aux.identificador);
    }

    fclose(tmp);
        //hay que encontrar una forma de eliminar el fichero temporal

    return 0;
}

