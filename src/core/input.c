#define _POSIX_C_SOURCE 200809L
#include "global.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

static int isInteractiveInput(void)
{
#ifdef _WIN32
        DWORD mode;
        HANDLE hConsole = GetStdHandle(STD_INPUT_HANDLE);
        if(hConsole == INVALID_HANDLE_VALUE)
        {
                return 0;
        }
        if(!GetConsoleMode(hConsole, &mode))
        {
                return 0;
        }
        return 1;
#else
        return isatty(STDIN_FILENO);
#endif
}

static void clearInputBuffer(void)
{
        int c;
        while((c = getchar()) != '\n' && c != EOF)
        {
                ;
        }
}

static void skipWhitespace(void)
{
        int c;
        while((c = getchar()) != EOF && (c == ' ' || c == '\t' || c == '\n' || c == '\r'))
        {
                ;
        }
        if(c != EOF)
        {
                ungetc(c, stdin);
        }
}

int readString(char *buffer, size_t size)
{
        char *newline;
        int c;
        size_t i = 0;

        if(buffer == NULL || size == 0)
        {
                return 0;
        }

        skipWhitespace();

        if(isInteractiveInput())
        {
                if(fgets(buffer, (int)size, stdin) == NULL)
                {
                        buffer[0] = '\0';
                        return 0;
                }

                newline = strchr(buffer, '\n');
                if(newline != NULL)
                {
                        *newline = '\0';
                }
                else
                {
                        clearInputBuffer();
                }
        }
        else
        {
                while(1)
                {
                        c = getchar();
                        if(c == EOF || c == '\n' || c == '\r' || c == ' ' || c == '\t')
                        {
                                if(c != EOF)
                                {
                                        ungetc(c, stdin);
                                }
                                break;
                        }
                        if(i < size - 1)
                        {
                                buffer[i++] = (char)c;
                        }
                        else
                        {
                                while((c = getchar()) != EOF && c != '\n' && c != '\r' && c != ' ' && c != '\t')
                                {
                                        ;
                                }
                                if(c != EOF)
                                {
                                        ungetc(c, stdin);
                                }
                                break;
                        }
                }
                buffer[i] = '\0';
                if(i == 0)
                {
                        return 0;
                }
        }

        return 1;
}

int readInt(int *value)
{
        char buffer[100];
        char *end = NULL;
        long parsed = 0;

        if(value == NULL || !readString(buffer, sizeof(buffer)))
        {
                return 0;
        }

        parsed = strtol(buffer, &end, 10);
        if(end == buffer || *end != '\0')
        {
                return 0;
        }

        *value = (int)parsed;
        return 1;
}

int readPassword(char *buffer, size_t size)
{
        char *newline;
#ifdef _WIN32
        HANDLE hConsole = GetStdHandle(STD_INPUT_HANDLE);
        DWORD mode;
        BOOL success;

        if(buffer == NULL || size == 0)
        {
                return 0;
        }

        success = GetConsoleMode(hConsole, &mode);
        if(success)
        {
                SetConsoleMode(hConsole, mode & ~ENABLE_ECHO_INPUT);
        }

        if(fgets(buffer, (int)size, stdin) == NULL)
        {
                buffer[0] = '\0';
                if(success)
                {
                        SetConsoleMode(hConsole, mode);
                }
                return 0;
        }

        if(success)
        {
                SetConsoleMode(hConsole, mode);
        }

        newline = strchr(buffer, '\n');
        if(newline != NULL)
        {
                *newline = '\0';
        }
        else
        {
                clearInputBuffer();
        }

        return 1;
#else
        struct termios oldTerm, newTerm;

        if(buffer == NULL || size == 0)
        {
                return 0;
        }

        if(tcgetattr(STDIN_FILENO, &oldTerm) != 0)
        {
                return readString(buffer, size);
        }

        newTerm = oldTerm;
        newTerm.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL);

        if(tcsetattr(STDIN_FILENO, TCSANOW, &newTerm) != 0)
        {
                return readString(buffer, size);
        }

        if(fgets(buffer, (int)size, stdin) == NULL)
        {
                buffer[0] = '\0';
                tcsetattr(STDIN_FILENO, TCSANOW, &oldTerm);
                return 0;
        }

        tcsetattr(STDIN_FILENO, TCSANOW, &oldTerm);

        newline = strchr(buffer, '\n');
        if(newline != NULL)
        {
                *newline = '\0';
        }
        else
        {
                clearInputBuffer();
        }

        return 1;
#endif
}