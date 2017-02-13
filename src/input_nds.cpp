#include "input_interface.h"

#include <stdio.h>

int WaitInteraction(enum buttons type){
    if (type == ANY_BUTTON) {
        while (ReadLastInteraction() == ANY_BUTTON) {}
            //this function returning ANY_BUTTON implies no interaction
    } else {
        while (ReadLastInteraction() != type) {}
    }

    return 0;
}

enum buttons ReadLastInteraction(){
    int key = keysDown();
    
    if (key & KEY_UP)
        return UP_BUTTON;
    else if (key & KEY_LEFT)
        return LEFT_BUTTON;
    else if (key & KEY_DOWN)
        return DOWN_BUTTON;
    else if (key & KEY_RIGHT)
        return RIGHT_BUTTON;
    else if (key & KEY_A)
        return A_BUTTON;
    else if (key & KEY_B)
        return B_BUTTON;
    else 
        return ANY_BUTTON;
    }
}
