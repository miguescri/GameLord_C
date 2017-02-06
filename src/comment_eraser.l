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
    
    int limpiar_fichero(FILE *entrada, FILE *salida){
        dup(0);
            //stdin queda en la posición 3 de la tabla de descriptores
        close(0);
        dup(fileno(entrada));
            //in.fileno queda en la posición 0 de la tabla de descriptores
            //siendo la entrada por defecto
        dup(1);
            //stdout queda en la posición 4 de la tabla de descriptores

        close(1);
        dup(fileno(salida));
            //out.fileno queda en la posición 1 de la tabla de descriptores
            //siendo la salida por defecto

        yylex();

        close(0);
        dup(3);
        close(3);
            //restaura stdin como la entrada por defecto
        close(1);
        dup(4);
        close(4);
            //restaura stdout como la salida por defecto
            
        return 0;
    }

%}

%%

^#.*\n              {/*elimina líneas de comentario*/} 
[^" "\n]+.*\n       {printf("%s", yytext); /*elimina los espacios al comienzo de línea*/}
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
