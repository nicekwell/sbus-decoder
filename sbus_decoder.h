/*
  s-bus的硬件协议和uart相同，但电平相反，可以通过反相器后用uart接收数据。
  使用方法：
  1、把s-bus输出通过反相器后连接到uart接收端。
  1、通过uart接收s-bus数据，uart使用格式为：
     100k波特率，8位数据位，偶校验（Even），2位停止位。
  2、uart接收到的数据通过sbus_decoder_get_byte()函数或者sbus_decoder_get_buf()函数传给此模块。
     模块会解析数据，当解析到数据后会把数据保存在rc_data.rawdata（数据范围0-2047） 和 rc_data.percent（数据范围0-100）中。
  3、数据返回方式采用全局变量，一般只要用一个进程不断读取uart数据并往sbus_decoder模块里送，模块就会不断刷新rc_data，
     在需要获取遥控器数据的地方直接取变量 rc_data.rawdata[i] 或 rc_data.percent[i] 即可。
  说明：
  对于我的天地飞遥控器，rawdata范围是 340-1740，计算percent方式也是按照这个范围计算的，如果其他遥控器范围不同的话则计算方式也不同。
 */
#ifndef __SBUS_DECODER_H__
#define __SBUS_DECODER_H__

#include <main.h>       //stm32 cubemx框架头文件
#include <stdint.h>

typedef struct {
    uint16_t rawdata[16];       //根据sbus协议，一帧包含16通道，每通道11位（0-2047）
    uint8_t percent[16];        //我的天地飞遥控器实际rawdata范围是340-1704，中间值1024，这里还原遥控器的百分比
    uint8_t flag_refresh;  //解析代码每次成功解析数据都会将此变量设为1，选择使用
} rc_data_t;
extern rc_data_t rc_data;

/*
  把串口获取到的数据送给下面函数，该函数会解析数据包，把获取到的数据放到rc_data里。
 */
void sbus_decoder_get_byte(uint8_t data);

/*
  功能和sbus_decoder_get_byte相同，这里直接传入的是buf。
 */
void sbus_decoder_get_buf(uint8_t* buf, uint16_t len);

#endif
