#include "led_matrix_api.h"
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

int LEDMatrix_INIT(LEDMatrix *matrix, const char *device)
{
    matrix->file_descriptor = open(device, O_RDWR | O_NOCTTY);
    if (matrix->file_descriptor == -1)
    {
        return -1; // Failed to open the device
    }

    if (tcgetattr(matrix->file_descriptor, &matrix->communicator) != 0)
        return -1;
    cfsetispeed(&matrix->communicator, B115200);
    cfsetospeed(&matrix->communicator, B115200);
    matrix->communicator.c_cflag |= (CLOCAL | CREAD);
    matrix->communicator.c_cflag &= ~PARENB;
    matrix->communicator.c_cflag &= ~CSTOPB;
    matrix->communicator.c_cflag &= ~CSIZE;
    matrix->communicator.c_cflag |= CS8;
    matrix->communicator.c_cc[VMIN] = 0;
    matrix->communicator.c_cc[VTIME] = 1;
    cfmakeraw(&matrix->communicator);
    if (tcsetattr(matrix->file_descriptor, TCSANOW, &matrix->communicator) != 0)
        return -1;
    tcflush(matrix->file_descriptor, TCIOFLUSH);
    // store version information
    if(LEDMatrix_send_cmd(matrix, LEDMAT_VERSION, NULL, 0) == -1) printf("failed to send version request\n");
    uint8_t version_buf[32];
    if (LEDMatrix_read(matrix, version_buf) == -1)
    {
        printf("failed to receive version response\n");
        return -1;
    }
    uint8_t major = version_buf[0];
    uint8_t minor = (version_buf[1] & 0xF0) >> 4;
    uint8_t patch = version_buf[1] & 0x0F;
    uint8_t pre_release = version_buf[2] == 1;
    snprintf(matrix->version, sizeof(matrix->version), "%d.%d.%d%s", major, minor, patch, pre_release ? "-pre" : "");
    return 0; // Success
}

void LEDMatrix_CLOSE(LEDMatrix *matrix)
{
    if (matrix->file_descriptor != -1)
    {
        close(matrix->file_descriptor);
        matrix->file_descriptor = -1;
    }
}

int LEDMatrix_send_cmd(LEDMatrix *matrix, uint8_t command, const uint8_t *data, size_t length)
{
    uint8_t buffer[64];
    if (length > 61)
        return -1; // data too long
    buffer[0] = 0x32;
    buffer[1] = 0xAC;
    buffer[2] = (uint8_t)command;
    if (data && length > 0)
        memcpy(buffer + 3, data, length);

    // blocking write
    size_t total = length + 3;

    ssize_t n = write(matrix->file_descriptor,
                          buffer,
                          total);

    if (n != total)
        return -1;
    return 0;
}

int LEDMatrix_read(LEDMatrix *matrix, uint8_t *buff)
{
    uint8_t version_buf[32];
    ssize_t n = read(matrix->file_descriptor, version_buf, 32);
    if (n != 32)
        return -1;
    memcpy(buff, version_buf, 32);
    return 0;
}

uint8_t read_bitmap(const uint8_t bitmap[39], size_t col, size_t row)
{
    size_t i = col + 9 * row;
    size_t byte = i / 8;
    size_t bit = i % 8;
    if (i >= 306)
        return 2; // out of bounds
    return (bitmap[byte] >> bit) & 0x01;
}

void set_bitmap_entry(uint8_t bitmap[39], size_t col, size_t row, uint8_t value)
{
    size_t i = col + 9 * row;
    size_t byte = i / 8;
    size_t bit = i % 8;
    if (i >= 306)
        return; // out of bounds
    bitmap[byte] = (bitmap[byte] & ~(1 << bit)) | ((value & 0x01) << bit);
}
