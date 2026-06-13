#include "led_matrix_api.h"
#include <stdio.h>
#include <unistd.h>

int main() {
    LEDMatrix m;

    if (LEDMatrix_INIT(&m, "/dev/ttyACM0") != 0)
        return 1;

    uint8_t column[35];
    column[0] = 3;
    for (int i = 1; i < 35; i++) {
        column[i] = i * 7; // gradient intensity
    }

    printf("Sending grayscale column...\n");
    LEDMatrix_send_cmd(&m, LEDMAT_STAGE_COLUMN_GRAYSCALE, column, 35);
    LEDMatrix_send_cmd(&m, LEDMAT_FLUSH, NULL, 0);
    sleep(3);
    LEDMatrix_send_cmd(&m, LEDMAT_FLUSH, NULL, 0);

    LEDMatrix_CLOSE(&m);
}