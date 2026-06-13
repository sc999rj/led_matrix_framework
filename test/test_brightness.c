#include "led_matrix_api.h"
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>

int main() {
    LEDMatrix m;

    if (LEDMatrix_INIT(&m, "/dev/ttyACM0") != 0) {
        printf("init failed\n");
        return 1;
    }
    printf("version: %s\n",m.version);
    uint8_t bitmap[39];
    memset(bitmap, 0xFF, sizeof(bitmap));

    uint8_t bright = 0xFF; // max brightness
    LEDMatrix_send_cmd(&m, LEDMAT_FLUSH, NULL, 0);
    LEDMatrix_send_cmd(&m, LEDMAT_DRAW_BINARY_BITMAP, bitmap, sizeof(bitmap));
    LEDMatrix_send_cmd(&m, LEDMAT_BRIGHTNESS, &bright, 1);


    printf("Brightness MAX set. Look at the matrix.\n");
    sleep(2);

    bright = 0x10; // low brightness
    LEDMatrix_send_cmd(&m, LEDMAT_FLUSH, NULL, 0);
    LEDMatrix_send_cmd(&m, LEDMAT_BRIGHTNESS, &bright, 1);
    LEDMatrix_send_cmd(&m, LEDMAT_DRAW_BINARY_BITMAP, bitmap, sizeof(bitmap));

    printf("Brightness LOW set. Look again.\n");
    sleep(2);
    LEDMatrix_send_cmd(&m, LEDMAT_FLUSH, NULL, 0);
    LEDMatrix_CLOSE(&m);
    return 0;
}