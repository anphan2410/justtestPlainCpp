#ifndef ANLOGGER_H
#define ANLOGGER_H
//PREREQUISITE: -std=c++11
/************* Control Flags ****************************************************/
//_anLoggerEnabled Is Zero => Logger Is Globally Disabled
#define _anLoggerEnabled 1
//_anLoggerEnabled Is Zero => Logger Message Is Not Verbosely Positioned
#define _anPositionEnabled 1
/************* Performance Flags ************************************************/
#define _anMessagePathTextAttribute anDefaultTextAttribute
#define _anThreadIdPositionEnabled 1
#define _anFunctionPositionEnabled 1
#define _anFilePositionEnabled 1
#define _anLinePositionEnabled 1
#define _anTimePositionEnabled 1
/********************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <sstream>
#include <thread>
#include <chrono>
#include <time.h>
#include <mutex>
#if defined _WINDOWS_ || defined __WINDOWS__\
        || defined _WIN16 || defined __TOS_WIN__\
        || defined _WIN32_WCE || defined _WIN32_WCE\
        || defined WIN32 || defined _WIN32 || defined _WIN32_\
        || defined __WIN32 || defined __WIN32__\
        || defined _WIN32_WINNT || defined WINNT || defined _WINNT\
        || defined _WINNT_ || defined __WINNT || defined __WINNT__
    #include <windows.h>
    #define __anWINOS__
#elif defined linux || defined _linux_ || defined __linux || defined __linux__\
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
/************* Support Variables, Functions and Macros **************************/

static char anStdErrBuffer[BUFSIZ];

static const std::chrono::steady_clock::time_point anThisProgramStartingTimePoint = [](){
    setvbuf(stderr, anStdErrBuffer, _IOFBF, BUFSIZ);
    return std::chrono::steady_clock::now();
}();
#define __anStartTimePoint__ anThisProgramStartingTimePoint

#define anTxtAttribType unsigned short

#ifdef __anWINOS__

/*  8Bit - ConsoleTextAttribute
    bit 0 - foreground blue
    bit 1 - foreground green
    bit 2 - foreground red
    bit 3 - foreground intensity
    bit 4 - background blue
    bit 5 - background green
    bit 6 - background red
    bit 7 - background intensity */
    #define anForegroundBlack 0
    #define anForegroundRed 0b00001100
    #define anForegroundGreen 0b00001010
    #define anForegroundYellow 0b00001110
    #define anForegroundBlue 0b00001001
    #define anForegroundMagenta 0b00001101
    #define anForegroundCyan 0b00001011
    #define anForegroundWhite 0b00001111
    #define anDefaultTextAttribute 0b00000111

    inline static bool anGetCurrentConsoleTextAttribute(anTxtAttribType &OutputVar) {
        CONSOLE_SCREEN_BUFFER_INFO tmp;
        if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &tmp))
        {
            OutputVar = tmp.wAttributes;
            return true;
        }
        return false;
    }
    #define _anGetConsoleTextAttribute(destination)\
        anGetCurrentConsoleTextAttribute(destination)

    #define anSetConsoleTextAttribute(TextAtrribute)\
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), TextAtrribute);

    #define __anFilePathSlashChar__ u'\\'

#elif defined __anLINUXOS__

    #define anForegroundBlack 30
    #define anForegroundRed 31
    #define anForegroundGreen 32
    #define anForegroundYellow 33
    #define anForegroundBlue 34
    #define anForegroundMagenta 35
    #define anForegroundCyan 36
    #define anForegroundWhite 37
    #define anDefaultTextAttribute 0

    inline static bool anGetCurrentConsoleTextAttribute(anTxtAttribType &OutputVar) {
        std::string tmpBuff = std::string(anStdErrBuffer);
        std::size_t tmp = tmpBuff.find_last_of(u8"\033[");
        if (tmp != std::string::npos)
        {
            ++tmp;
            if ((tmpBuff.at(tmp) == u'0') && (tmpBuff.at(tmp+1) == u'm'))
                OutputVar = 0;
            else if ((tmpBuff.at(tmp) == u'3') && (tmpBuff.at(tmp+2) == u'm'))
                OutputVar = std::stoi(tmpBuff.substr(tmp,2));
            else
                return false;
            return true;
        }
        else
            return false;
    }
    #define _anGetConsoleTextAttribute(destination)\
        anGetCurrentConsoleTextAttribute(destination)

    #define anSetConsoleTextAttribute(TxtAttrib) {\
        std::string tmp = u8"\033[";\
        tmp += std::to_string(TxtAttrib);\
        tmp += u8"m";\
        fprintf(stderr, tmp.c_str());\
    }

    #define __anFilePathSlashChar__ u'/'

