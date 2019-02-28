#include <sbus_decoder.h>
#include <string.h>
#include <stdio.h>

rc_data_t rc_data;

static void sbus_decoder_get_frame(uint8_t* buf)        //传入一帧数据，解析成各个通道数据，一帧长度必然是25字节
{
    rc_data.rawdata[0]  = ((buf[1]| buf[2]<<8)                 & 0x07FF);
    rc_data.rawdata[1]  = ((buf[2]>>3 |buf[3]<<5)              & 0x07FF);
    rc_data.rawdata[2]  = ((buf[3]>>6 |buf[4]<<2 |buf[5]<<10)  & 0x07FF);
    rc_data.rawdata[3]  = ((buf[5]>>1 |buf[6]<<7)              & 0x07FF);
    rc_data.rawdata[4]  = ((buf[6]>>4 |buf[7]<<4)              & 0x07FF);
    rc_data.rawdata[5]  = ((buf[7]>>7 |buf[8]<<1 |buf[9]<<9)   & 0x07FF);
    rc_data.rawdata[6]  = ((buf[9]>>2 |buf[10]<<6)             & 0x07FF);
    rc_data.rawdata[7]  = ((buf[10]>>5|buf[11]<<3)             & 0x07FF);
    rc_data.rawdata[8]  = ((buf[12]   |buf[13]<<8)             & 0x07FF);
    rc_data.rawdata[9]  = ((buf[13]>>3|buf[14]<<5)             & 0x07FF);
    rc_data.rawdata[10] = ((buf[14]>>6|buf[15]<<2|buf[16]<<10) & 0x07FF);
    rc_data.rawdata[11] = ((buf[16]>>1|buf[17]<<7)             & 0x07FF);
    rc_data.rawdata[12] = ((buf[17]>>4|buf[18]<<4)             & 0x07FF);
    rc_data.rawdata[13] = ((buf[18]>>7|buf[19]<<1|buf[20]<<9)  & 0x07FF);
    rc_data.rawdata[14] = ((buf[20]>>2|buf[21]<<6)             & 0x07FF);
    rc_data.rawdata[15] = ((buf[21]>>5|buf[22]<<3)             & 0x07FF);
    uint8_t i;
    for(i=0;i<16;i++) {
        rc_data.percent[i] = (rc_data.rawdata[i]-340)*100/(1704-340);
    }
    rc_data.flag_refresh = 1;
}

static uint8_t buf[25]={0}; //保存一帧数据
static int8_t p = 0;   //指向下个字节将要保存的位置
void sbus_decoder_get_byte(uint8_t data)
{
    int8_t q;
    buf[p] = data;
    //判断当前缓存是否满足一帧的格式
    q = p+1;
    if(q == 25) {
        q = 0;
    }
    if(buf[p] == 0x00 && buf[q] == 0x0f) {      //当前缓存数据满足0x0f开头且0x00结尾
        uint8_t buf_frame[25] = {0};
        memcpy(buf_frame, buf+q, 25-q);
        memcpy(buf_frame+25-q, buf, q);
        sbus_decoder_get_frame(buf_frame);
    }

    p++;
    if(p == 25) {
        p = 0;
    }
}

void sbus_decoder_get_buf(uint8_t* buf, uint16_t len)
{
    uint16_t i;
    for(i=0;i<len;i++) {
        sbus_decoder_get_byte(buf[i]);
    }
}
