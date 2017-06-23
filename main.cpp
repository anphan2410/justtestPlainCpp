#include "anlogger.h"
using namespace std;

int main(int argc, char *argv[])
{
        anMsgWithColor("=> This Should Be Normally Printed !" << std::endl, anForegroundGreen);
        cout << __PRETTY_FUNCTION__ << endl;
#ifdef _WIN32
        std::string tmp(__FILE__);
        cout << tmp.substr(tmp.find_last_of("/\\")) << endl;
#else

#endif
        std::cout << std::endl << "This Line Should Be Shown In White !" << std::endl;
	return 0;
}
