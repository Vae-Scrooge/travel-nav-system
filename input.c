#include "global.h"

int readString(char *buffer, size_t size)
{
        char temp[256];
        int result = 0;
        if(buffer == NULL || size == 0)
        {
                return 0;
        }

        result = scanf("%255s", temp);
        if(result != 1)
        {
                buffer[0] = '\0';
                return 0;
        }

        strncpy(buffer, temp, size - 1);
        buffer[size - 1] = '\0';
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
