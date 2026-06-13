#include "led_matrix_api.h"
#include <stdio.h>
#include <unistd.h>

int main() {
    LEDMatrix m;

    if (LEDMatrix_INIT(&m, "/dev/ttyACM0") != 0) {
        printf("init failed\n");
        return 1;
    }

    printf("Device version: %s\n", m.version);

    LEDMatrix_CLOSE(&m);
}