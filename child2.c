#include <stdio.h>
#include <unistd.h>

int main() {
    char ch;
    char prev_ch = '\0';

    while (read(0, &ch, 1) > 0) {
        if (ch == ' ' && prev_ch == ' ') {
            continue; 
        }
        write(1, &ch, 1);
        prev_ch = ch;
    }
    return 0;
}