#endif

static const anTxtAttribType anOriginalConsoleTextAttribute = [](){
    anTxtAttribType tmp = anDefaultTextAttribute;
    anGetCurrentConsoleTextAttribute(tmp);
    return tmp;
}();
#define __anOriginalConsoleTextAttribute__ anOriginalConsoleTextAttribute

#if _anPositionEnabled && (_anThreadIdPositionEnabled\
        || _anFunctionPositionEnabled || _anFilePositionEnabled\
        || _anLinePositionEnabled || _anTimePositionEnabled)

    #if _anThreadIdPositionEnabled
        inline static const std::string anGetCurrentThreadIdStr(const std::thread::id &currentThreadId) {
            std::stringstream tmp;
            tmp << currentThreadId;
            return tmp.str();
        }
        #define __anThreadIdStr__ anGetCurrentThreadIdStr(std::this_thread::get_id())
    #endif

    #if _anFilePositionEnabled

        inline static const std::string anGetCurrentFileName(const std::string &currentFilePath) {
            return currentFilePath.substr(
                        currentFilePath.find_last_of(__anFilePathSlashChar__)+1);
        }
        #define __anFILENAME__ anGetCurrentFileName(__FILE__)

    #endif

    #if _anTimePositionEnabled

        #define __anElapsedTimeNSEC__\
            std::chrono::duration_cast<std::chrono::nanoseconds>(\
                std::chrono::steady_clock::now() - __anStartTimePoint__).count()
    #endif

    inline static const std::string anCurrentMessagePath(
            #if _anTimePositionEnabled
                const unsigned long int &currentElapsedTime,
            #endif
            #if _anThreadIdPositionEnabled
                const std::string &currentThreadIdStr,
            #endif
            #if _anFunctionPositionEnabled
                const std::string &currentFunctionName,
            #endif
            #if _anFilePositionEnabled
                const std::string &currentFileName,
            #endif
            #if _anLinePositionEnabled
                const unsigned int &currentLine
            #endif
            ) {
        std::string tmp = u8"";
            #if _anThreadIdPositionEnabled
                tmp += u8"|";
                tmp += currentThreadIdStr;
            #endif
            #if _anFunctionPositionEnabled
                tmp += u8"|";
                tmp += currentFunctionName;
            #endif
            #if _anFilePositionEnabled
                tmp += u8"|";
                tmp += currentFileName;
            #endif
            #if _anLinePositionEnabled
                tmp += u8"|";
                tmp += std::to_string(currentLine);
            #endif
            #if _anTimePositionEnabled
                tmp += u8"|";
                tmp += std::to_string(currentElapsedTime);
            #endif
        tmp += u8"|";
        return tmp;
    }

    #if _anTimePositionEnabled
        #define anTmpTimeParam __anElapsedTimeNSEC__,
    #else
        #define anTmpTimeParam
    #endif

    #if _anThreadIdPositionEnabled
        #define anTmpThreadIdParam __anThreadIdStr__,
    #else
        #define anTmpThreadIdParam
    #endif

    #if _anFunctionPositionEnabled
        #define anTmpFunctionParam __PRETTY_FUNCTION__,
    #else
        #define anTmpFunctionParam
    #endif

    #if _anFilePositionEnabled
        #define anTmpFileNameParam __anFILENAME__,
    #else
        #define anTmpFileNameParam
    #endif

    #if _anLinePositionEnabled
        #define anTmpLineParam __LINE__
    #else
        #define anTmpLineParam
    #endif

    #define __anMessagePath__\
        anCurrentMessagePath(anTmpTimeParam\
                             anTmpThreadIdParam\
                             anTmpFunctionParam\
                             anTmpFileNameParam\
                             anTmpLineParam)

#else
    #define _anPositionDisabled
