#include "anlogger.h"

int main(int argc, char *argv[])
{


    anInfo("This Type Is To Provide Info !");
    anAck("This Type Is To Show Acknowledgement !");
    anWarning("This Type Is To Show Warning !");
    anError("This Type Is To Warn !");

    std::cout << "END PROGRAM !" << std::endl;

    return 0;
}
