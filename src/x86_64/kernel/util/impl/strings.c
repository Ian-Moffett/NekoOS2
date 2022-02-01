#include "../strings.h"


unsigned int strlen(const char* const STR) {
    unsigned int len = 0;
    while (STR[len++]);
    return len;
}
