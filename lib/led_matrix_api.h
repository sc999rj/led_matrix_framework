#ifndef LED_MATRIX_API_H
#define LED_MATRIX_API_H
#include <termios.h>
#include <stdint.h>
#include <stddef.h>

// define commands for the LED matrix
typedef enum {
    LEDMAT_BRIGHTNESS = 0x00,
    LEDMAT_PATTERN = 0x01,
    LEDMAT_BOOTLOADER = 0x02,
    LEDMAT_SLEEP = 0x03,
    LEDMAT_DRAW_BINARY_BITMAP = 0x06,
    LEDMAT_STAGE_COLUMN_GRAYSCALE = 0x07,
    LEDMAT_FLUSH = 0x08,
    LEDMAT_VERSION = 0x20
} LEDMatrixCMD;

typedef enum {
    SLEEP = 1,
    WAKE = 0
} LEDMatrixSleepState;

// LEDMatrix holds the identifier for the device
typedef struct {
    int file_descriptor;
    struct termios communicator;
    char version[512];
} LEDMatrix;


// Functions to interact with the LED matrix
int LEDMatrix_INIT(LEDMatrix* matrix, const char* device);
void LEDMatrix_CLOSE(LEDMatrix* matrix);
int LEDMatrix_send_cmd(LEDMatrix* matrix, uint8_t command, const uint8_t* data, size_t length);
int LEDMatrix_read(LEDMatrix* matrix, uint8_t* buffer);
uint8_t read_bitmap(const uint8_t bitmap[39], size_t col, size_t row) ;
void set_bitmap_entry(uint8_t bitmap[39], size_t col, size_t row, uint8_t value);
#endif // LED_MATRIX_API_H