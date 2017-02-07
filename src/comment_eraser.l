%{
    /*
        Module: comment_eraser.l
        Author: Miguel Escribano Perez (miguescri)
        Creation date: 25 January 2017
        Last modification: 25 January 2017

    */
    
    #include <stdio.h>
    #include <unistd.h>
    #include "comment_eraser.h"
    
    
    int comment_eraser(FILE *entrada, FILE *salida){
        yyset_in(entrada);
        yyset_out(salida);
        yylex();
        
        return 0;
    }

%}

%%

^#.*\n              {/*elimina líneas de comentario*/} 
[^" "\n]+.*\n       {fprintf(yyout, "%s", yytext); /*elimina los espacios al comienzo de línea*/}
.|\n                {/*limpia líneas vacías*/}


%%
/*
int main(){
    char *file1 = "entrada.txt", *file2 = "salida.txt";
    FILE *entrada = fopen(file1, "r"),
            *salida = fopen(file2, "w");
    
    limpiar_fichero(entrada, salida);
    
    return 0;    
}
*/
