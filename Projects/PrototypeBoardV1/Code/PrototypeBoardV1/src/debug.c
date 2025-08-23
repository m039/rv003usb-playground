#include <ch32fun.h>

__attribute__((used)) int _write(int fd, const char *buf, int size) {
    int writeSize = size;

    do {
        /**
         * data0  data1 8 bytes
         * data0 The lowest byte storage length, the maximum is 7
         *
         */
        while (*DMDATA0) {}

        *DMDATA1 = (*(buf + 3)) | (*(buf + 4) << 8) | (*(buf + 5) << 16) | (*(buf + 6) << 24);
        *DMDATA0 = ((writeSize > 7) ? 7 : writeSize) | (*buf << 8) | (*(buf + 1) << 16) | (*(buf + 2) << 24);

        if (writeSize > 7) {
            writeSize -= 7;
            buf += 7;
        } else
            writeSize = 0;
    } while (writeSize);
    
    return size;
}