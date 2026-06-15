#include <assert.h>
#include <string.h>

#include "led_matrix_api.h"

void test_all_pixels_roundtrip(void)
{
    uint8_t bitmap[39];

    for(size_t r=0;r<34;r++)
    {
        for(size_t c=0;c<9;c++)
        {
            memset(bitmap,0,sizeof(bitmap));

            set_bitmap_entry(bitmap,c,r,1);

            assert(read_bitmap(bitmap,c,r)==1);

            set_bitmap_entry(bitmap,c,r,0);

            assert(read_bitmap(bitmap,c,r)==0);
        }
    }
}

void test_first_bit(void)
{
    uint8_t bitmap[39]={0};

    set_bitmap_entry(bitmap,0,0,1);

    assert(bitmap[0] == 0x01);
}


void test_last_bit(void)
{
    uint8_t bitmap[39]={0};

    set_bitmap_entry(bitmap,8,33,1);

    assert(bitmap[38] & 0x02);
}


void test_oob_reads(void)
{
    uint8_t bitmap[39]={0};

    assert(read_bitmap(bitmap,9,0)==2);
    assert(read_bitmap(bitmap,0,34)==2);
    assert(read_bitmap(bitmap,999,999)==2);
}


void test_value_normalization(void)
{
    uint8_t bitmap[39]={0};

    set_bitmap_entry(bitmap,0,0,255);

    assert(read_bitmap(bitmap,0,0)==1);
}

int main(){
    test_all_pixels_roundtrip();
    test_first_bit();
    test_last_bit();
    test_oob_reads();
    test_value_normalization();

}