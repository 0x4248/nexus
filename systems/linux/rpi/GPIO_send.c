/* Raspberry Pi GPIO Send
 * Send a file trough the GPIO pins of a Raspberry Pi using the wiringPi library
 * Licence: GNU General Public License v3.0
 * By: 0x4248
*/

#include <stdio.h>
#include <wiringPi.h>
#include <string.h>
#include <stdlib.h>

void sendBit(int bit, int pin, float delay_time) {
    digitalWrite(pin, bit);
    delay(delay_time);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s [FILE] [PIN] [DELAY]\n", argv[0]);
        printf("\tFILE: The file to send\n");
        printf("\tPIN: The pin to use\n");
        printf("\tDELAY: The delay between bits being sent\n");
        return 1;
    }

    FILE *file = fopen(argv[1], "rb");
    if (file == NULL) {
        printf("Failed to open file\n");
        return 1;
    }

    int pin = atoi(argv[2]);
    float delay_time = atof(argv[3]);

    wiringPiSetup();
    pinMode(pin, OUTPUT);

    int c;
    while ((c = fgetc(file)) != EOF) {
        for (int i = 7; i >= 0; i--) {
            sendBit((c >> i) & 1, pin, delay_time);
        }
    }

    fclose(file);
    return 0;
}
