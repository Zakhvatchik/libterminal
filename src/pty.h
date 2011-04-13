#ifndef PECERA_PTY_H
#define PECERA_PTY_H

#include <string>
#include <pthread.h>

namespace Pecera {

enum PtyInitResult {
    SUCCESS,
    ERROR_GET_USERS_SHELL,
    ERROR_TCGETATTR,
    ERROR_IOCTL_TIOCGWINSZ,
    ERROR_OPENPT,
    ERROR_GRANTPT,
    ERROR_UNLOCKPT,
    ERROR_PTSNAME,
    ERROR_SET_NONBLOCK,
    ERROR_FORK
};

enum PtyUnputResult {
    UNPUT_SUCCESS,
    UNPUT_ERROR_UNDERFLOW
};

const int PTY_BUFFER_SIZE = 1024;
const int PTY_READ_BUFFER_SIZE = 10240;

class Pty {
public:
    Pty();
    ~Pty();
    static std::string* getUsersShell();
    PtyInitResult init();
    PtyInitResult init(const std::string& pathToExecutable);
    int ptyWrite(const char* buffer, const int count);
    int putChar(const char character);
    void readProcessor();
    PtyUnputResult unPut(int count);

    void readWriteLoop();

    char* getReadEnd();
    char* getWriteStart();
    char* getWriteEnd();

    void setReadEnd(char* value);
    void setWriteStart(char* value);
    void setWriteEnd(char* value);

private:
    int masterfd;
    char* readBuffer;
    char* writeBuffer;
    char* readStart;
    char* readEnd;
    char* writeStart;
    char* writeEnd;

    pthread_mutex_t readEndMutex;
    pthread_mutex_t writeStartMutex;
    pthread_mutex_t writeEndMutex;

    void closeMasterFd();
    int getBufferFreeSpace(const char* buffer, const char* start, const char* end);
    int getBufferUsedSpace(const char* buffer, const char* start, const char* end);
};

};

#endif
