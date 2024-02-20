#include "debug.h"
#include "TOTP.h"
#include "stdlib.h"
#include "stdio.h"

int main() {
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    SystemCoreClockUpdate();
    Delay_Init();
    USART_Printf_Init(115200);
    unsigned char hmacKey[] = {0x4d, 0x79, 0x4c, 0x65, 0x67, 0x6f, 0x44, 0x6f, 0x6f, 0x72};

    size_t length = sizeof(hmacKey);

    char* base32;

    base32 = (char*)malloc(((length * 8 + 4) / 5) + 1);

    int i, index;
    unsigned int buffer;
    int bitsLeft = 0;
    const char* b32_digits = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";

    for (i = 0, index = 0; i < length; i++) {
        buffer = (buffer << 8) | hmacKey[i];
        bitsLeft += 8;

        while (bitsLeft >= 5) {
            base32[index++] = b32_digits[(buffer >> (bitsLeft - 5)) & 31];
            bitsLeft -= 5;
        }
    }

    if (bitsLeft > 0) {
        buffer <<= (5 - bitsLeft);
        base32[index++] = b32_digits[buffer & 31];

        for (i = 0; i < bitsLeft / 5; i++) {
            base32[index++] = '=';
        }
    }

    base32[index] = '\0';

    printf("%s\r\n", base32);

    free(base32);

    return 0;
}

