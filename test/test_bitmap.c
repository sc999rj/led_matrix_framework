#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "led_matrix_api.h"

int main(void)
{
    uint8_t bitmap[39];
    memset(bitmap, 0, sizeof(bitmap));

    LEDMatrix lm;
    assert(LEDMatrix_INIT(&lm, "/dev/ttyACM0") == 0);
    printf("LED Matrix version: %s\n", lm.version);

    /* Test setting a bit */
    set_bitmap_entry(bitmap, 0, 0, 1);
    assert(read_bitmap(bitmap, 0, 0) == 1);

    assert(LEDMatrix_send_cmd(&lm, LEDMAT_DRAW_BINARY_BITMAP, bitmap, sizeof(bitmap)) == 0);
    sleep(2);

    /* Test clearing a bit */
    set_bitmap_entry(bitmap, 0, 0, 0);
    assert(read_bitmap(bitmap, 0, 0) == 0);

    assert(LEDMatrix_send_cmd(&lm, LEDMAT_DRAW_BINARY_BITMAP, bitmap, sizeof(bitmap)) == 0);
    sleep(2);
    /* Test last valid LED */
    set_bitmap_entry(bitmap, 8, 33, 1);
    assert(read_bitmap(bitmap, 8, 33) == 1);

    assert(LEDMatrix_send_cmd(&lm, LEDMAT_DRAW_BINARY_BITMAP, bitmap, sizeof(bitmap)) == 0);
    sleep(2);

    /* Test neighboring LEDs don't change */
    assert(read_bitmap(bitmap, 7, 33) == 0);
    assert(read_bitmap(bitmap, 8, 32) == 0);

    /* Test out-of-bounds reads */
    assert(read_bitmap(bitmap, 9, 33) == 2);
    assert(read_bitmap(bitmap, 8, 34) == 2);

    //test all on 
    memset(bitmap, 0xFF, sizeof(bitmap));
    assert(LEDMatrix_send_cmd(&lm, LEDMAT_DRAW_BINARY_BITMAP, bitmap, sizeof(bitmap)) == 0);
    sleep(2);

    memset(bitmap, 0, sizeof(bitmap));

    // draw diagonal line
    for (int i = 0; i < 9; i++) {
        set_bitmap_entry(bitmap, i, i, 1);
    }
    LEDMatrix_send_cmd(&lm, LEDMAT_DRAW_BINARY_BITMAP, bitmap, sizeof(bitmap));
    sleep(2);

    assert(LEDMatrix_send_cmd(&lm, LEDMAT_FLUSH, NULL, 0) == 0);
    printf("Flush command sent successfully.\n");
    sleep(2);

    LEDMatrix_CLOSE(&lm);

    return 0;
}