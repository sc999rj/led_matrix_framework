#include "led_matrix_api.h"
#include <unistd.h>
#include <assert.h>

extern unsigned char mock_read_buffer[32];
extern ssize_t mock_read_result;

void test_read_success(void)
{
    LEDMatrix m;

    uint8_t out[32];

    for(int i=0;i<32;i++)
        mock_read_buffer[i]=i;

    mock_read_result = 32;

    assert(LEDMatrix_read(&m,out)==0);

    for(int i=0;i<32;i++)
        assert(out[i]==i);
}

void test_read_short(void)
{
    LEDMatrix m;

    uint8_t out[32];

    mock_read_result = 31;

    assert(LEDMatrix_read(&m,out)==-1);
}

int main(){
    test_read_success();
    test_read_short();
}
