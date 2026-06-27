#ifndef LED_MATRIX_API_H
#define LED_MATRIX_API_H

#ifdef __cplusplus
extern "C" {
#endif

#include <termios.h>
#include <stdint.h>
#include <stddef.h>

/**
 * @file led_matrix_api.h
 * @brief Serial communication library for controlling a LED matrix device.
 *
 * This library provides functions for initializing communication with a LED
 * matrix controller, sending commands, receiving responses, and manipulating
 * bitmap data used by the display.
 */


/**
 * @brief Commands supported by the LED matrix firmware.
 */
typedef enum {
     /** Set display brightness. */
    LEDMAT_BRIGHTNESS = 0x00,
    /** Display a predefined pattern. */
    LEDMAT_PATTERN = 0x01,
    /** Enter bootloader mode. */
    LEDMAT_BOOTLOADER = 0x02,
    /** Change sleep state. Use LEDMatrixSleepState to set the data*/
    LEDMAT_SLEEP = 0x03,
    /** Draw a binary bitmap to the display. Does not need flushing. 
     * Data is a uint8_t[39] bitmap, i.e. 39 Bytes of which the first 306 bits are used (1 bit per LED)  */
    LEDMAT_DRAW_BINARY_BITMAP = 0x06,
    /** Stage grayscale values for a column. Data is a uint8_t[35], 
     * 1st Byte for column index and 34 Byte fot grayscale data for each LED in the column*/
    LEDMAT_STAGE_COLUMN_GRAYSCALE = 0x07,
    /** Flush staged grayscale column data to the display. */
    LEDMAT_FLUSH = 0x08,
    /** Request firmware version information. */
    LEDMAT_VERSION = 0x20
} LEDMatrixCMD;

/**
 * @brief Sleep state values for the LED matrix.
 */
typedef enum {
    SLEEP = 1,
    WAKE = 0
} LEDMatrixSleepState;

/**
 * @brief Represents a connection to a LED matrix device.
 *
 * This structure stores the serial file descriptor, terminal
 * communication settings, and firmware version string.
 */
typedef struct {
    /** File descriptor associated with the serial device. */
    int file_descriptor;
    /** Configured serial communication parameters. */
    struct termios communicator;
    /** Firmware version string returned by the device. */
    char version[512];
} LEDMatrix;


/**
 * @brief Initialize communication with a LED matrix device.
 *
 * Opens the specified serial device, configures it for raw
 * communication at 115200 baud, and queries the firmware version.
 *
 * @param matrix Pointer to a LEDMatrix structure to initialize.
 * @param device Path to the serial device (e.g. "/dev/ttyACM0").
 *
 * @return 0 on success, -1 on failure.
 */
int LEDMatrix_INIT(LEDMatrix* matrix, const char* device);


/**
 * @brief Close communication with the LED matrix.
 *
 * Closes the serial device associated with the matrix and invalidates
 * the file descriptor.
 *
 * @param matrix Pointer to an initialized LEDMatrix structure.
 */
void LEDMatrix_CLOSE(LEDMatrix* matrix);

/**
 * @brief Send a command packet to the LED matrix.
 *
 * Packets are transmitted in the format:
 *
 * @code
 * [0x32][0xAC][COMMAND][DATA...]
 * @endcode
 *
 * The maximum payload size is 61 bytes.
 *
 * @param matrix Pointer to an initialized LEDMatrix structure.
 * @param command Command identifier.
 * @param data Pointer to payload data. May be NULL when length is 0.
 * @param length Number of payload bytes.
 *
 * @return 0 on success, -1 on failure.
 */
int LEDMatrix_send_cmd(LEDMatrix* matrix, uint8_t command, const uint8_t* data, size_t length);

/**
 * @brief Read a 32-byte response packet from the LED matrix.
 *
 * This function blocks until data is received or the serial timeout
 * configured by the driver expires.
 *
 * @param matrix Pointer to an initialized LEDMatrix structure.
 * @param buffer Destination buffer. Must have room for at least 32 bytes.
 *
 * @return 0 on success, -1 on failure.
 */
int LEDMatrix_read(LEDMatrix* matrix, uint8_t* buffer);

/**
 * @brief Read a pixel value from a packed binary bitmap.
 *
 * The bitmap contains 306 pixels arranged as a 9×34 display and stored
 * in row-major order. Pixels are packed into 39 bytes.
 *
 * @param bitmap Packed bitmap data.
 * @param col Column index.
 * @param row Row index.
 *
 * @return
 * - 0 if the pixel is cleared.
 * - 1 if the pixel is set.
 * - 2 if the coordinates are out of bounds.
 */
uint8_t read_bitmap(const uint8_t bitmap[39], size_t col, size_t row) ;

/**
 * @brief Set a pixel value in a packed binary bitmap.
 * 
 * The bitmap contains 306 pixels arranged as a 9×34 display and stored
 * in row-major order. Pixels are packed into 39 bytes.
 *
 * Values other than 0 are treated as 1.
 *
 * @param bitmap Packed bitmap data to modify.
 * @param col Column index.
 * @param row Row index.
 * @param value Pixel value (0 or 1).
 */
void set_bitmap_entry(uint8_t bitmap[39], size_t col, size_t row, uint8_t value);

/**
 * @brief set bitmap value for text rendering
 */
void set_bitmap_string(uint8_t bitmap[39], char str[], float scale );

#ifdef __cplusplus
}
#endif

#endif // LED_MATRIX_API_H