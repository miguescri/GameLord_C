#ifndef INPUT_INTERFACE_H_INCLUDED
#define INPUT_INTERFACE_H_INCLUDED

enum buttons{ANY_BUTTON,
            UP_BUTTON,
            DOWN_BUTTON,
            LEFT_BUTTON,
            RIGHT_BUTTON,
            L_BUTTON,
            R_BUTTON,
            A_BUTTON,
            B_BUTTON,
            X_BUTTON,
            Y_BUTTON,
            START_BUTTON,
            SELECT_BUTTON,
            TACTIL_TOUCH
};

int WaitInteraction(enum buttons type);

enum buttons ReadLastInteraction();

#endif // INPUT_INTERFACE_H_INCLUDED
