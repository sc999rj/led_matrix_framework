#include "led_matrix_api.h"
#include <stdio.h>
#include <unistd.h>

int main() {
    LEDMatrix m;

    if (LEDMatrix_INIT(&m, "/dev/ttyACM0") != 0)
        return 1;

    uint8_t pattern = 0x01;
    LEDMatrix_send_cmd(&m, LEDMAT_PATTERN, &pattern, 1);

    printf("Pattern 1 displayed\n");
    sleep(5);

    pattern = 0x02;
    LEDMatrix_send_cmd(&m, LEDMAT_PATTERN, &pattern, 1);

    printf("Pattern 2 displayed\n");
    sleep(5);

    LEDMatrix_CLOSE(&m);
}