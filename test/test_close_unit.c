#include "led_matrix_api.h"
#include <assert.h>

extern int mock_close_called;

void test_close(void)
{
    LEDMatrix m;

    m.file_descriptor = 123;

    mock_close_called = 0;

    LEDMatrix_CLOSE(&m);

    assert(mock_close_called == 123);
    assert(m.file_descriptor == -1);
}

int main(){
    test_close();
}