
# 一、概述和s-bus简介

这是遥控器sbus的解码库，用于把s-bus数据还原成遥控器数据。

简单来说s-bus就是用数字信号传输多通道遥控器数据，而且很方便的一点是它的硬件就是基于uart，这使得单片机读取其数据非常方便。  
不同的是其电平和uart相反。


## 数据协议简介

* 大约14ms一帧数据
* 每帧数据25字节，具体格式如下：  
	<START> <data0> <data1> ... <data21> <Flags> <END>  
	其中：  
    * <START>一定是0x0F，<END>一定是0x00。
    * <data0>到<data21>共22字节，表示16个通道的数值，每个通道11位（0-2047）。具体格式这里不介绍，看解码的代码。
* <Flags>这里不讨论，解码时也不关心。

# 二、使用方法

## 1、硬件

接收机一般电压是5v，输出的s-bus信号电平也是5v。  
注意！！s-bus的电平和uart串口电平相反，一般加下面这个反相器可同时完成反相和电平转换
：

![1.jpg](https://github.com/nicekwell/sbus-decoder/raw/master/img/1.jpg)

## 2、串口配置

s-bus的uart采用：100k波特率，8位数据位，偶校验，2位停止位，无流控。
stm32cubemx里配置如下：

![2.png](https://github.com/nicekwell/sbus-decoder/raw/master/img/2.png)

## 解码库的使用

见代码注释：

```c
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
```



