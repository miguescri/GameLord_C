#include "input_interface.h"

#include <stdio.h>

int WaitInteraction(enum buttons type){
    if (type == ANY_BUTTON) {
        ReadLastInteraction();
    } else {
        while (ReadLastInteraction() != type) {}
    }

    return 0;
}

enum buttons ReadLastInteraction(){
    char c = getchar();
    while(getchar() != '\n'){}

    switch (c) {
        case 'w':return UP_BUTTON;
        case 'a':return LEFT_BUTTON;
        case 's':return DOWN_BUTTON;
        case 'd':return RIGHT_BUTTON;
        case 'i':return A_BUTTON;
        case 'o':return B_BUTTON;
        default:return ANY_BUTTON;
    }
}
