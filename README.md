### 基于arduino的键盘扩展器，将USB键盘插在扩展器上后，实现可修改键盘布局、调整大写锁定与Ctrl位置并在键盘上集成了鼠标功能

---

### 程序说明：
USBHIDBootKbd代码是用于 Arduino Mini 上获取按键和处理输出。
获取键盘按键后根据按键和大写锁定切换情况向 Arduino Micro 或蓝牙模块输出数据。

KeyboardAndMouseControl代码用于 Arduino Micro 上，接收 Arduino Mini 传来的键位数据，然后做以处理向电脑输出。

Arduino pro mini & USB Host Shield mini 焊接方式
<img src="https://raw.githubusercontent.com/KevinsBobo/arduino_keyboard/master/pro_mini_usb_host_焊接方式.jpg" whdth="100%"/>

这是我的接法，官方手册中说可以直接把USB Host的USB电压输出直接接到上图中右下角的RAW针脚，因为这个针脚可以接受5V的供电，这样就不用提供3.3V的供电了（但VCC针脚应该还是不能接到一起，因为VCC针脚只能接受3.3V的供电）。

~~详细功能说明见 http://kevins.pro/arduino\_keyboard/~~ （博客迁移，文章没有迁过来。）

---

### Author : KevinsBobo
### Email  : kevins.bobo@gmail.com
### Website: http://kevins.pro/
