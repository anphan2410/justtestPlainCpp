#include "anlogger.h"


int main(int argc, char *argv[])

    anSetConsoleTextAttribute(anForegroundMagenta);

    anAck("Success !");
    anInfo("It is running !");
    anWarning("too disappointed !");
    anError("oh no it failed !");

    std::cout << "END PROGRAM !" << std::endl;

    return 0;
}
