#include <stdio.h>
#include <ctype.h>
#include <unistd.h>

int main() {
    char ch;
    while (read(0, &ch, 1) > 0) {
        ch = toupper((unsigned char)ch);
        write(1, &ch, 1);
    }
    return 0;
}
