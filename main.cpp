#include "anlogger.h"

#define anMsg(msg, txtAttrib) \
        std::cout << "This Should Be Printed !" << std::endl;

int main(int argc, char *argv[])

    anMsg("this is a test!", anForegroundGreen);

    return 0;
}
