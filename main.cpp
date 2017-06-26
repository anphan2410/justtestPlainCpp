#include "anlogger.h"

#define anMsgTest(msg, txtAttrib) {\
		std::cout << "TEST GATE 1" << std::endl; \
        anTxtAttribType previousTxtAttrib = 0; \
        if (!(_anGetConsoleTextAttribute(previousTxtAttrib))) \
            previousTxtAttrib = anOriginalConsoleTextAttribute; \
		std::cout << "TEST GATE 2" << std::endl; \	
        anSetConsoleTextAttribute(txtAttrib) \
		std::cout << "TEST GATE 3" << std::endl; \
        anMsgInputToMsgString(msg, tmp) \
		std::cout << "TEST GATE 4" << std::endl; \
        std::cerr << tmp; \
		std::cout << "TEST GATE 5" << std::endl; \
        anSetConsoleTextAttribute(previousTxtAttrib) \
	}

int main(int argc, char *argv[])

    anMsgTest("this is a test!", anForegroundGreen);

    return 0;
}
