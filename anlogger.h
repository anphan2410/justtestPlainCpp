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
#ifdef __anWINOS__
    HANDLE lastRetrievedStdOutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO lastRetrievedConsoleScreenBufferInfo;
    WINBOOL lastConsoleScreenBufferInfoProcurementBooleanResult =
                GetConsoleScreenBufferInfo(lastRetrievedStdOutputHandle,
                                           &lastRetrievedConsoleScreenBufferInfo);
    WORD lastRetrievedConsoleTextAttribute = lastRetrievedConsoleScreenBufferInfo.wAttributes;
    WORD getCurrentConsoleTextAttribute() {
        lastRetrievedStdOutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        lastConsoleScreenBufferInfoProcurementBooleanResult =
                GetConsoleScreenBufferInfo(lastRetrievedStdOutputHandle,
                                           &lastRetrievedConsoleScreenBufferInfo);
        if(lastConsoleScreenBufferInfoProcurementBooleanResult)
        {
            lastRetrievedConsoleTextAttribute = lastRetrievedConsoleScreenBufferInfo.wAttributes;
            return lastRetrievedConsoleTextAttribute;
        }
        else
            return 0;
    }
    void setCurrentConsoleTextAttribute(WORD TextAttribute) {
#if _anExplicitTextAttributeRestoration
        getCurrentConsoleTextAttribute();
#endif
        if (TextAttribute)
            SetConsoleTextAttribute(lastRetrievedStdOutputHandle, TextAttribute);
    }
    void restorePreviousConsoleTextAttribute() {
        if (lastRetrievedConsoleTextAttribute)
            SetConsoleTextAttribute(lastRetrievedStdOutputHandle, lastRetrievedConsoleTextAttribute);
    }
    #define setConsoleTextAttribute(TxtAttrib) setCurrentConsoleTextAttribute(TxtAttrib);
    #define restoreConsoleTextAttribute restorePreviousConsoleTextAttribute();
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
#elif defined __anLINUXOS__
    #define anForegroundRed 31
    #define anForegroundGreen 32
    #define anForegroundYellow 33
    #define anForegroundBlue 34
    #define anForegroundMagenta 35
    #define anForegroundCyan 36
    #define anForegroundWhite 37

    #define setConsoleTextAttribute(TxtAttrib) std::cerr << "\033["   \
                                                    << std::to_string(TxtAttrib)\
                                                    << "m";
    #define restoreConsoleTextAttribute std::cerr << "\033[0m";
#endif
/********************************************************************************/
#if _anLoggerEnabled
/********************************************************************************/
    #define anLogCode(...) __VA_ARGS__
#ifdef __anQt__
    #define anMsg(msg, txtColor)    setConsoleTextAttribute(txtColor)           \
                                    QTextStream(stderr, QIODevice::WriteOnly)   \
                                                                      << msg;   \
                                    restoreConsoleTextAttribute
#else
    #define anMsg(msg, txtColor)    setConsoleTextAttribute(txtColor)           \
                                    std::cerr << msg;                           \
                                    restoreConsoleTextAttribute
#endif
/********************************************************************************/


/********************************************************************************/
#else
#endif
#endif // ANLOGGER_H
