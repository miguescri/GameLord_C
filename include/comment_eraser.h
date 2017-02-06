/*
    Module: comment_eraser.h
    Author: Miguel Escribano Perez (miguescri)
    Creation date: 25 January 2017
    Last modification: 25 January 2017

*/
#ifndef COMMENT_ERASER_H_INCLUDED
#define COMMENT_ERASER_H_INCLUDED


/** \brief Elimina las líneas vacías y que comienzan por #
 *
 * \param entrada FILE*
 * \param salida FILE*
 * \return int
 *
 */
int limpiar_fichero(FILE *entrada, FILE *salida);

#endif // COMMENT_ERASER_H_INCLUDED
