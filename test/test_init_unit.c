#include "led_matrix_api.h"
#include <unistd.h>
#include <assert.h>
#include <string.h>


extern int mock_open_result;
extern unsigned char mock_read_buffer[32];
extern ssize_t mock_read_result;
extern ssize_t mock_write_result;
extern int mock_tcsetattr_result;

void test_init_open_failure(void)
{
    LEDMatrix m;

    mock_open_result = -1;

    assert(
        LEDMatrix_INIT(
            &m,
            "/dev/fake") == -1);
}

extern int mock_open_result;
extern int mock_tcgetattr_result;

void test_init_tcgetattr_failure(void)
{
    LEDMatrix m;

    mock_open_result = 10;
    mock_tcgetattr_result = -1;

    assert(
        LEDMatrix_INIT(
            &m,
            "/dev/fake") == -1);
}

void test_version_parsing(void)
{
    LEDMatrix m;

    mock_open_result = 10;
    mock_tcgetattr_result = 0;
    mock_tcsetattr_result = 0;

    mock_write_result = 3;

    memset(mock_read_buffer,0,32);

    mock_read_buffer[0] = 2;
    mock_read_buffer[1] = (5 << 4) | 7;
    mock_read_buffer[2] = 1;

    mock_read_result = 32;

    assert(
        LEDMatrix_INIT(
            &m,
            "/dev/fake") == 0);

    assert(strcmp(m.version,"2.5.7-pre")==0);
}

int main(){
    test_init_open_failure();
    test_init_tcgetattr_failure();
    test_version_parsing();
}

