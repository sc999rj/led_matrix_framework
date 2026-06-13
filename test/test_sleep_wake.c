#include "led_matrix_api.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
    LEDMatrix m;

    if (LEDMatrix_INIT(&m, "/dev/ttyACM0") != 0)
        return 1;

    uint8_t bitmap[39];
    memset(bitmap, 0xFF, sizeof(bitmap));
    LEDMatrix_send_cmd(&m, LEDMAT_DRAW_BINARY_BITMAP, bitmap, sizeof(bitmap));
    sleep(2);
    uint8_t state = SLEEP;

    printf("Going to SLEEP...\n");
    LEDMatrix_send_cmd(&m, LEDMAT_SLEEP, &state, 1);
    sleep(5);

    state = WAKE;
    printf("WAKING UP...\n");
    LEDMatrix_send_cmd(&m, LEDMAT_SLEEP, &state, 1);

    sleep(5);
    LEDMatrix_send_cmd(&m, LEDMAT_FLUSH, NULL, 0);
    LEDMatrix_CLOSE(&m);
}