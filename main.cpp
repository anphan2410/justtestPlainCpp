#include "anlogger.h"

#define anMsg(msg, txtAttrib) {\
        anTxtAttribType previousTxtAttrib = 0;\
        if (!(_anGetConsoleTextAttribute(previousTxtAttrib)))\
            previousTxtAttrib = anOriginalConsoleTextAttribute;\
    }

int main(int argc, char *argv[])

    anMsg("this is a test!", anForegroundGreen);

    return 0;
}
