#ifndef ANLOGGER_H
#define ANLOGGER_H
/*************Control Flags******************************************************/
//_anLoggerEnabled Is Zero => Logger Is Globally Disabled
#define _anLoggerEnabled 1
/*************Performance Flags**************************************************/
#define _anExplicitTextAttributeRestoration 0
/********************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <sstream>
#include <thread>
#if defined _WINDOWS_ || defined __WINDOWS__ \
        || defined _WIN16 || defined __TOS_WIN__ \
        || defined _WIN32_WCE || defined _WIN32_WCE \
        || defined WIN32 || defined _WIN32 || defined _WIN32_ \
        || defined __WIN32 || defined __WIN32__ \
        || defined _WIN32_WINNT || defined WINNT || defined _WINNT \
        || defined _WINNT_ || defined __WINNT || defined __WINNT__
    #include <windows.h>
    #define __anWINOS__
#elif defined linux || defined _linux_ || defined __linux || defined __linux__ \
        || defined __gnu_linux__
    #define __anLINUXOS__
#endif
#ifdef QT_CORE_LIB
    #include <QtGlobal>
    #include <QDebug>
    #include <QVariant>
    #include <QString>
    #include <QTime>
    #include <QTextStream>
    #include <QIODevice>
    #define __anQt__
#endif
/*************Standalone Macros**************************************************/

std::string anCurrentLocation() {
    std::stringstream tmp;
    tmp << std::this_thread::get_id();
    return tmp.str();
}

#define __anPath__
#ifdef __anWINOS__
    HANDLE anLastRetrievedStdOutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO anLastRetrievedConsoleScreenBufferInfo;
    WINBOOL anLastConsoleScreenBufferInfoProcurementBooleanResult =
                GetConsoleScreenBufferInfo(anLastRetrievedStdOutputHandle,
                                           &anLastRetrievedConsoleScreenBufferInfo);
    WORD anLastRetrievedConsoleTextAttribute =
            anLastRetrievedConsoleScreenBufferInfo.wAttributes;
    WORD anGetCurrentConsoleTextAttribute() {
        anLastRetrievedStdOutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        anLastConsoleScreenBufferInfoProcurementBooleanResult =
                GetConsoleScreenBufferInfo(anLastRetrievedStdOutputHandle,
                                           &anLastRetrievedConsoleScreenBufferInfo);
        if(anLastConsoleScreenBufferInfoProcurementBooleanResult)
        {
            anLastRetrievedConsoleTextAttribute =
                    anLastRetrievedConsoleScreenBufferInfo.wAttributes;
            return anLastRetrievedConsoleTextAttribute;
        }
        else
            return 0;
    }
    void anSetCurrentConsoleTextAttribute(WORD TextAttribute) {
#if _anExplicitTextAttributeRestoration
        anGetCurrentConsoleTextAttribute();
#endif
        if (TextAttribute)
            anSetConsoleTextAttribute(anLastRetrievedStdOutputHandle,
                                    TextAttribute);
    }
    void anRestorePreviousConsoleTextAttribute() {
        if (anLastRetrievedConsoleTextAttribute)
            anSetConsoleTextAttribute(anLastRetrievedStdOutputHandle,
                                    anLastRetrievedConsoleTextAttribute);
    }
/*  8Bit - ConsoleTextAttribute
    bit 0 - foreground blue
    bit 1 - foreground green
    bit 2 - foreground red
    bit 3 - foreground intensity
    bit 4 - background blue
    bit 5 - background green
    bit 6 - background red
    bit 7 - background intensity */
    #define anForegroundRed 0b00001100
    #define anForegroundGreen 0b00001010
    #define anForegroundYellow 0b00001110
    #define anForegroundBlue 0b00001001
    #define anForegroundMagenta 0b00001101
    #define anForegroundCyan 0b00001011
    #define anForegroundWhite 0b00001111

    #define anSetConsoleTextAttribute(TxtAttrib) \
        anSetCurrentConsoleTextAttribute(TxtAttrib);
    #define anRestoreConsoleTextAttribute \
        anRestorePreviousConsoleTextAttribute();

#elif defined __anLINUXOS__
    #define anForegroundRed 31
    #define anForegroundGreen 32
    #define anForegroundYellow 33
    #define anForegroundBlue 34
    #define anForegroundMagenta 35
    #define anForegroundCyan 36
    #define anForegroundWhite 37

    #define anSetConsoleTextAttribute(TxtAttrib) \
        std::cerr << "\033[" \
        << std::to_string(TxtAttrib)\
        << "m";
    #define anRestoreConsoleTextAttribute \
        std::cerr << "\033[0m";
#endif
/********************************************************************************/
#if _anLoggerEnabled
/********************************************************************************/
    #define anLogCode(...) __VA_ARGS__
#ifdef __anQt__
    #define anMsgWithColor(msg, TxtAttrib) \
        anSetConsoleTextAttribute(TxtAttrib) \
        QTextStream(stderr, QIODevice::WriteOnly) << msg; \
        anRestoreConsoleTextAttribute
    #define anMsg(msg) QTextStream(stderr, QIODevice::WriteOnly) << msg;
#else
    #define anMsgWithColor(msg, TxtAttrib) \
        anSetConsoleTextAttribute(TxtAttrib) \
        std::cerr << msg; \
        anRestoreConsoleTextAttribute
    #define anMsg(msg) std::cerr << msg;
#endif
/********************************************************************************/


/********************************************************************************/
#else
    #define anLogCode(...)
    #define anMsgWithColor(msg, TxtAttrib)
    #define anMsg(msg)
#endif
#endif // ANLOGGER_H