#endif

/********************************************************************************/
#if _anLoggerEnabled
/********************************************************************************/
    #define anLogCode(...) __VA_ARGS__

#ifdef __anQt__
    #define anMsgInputToMsgString(msgIn,msgStr)\
            std::string msgStr;\
            {\
                QString * tmpQStrrBefore##msgStr = new QString(u8"");\
                QTextStream(tmpQStrrBefore##msgStr, QIODevice::ReadWrite) << msgIn;\
                msgStr = tmpQStrrBefore##msgStr->toStdString();\
                delete tmpQStrrBefore##msgStr;\
                tmpQStrrBefore##msgStr = Q_NULLPTR;\
            }
#else
    #define anMsgInputToMsgString(msgIn,msgStr)\
            std::string msgStr;\
            {\
                std::stringstream tmpStrStrmBefore##msgStr;\
                tmpStrStrmBefore##msgStr << msgIn;\
                msgStr = tmpStrStrmBefore##msgStr.str();\
            }
#endif

#ifndef _anPositionDisabled
    inline static void anNoLineMessageLogger(const std::string &aNoLineMessage,
                                  const std::string &msgPath,
                                  const anTxtAttribType &preTxtAttrib) {
        fprintf(stderr, aNoLineMessage.c_str());
        anSetConsoleTextAttribute(_anMessagePathTextAttribute)
        fprintf(stderr, msgPath.c_str());
        anSetConsoleTextAttribute(preTxtAttrib)
    }
    inline static void anColorizedMessageLogger(std::string &rawMsgStr,
                                         const anTxtAttribType &txtAttrib,
                                         const std::string &currentMsgPath,
                                         const anTxtAttribType &previousTxtAttrib) {
        anSetConsoleTextAttribute(txtAttrib)
        std::string nowPath = u8"<-";
        nowPath += currentMsgPath;
        std::string tmpPath = nowPath;
        tmpPath += u8"\n";
        for(std::string::size_type i = 0;(i = rawMsgStr.find(u8"\n", 0)) != std::string::npos;)
        {
            anNoLineMessageLogger(rawMsgStr.substr(0,i++),tmpPath,txtAttrib);
            rawMsgStr = rawMsgStr.substr(i);
        }
        if (!rawMsgStr.empty())
            anNoLineMessageLogger(rawMsgStr,nowPath,txtAttrib);
        anSetConsoleTextAttribute(previousTxtAttrib)
    }

    #define anMsg(msg, aTextAttribute) {\
        std::string currentMessagePath = __anMessagePath__;\
        anTxtAttribType prevTxtAttrib = 0;\
        if (!(_anGetConsoleTextAttribute(prevTxtAttrib)))\
            prevTxtAttrib = anOriginalConsoleTextAttribute;\
        anMsgInputToMsgString(msg, tmpMsgStr)\
        anColorizedMessageLogger(tmpMsgStr, aTextAttribute, currentMessagePath, prevTxtAttrib);\
        std::cerr.flush();\
    }
#else

    #define anMsg(msg, txtAttrib) {\
        anTxtAttribType previousTxtAttrib = 0;\
        if (!(_anGetConsoleTextAttribute(previousTxtAttrib)))\
            previousTxtAttrib = anOriginalConsoleTextAttribute;\
        anSetConsoleTextAttribute(txtAttrib)\
        anMsgInputToMsgString(msg, tmp)\
        fprintf(stderr, tmp.c_str());\
        anSetConsoleTextAttribute(previousTxtAttrib)\
        std::cerr.flush();\
    }
#endif

#define anDbg(msg, condition) if (condition)\
                                anMsg(u8"=> " << msg << u8"\n", anForegroundCyan)
#define anInfo(msg) anMsg(u8"   " << msg << u8"\n", anForegroundWhite)
#define anAck(msg) anMsg(u8"=> " << msg << u8"\n", anForegroundGreen)
#define anWarning(msg) anMsg(u8"=> " << msg << u8"\n", anForegroundYellow)
#define anError(msg) anMsg(u8"=> " << msg << u8"\n", anForegroundRed)

/********************************************************************************/


/********************************************************************************/
#else
    #define anLogCode(...)
    #define anMsg(msg)
#endif
#endif // ANLOGGER_H
