### Arduino打造USB蓝牙键盘扩展器

### 基于arduino的键盘扩展器，将USB键盘插在扩展器上后，USB键盘有蓝牙键盘功能，实现可修改键盘布局（Dovrak）、调整大写锁定与Ctrl位置并在键盘上集成了鼠标功能

---

> 制作原因：因为爱折腾所以尝试使用了`Dvorak`键盘布局，结果太爽了，于是就变成了现在这个使用两种键盘布局的XXX了：输入英文使用`Dvorak`布局、输入中文使用`Qwerty`布局（因为使用`Dvorak`输入中文特别别扭）<br><br>
但是感觉在电脑上切换键盘布局不太爽，尤其是Linux下，于是就做了这个键盘扩展器，顺带实现了将USB键盘扩展为蓝牙键盘并集成了鼠标功能。

> 当时制作的时候主要参考的是这里的做法：http://www.lab-z.com/ardusb2bt/<br>
但是使用的硬件除了蓝牙都是不同的，因为我需要的是一个小巧的，可以随身携带的。

> 2016年12月27日更新：<br><br>
在开始制作的时候就写了一篇介绍博客，本来是打算做好了后详细更新一篇博客的，结果那段时间有点忙就只更新了需要的硬件，后来博客迁移了，就没把文章迁过来。没想到在这里留下坑了。。。所以现在把GitHub仓库里的README更新一下，把坑补上。虽然更新后的结果百度搜不到，但是谷歌可以，相信爱折腾的朋友肯定会用谷歌。<br><br>
另外USB Host Shield mini的官方文档虽然内容的确比较少，英文阅读起来也有点费劲，改天有空了，我把官方说明翻译下，补充点信息，尽量少点坑^_^<br><br>
感谢@caoczy 提出了问题：https://github.com/KevinsBobo/arduino_keyboard/issues/1 在我们的交流中也有很多细节问题，想入坑的朋友可以参考下。


### 硬件部分：
1. `Arduino Micro` 作用：模拟USB键盘
2. `Arduino Pro mini 改进版 ATMEGA328P 5V 16M` 作用：从`USB Host Shield mini`接收USB键盘按键信息，做以处理并选择通过`Arduino Micor`输出或通过蓝牙模块输出。
3. `USB Host Shield mini` 作用：接收USB键盘按键信息
4. `XM-04-HID-K 蓝牙键盘模块` 作用：实现HID蓝牙键盘功能。PS：这个是某宝的一家店里的，而且没有店铺里没有上架这一个模块，可以拍下`XM-04-HID-S 扫描枪模块`然后告诉老板需要蓝牙键盘模块就行了。

### 程序说明：
USBHIDBootKbd代码是用于 Arduino Mini 上获取按键和处理输出。
获取键盘按键后根据按键和大写锁定切换情况向 Arduino Micro 或蓝牙模块输出数据。

KeyboardAndMouseControl代码用于 Arduino Micro 上，接收 Arduino Mini 传来的键位数据，然后做以处理向电脑输出。

### Arduino pro mini & USB Host Shield mini 焊接方式
<img src="https://raw.githubusercontent.com/KevinsBobo/arduino_keyboard/master/pro_mini_usb_host_焊接方式.jpg" whdth="100%"/>

这是我的接法，官方手册中说可以直接把USB Host的USB电压输出直接接到上图中右下角的RAW针脚，因为这个针脚可以接受5V的供电，这样就不用提供3.3V的供电了（但VCC针脚应该还是不能接到一起，因为VCC针脚只能接受3.3V的供电）。

还有一些细节和坑可以参考这里：https://github.com/KevinsBobo/arduino_keyboard/issues/1

~~详细功能说明见 http://kevins.pro/arduino_keyboard/~~ （博客迁移，文章没有迁过来。）

---

### Author : KevinsBobo
### Email  : kevins.bobo@gmail.com
### Website: http://kevins.pro/
