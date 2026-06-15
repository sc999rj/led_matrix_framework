#include "led_matrix_api.h"
#include <unistd.h>
#include <assert.h>

extern unsigned char last_write[64];
extern size_t last_write_size;
extern ssize_t mock_write_result;

void test_send_cmd_packet(void)
{
    LEDMatrix m;
    m.file_descriptor = 5;

    uint8_t payload[] = {0x11,0x22};

    mock_write_result = 5;

    assert(
        LEDMatrix_send_cmd(
            &m,
            LEDMAT_PATTERN,
            payload,
            2) == 0);

    assert(last_write_size == 5);

    assert(last_write[0] == 0x32);
    assert(last_write[1] == 0xAC);
    assert(last_write[2] == LEDMAT_PATTERN);
    assert(last_write[3] == 0x11);
    assert(last_write[4] == 0x22);
}

void test_send_cmd_oversized(void)
{
    LEDMatrix m;

    uint8_t payload[62];

    assert(
        LEDMatrix_send_cmd(
            &m,
            1,
            payload,
            62) == -1);
}

void test_send_cmd_partial_write(void)
{
    LEDMatrix m;
    m.file_descriptor = 1;

    uint8_t payload[4]={0};

    mock_write_result = 3;

    assert(
        LEDMatrix_send_cmd(
            &m,
            1,
            payload,
            4) == -1);
}

int main(){
    test_send_cmd_oversized();
    test_send_cmd_packet();
    test_send_cmd_partial_write();
}

