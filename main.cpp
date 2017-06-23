#include "anlogger.h"

using namespace std;

int main(int argc, char *argv[])
{
    	anMsg("=> This Should Be Normally Printed !", anForegroundGreen);
    	std::cout << "This Line Should Be Shown In White !" << std::endl;
	return 0;
}
