/*
*
*  Author  : KevinsBobo
*  Email   : kevins.bobo@gmail.com
*  Website : http://kevins.pro/
*
*/

/*************************************************************

程序说明：
	
	本段代码是用于 Arduino Mini 上获取按键和处理输出。

	获取键盘按键后根据按键和大写锁定切换情况向 Arduino Micro
	或蓝牙模块输出数据。

	详细功能说明见 http://kevins.pro/arduino_keyboard/

*************************************************************/

#include <hidboot.h>
#include <usbhub.h>
#include <SoftwareSerial.h>

SoftwareSerial Serial1(2, 3); // RX, TX (软件模拟串口，供蓝牙使用)

int btdvorakbm[] = { 0, 0, 0, 0, 4, 27, 13, 8, 55, 24, 12, 7, 6, 11, 23, 17, 16, 5, 21, 15, 52, 19, 18, 28, 10, 14, 54, 20, 9, 51, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 47, 48, 56, 46, 49, 49, 22, 45, 53, 26, 25, 29}; // dvorak切换数组

int changecap = 0; // 大写锁定键切换判断，0 未切换，1 已切换
int capchanged = 0; // 大写锁定切换辅助，防止重复切换
int changedvo = 0; // dvorak切换判断
int bmispress[9] = { 0 }; // 功能键按下判断
int capispress = 0; // 大写锁定按下判断
int capnothing = 0; // 大写锁定按下后是否按下其他键判断
int arrowctrl = 0; // 方向控制键按下判断
int swich = 0; // 键盘输出模式，0 仅USB，1 仅蓝牙，2 共同输出，3 不输出
int capcannotup[3] = { 0 }; // 在大写锁定键按下期间按下其他键时值为1
int capnotup = 0; // 在大写锁定键按下期间按下其他键时值为1
int btwid = 0x00; // 蓝牙HID数据长度
int btkeymode = 0x00; // 蓝牙HID数据模式
int btctrlbm = 0x00; // 蓝牙HID控制键
int btkey[4] = { 0x00 }; // 蓝牙HID普通键

class KbdRptParser : public KeyboardReportParser
{
        void PrintKey(uint8_t mod, uint8_t key); // 获取键盘按键后执行输出函数
		void AddBtbm(int addbtwid, int addbtkeymode, int addctrlbm, int addbtbm, int addpost, int addormin); // 蓝牙HID输出控制函数

protected:
        virtual void OnControlKeysChanged(uint8_t before, uint8_t after);

	virtual void OnKeyDown	(uint8_t mod, uint8_t key);
	virtual void OnKeyUp	(uint8_t mod, uint8_t key);
};

void KbdRptParser::PrintKey(uint8_t m, uint8_t key)
{
    MODIFIERKEYS mod;
    *((uint8_t*)&mod) = m;
	int keyp;
	if(capispress == 1 || (changecap == 0 && key == 57)){
		if(key == 57){
			capispress = 1;
			capnothing = 1;
		}else if(key == 182){
			if(capnothing == 1){
				if(swich != 3){
					Serial.write(57);
					delay(10);
					Serial.write(182);
				}
				if(swich == 1 || swich == 2){
					AddBtbm(0x0C, 0x01, 0, 57, 0, 1);
					delay(10);
					AddBtbm(0x0C, 0x01, 0, 57, 0, 0);
				}
				capnothing = 0;
			}
			if(capcannotup[0] == 0) capispress = 0;
			else capnotup = 1;
		}else if(capispress == 1){
			if(key == 39){
				changedvo = 1 - changedvo;
				Serial.write(110);
			}else if(key == 45){
				if(changecap == 0) changecap = 1;
				else capchanged = 1;
			}else if(key == 35){
				if(swich != 0){
					swich = 0;
					Serial.write((byte)0);
				}
			}else if(key == 36){
				if(swich != 1){
					swich = 1;
					Serial.write(1);
				}
			}else if(key == 37){
				if(swich != 2){
					swich = 2;
					Serial.write(2);
				}
			}else if(key == 38){
				if(swich != 3){
					swich = 3;
					Serial.write(3);
				}
			}else if(key == 25){
				if(swich == 0 || swich == 2) Serial.write(121);
				arrowctrl = 1;
			}else if(key == 150){
				if(swich == 0 || swich == 2) Serial.write(246);
				arrowctrl = 0;
			}else if(key == 22){
				if(swich == 0 || swich == 2) Serial.write(111);
			}else if(key == 147){
				if(swich == 0 || swich == 2) Serial.write(236);
			}else if(key == 7){
				if(swich == 0 || swich == 2) Serial.write(113);
			}else if(key == 132){
				if(swich == 0 || swich == 2) Serial.write(238);
			}else if(key == 9){
				if(swich == 0 || swich == 2) Serial.write(112);
			}else if(key == 134){
				if(swich == 0 || swich == 2) Serial.write(237);
			}else if(key == 12){
				if(swich == 0 || swich == 2) Serial.write(114);
				if(arrowctrl == 1 && (swich == 1 || swich == 2)) AddBtbm(0x0C, 0x01, 0, 82, 0, 1);
			}else if(key == 137){
				if(swich == 0 || swich == 2) Serial.write(239);
				if(arrowctrl == 1 && (swich == 1 || swich == 2)) AddBtbm(0x0C, 0x01, 0, 82, 0, 0);
			}else if(key == 13){
				if(swich == 0 || swich == 2) Serial.write(115);
				if(arrowctrl == 1 && (swich == 1 || swich == 2)) AddBtbm(0x0C, 0x01, 0, 80, 0, 1);
			}else if(key == 138){
				if(swich == 0 || swich == 2) Serial.write(240);
				if(arrowctrl == 1 && (swich == 1 || swich == 2)) AddBtbm(0x0C, 0x01, 0, 80, 0, 0);
			}else if(key == 14){
				if(swich == 0 || swich == 2) Serial.write(116);
				if(arrowctrl == 1 && (swich == 1 || swich == 2)) AddBtbm(0x0C, 0x01, 0, 81, 0, 1);
			}else if(key == 139){
				if(swich == 0 || swich == 2) Serial.write(241);
				if(arrowctrl == 1 && (swich == 1 || swich == 2)) AddBtbm(0x0C, 0x01, 0, 81, 0, 0);
			}else if(key == 15){
				if(swich == 0 || swich == 2) Serial.write(117);
				if(arrowctrl == 1 && (swich == 1 || swich == 2)) AddBtbm(0x0C, 0x01, 0, 79, 0, 1);
			}else if(key == 140){
				if(swich == 0 || swich == 2) Serial.write(242);
				if(arrowctrl == 1 && (swich == 1 || swich == 2)) AddBtbm(0x0C, 0x01, 0, 79, 0, 0);
			}else if(key == 16){
				if(swich == 0 || swich == 2) Serial.write(118);
				if(arrowctrl == 1 && (swich == 1 || swich == 2)) AddBtbm(0x0C, 0x01, 0, 78, 0, 1);
			}else if(key == 141){
				if(swich == 0 || swich == 2) Serial.write(243);
				if(arrowctrl == 1 && (swich == 1 || swich == 2)) AddBtbm(0x0C, 0x01, 0, 78, 0, 0);
			}else if(key == 54){
				if(swich == 0 || swich == 2) Serial.write(119);
			}else if(key == 179){
				if(swich == 0 || swich == 2) Serial.write(244);
			}else if(key == 55){
				if(swich == 0 || swich == 2) Serial.write(120);
				if(arrowctrl == 1 && (swich == 1 || swich == 2)) AddBtbm(0x0C, 0x01, 0, 75, 0, 1);
			}else if(key == 180){
				if(swich == 0 || swich == 2) Serial.write(245);
				if(arrowctrl == 1 && (swich == 1 || swich == 2)) AddBtbm(0x0C, 0x01, 0, 75, 0, 0);
			}else if((swich == 1 || swich == 2) && key == 58){
				AddBtbm(0x08, 0x03, 0, 64, 5, 1);

			}else if((swich == 1 || swich == 2) && key == 183){
				AddBtbm(0x08, 0x03, 0, 64, 5, 0);

			}else if((swich == 1 || swich == 2) && key == 59){
				AddBtbm(0x08, 0x03, 0, 2, 5, 1);

			}else if((swich == 1 || swich == 2) && key == 184){
				AddBtbm(0x08, 0x03, 0, 2, 5, 0);

			}else if((swich == 1 || swich == 2) && key == 60){
				AddBtbm(0x08, 0x03, 0, 16, 5, 1);

			}else if((swich == 1 || swich == 2) && key == 185){
				AddBtbm(0x08, 0x03, 0, 16, 5, 0);

			}else if((swich == 1 || swich == 2) && key == 61){
				AddBtbm(0x08, 0x03, 0, 1, 7, 1);

			}else if((swich == 1 || swich == 2) && key == 186){
				AddBtbm(0x08, 0x03, 0, 1, 7, 0);

			}else if((swich == 1 || swich == 2) && key == 62){
				AddBtbm(0x08, 0x03, 0, 8, 5, 1);

			}else if((swich == 1 || swich == 2) && key == 187){
				AddBtbm(0x08, 0x03, 0, 8, 5, 0);

			}else if((swich == 1 || swich == 2) && key == 63){
				AddBtbm(0x08, 0x03, 0, 2, 6, 1);

			}else if((swich == 1 || swich == 2) && key == 188){
				AddBtbm(0x08, 0x03, 0, 2, 6, 0);

			}else if((swich == 1 || swich == 2) && key == 64){
				AddBtbm(0x08, 0x03, 0, 4, 6, 1);

			}else if((swich == 1 || swich == 2) && key == 189){
				AddBtbm(0x08, 0x03, 0, 4, 6, 0);

			}else if((swich == 1 || swich == 2) && key == 65){
				AddBtbm(0x08, 0x03, 0, 1, 6, 1);

			}else if((swich == 1 || swich == 2) && key == 190){
				AddBtbm(0x08, 0x03, 0, 1, 6, 0);

			}else if((swich == 1 || swich == 2) && key == 41){
				AddBtbm(0x08, 0x03, 0, 1, 5, 1);

			}else if((swich == 1 || swich == 2) && key == 166){
				AddBtbm(0x08, 0x03, 0, 1, 5, 0);

			}else if((swich == 1 || swich == 2) && key == 76){
				AddBtbm(0x08, 0x03, 0, 1, 8, 1);

			}else if((swich == 1 || swich == 2) && key == 201){
				AddBtbm(0x08, 0x03, 0, 1, 8, 0);

			}

			if(key >= 4 && key <= 101){
				capnothing = 0;
				for(int i = 0; i < 3; i++){
					if(capcannotup[i] == 0){
						capcannotup[i] = 1;
						break;
					}
				}
			}

			if(key >= 129 && key <= 226){
				capnothing = 0;
				for(int i = 2; i >= 0; i--){
					if(capcannotup[i] == 1){
						capcannotup[i] = 0;
						break;
					}
				}
				if(capnotup == 1 && capcannotup[0] == 0){
					capispress = 0;
					capnotup = 0;
				}
			}
		}
	}else{
		if(swich == 0 || swich == 2){
			if((key == 57 || key == 182) && changecap == 1){
				key += 45;
			}
			Serial.write(key);
		}

		if(swich == 1 || swich == 2){
			if(key <= 109 && key >= 4){
				if(key == 57 && changecap == 1) key = 224;
				else if(changedvo == 1 && ((key >= 4 && key <= 29) || (key >= 45 && key <= 56))) key = btdvorakbm[key];
				AddBtbm(0x0C, 0x01, 0, key, 0, 1);
			}else if(key <= 234 && key >= 129){
				key -= 125;
				if(key == 57 && changecap == 1) key = 224;
				else if(changedvo == 1 && ((key >= 4 && key <= 29) || (key >= 45 && key <= 56))) key = btdvorakbm[key];
				AddBtbm(0x0C, 0x01, 0, key, 0, 0);
			}
		}
	}
};

void KbdRptParser::OnKeyDown(uint8_t mod, uint8_t key)
{
    PrintKey(mod, key);
}

void KbdRptParser::OnControlKeysChanged(uint8_t before, uint8_t after) {

    MODIFIERKEYS beforeMod;
    *((uint8_t*)&beforeMod) = before;

    MODIFIERKEYS afterMod;
    *((uint8_t*)&afterMod) = after;

    if (beforeMod.bmLeftCtrl != afterMod.bmLeftCtrl) {
		if(changecap == 1){
			int keyp = (bmispress[8] == 1) ? 182 : 57;
			if(keyp == 57){
				capispress = 1;
				capnothing = 1;
				bmispress[8] = 1 - bmispress[8];
			}else if(keyp == 182){
				if(capcannotup[0] == 0) capispress = 0;
				else capnotup = 1;

				bmispress[8] = 1 - bmispress[8];
				if(capnothing == 1){
					if(swich != 3){
						Serial.write(57);
						delay(10);
						Serial.write(182);
					}
					if(swich == 1 || swich == 2){
						AddBtbm(0x0C, 0x01, 0, 57, 0, 1);
						delay(10);
						AddBtbm(0x0C, 0x01, 0, 57, 0, 0);
					}
					capnothing = 0;
				}else if(capchanged == 1){
					changecap = 0;
					capchanged = 0;
				}
			}

		}else{
			if(swich == 0 || swich == 2) Serial.write((bmispress[0] == 1) ? 227 : 102);
			if(swich == 1 || swich == 2) AddBtbm(0x0C, 0x01, 1, 0, 0, (bmispress[0] == 1) ? 0 : 1);
			bmispress[0] = 1 - bmispress[0];
		}
    }
    if (beforeMod.bmLeftShift != afterMod.bmLeftShift) {
        if(swich == 0 || swich == 2) Serial.write((bmispress[1] == 1) ? 228 : 103);
		if(swich == 1 || swich == 2) AddBtbm(0x0C, 0x01, 2, 0, 0, (bmispress[1] == 1) ? 0 : 1);
		bmispress[1] = 1 - bmispress[1];
    }
    if (beforeMod.bmLeftAlt != afterMod.bmLeftAlt) {
        if(swich == 0 || swich == 2) Serial.write((bmispress[2] == 1) ? 229 : 104);
		if(swich == 1 || swich == 2) AddBtbm(0x0C, 0x01, 4, 0, 0, (bmispress[2] == 1) ? 0 : 1);
		bmispress[2] = 1 - bmispress[2];
    }
    if (beforeMod.bmLeftGUI != afterMod.bmLeftGUI) {
        if(swich == 0 || swich == 2) Serial.write((bmispress[3] == 1) ? 230 : 105);
		if(swich == 1 || swich == 2) AddBtbm(0x0C, 0x01, 8, 0, 0, (bmispress[3] == 1) ? 0 : 1);
		bmispress[3] = 1 - bmispress[3];
    }

    if (beforeMod.bmRightCtrl != afterMod.bmRightCtrl) {
        if(swich == 0 || swich == 2) Serial.write((bmispress[4] == 1) ? 231 : 106);
		if(swich == 1 || swich == 2) AddBtbm(0x0C, 0x01, 16, 0, 0, (bmispress[4] == 1) ? 0 : 1);
		bmispress[4] = 1 - bmispress[4];
    }
    if (beforeMod.bmRightShift != afterMod.bmRightShift) {
        if(swich == 0 || swich == 2) Serial.write((bmispress[5] == 1) ? 232 : 107);
		if(swich == 1 || swich == 2) AddBtbm(0x0C, 0x01, 32, 0, 0, (bmispress[5] == 1) ? 0 : 1);
		bmispress[5] = 1 - bmispress[5];
    }
    if (beforeMod.bmRightAlt != afterMod.bmRightAlt) {
        if(swich == 0 || swich == 2) Serial.write((bmispress[6] == 1) ? 233 : 108);
		if(swich == 1 || swich == 2) AddBtbm(0x0C, 0x01, 64, 0, 0, (bmispress[6] == 1) ? 0 : 1);
		bmispress[6] = 1 - bmispress[6];
    }
    if (beforeMod.bmRightGUI != afterMod.bmRightGUI) {
        if(swich == 0 || swich == 2) Serial.write((bmispress[7] == 1) ? 234 : 109);
		if(swich == 1 || swich == 2) AddBtbm(0x0C, 0x01, 128, 0, 0, (bmispress[7] == 1) ? 0 : 1);
		bmispress[7] = 1 - bmispress[7];
    }

}

void KbdRptParser::OnKeyUp(uint8_t mod, uint8_t key)
{
	key = key + 125;
    PrintKey(mod, key);
}

void KbdRptParser::AddBtbm(int addbtwid, int addbtkeymode, int addctrlbm, int addbtbm, int addpost, int addormin){
	btwid = addbtwid;
	btkeymode = addbtkeymode;
	if(addctrlbm != 0){
		if(addormin == 1) btctrlbm += addctrlbm;
		else btctrlbm -= addctrlbm;
	}
	if(addpost == 0){
		for(int i = 0; i < 4; i++){
			if(addormin == 1){
				if(btkey[i] == 0){
					btkey[i] = addbtbm;
					break;
				}
			}else{
				if(btkey[i] == addbtbm){
					btkey[i] = 0;
					break;
				}
			}
		}
	}else{
		if(addormin == 1){
			btkey[addpost - 5] += addbtbm;
		}else{
			btkey[addpost - 5] -= addbtbm;
		}
		
	}
	Serial1.write((byte)btwid);
	Serial1.write((byte)0x00);
	Serial1.write((byte)0xA1);
	Serial1.write((byte)btkeymode);
	if(btwid == 0x0C){
		Serial1.write((byte)btctrlbm);
		Serial1.write((byte)0x00);
		Serial1.write((byte)btkey[0]);
		Serial1.write((byte)btkey[1]);
		Serial1.write((byte)btkey[2]);
		Serial1.write((byte)btkey[3]);
		Serial1.write((byte)0);
		Serial1.write((byte)0);
	}else if(btwid == 0x08){
		Serial1.write((byte)btkey[0]);
		Serial1.write((byte)btkey[1]);
		Serial1.write((byte)btkey[2]);
		Serial1.write((byte)btkey[3]);
	}else{
		Serial1.write((byte)btctrlbm);
	}

};

USB     Usb;
//USBHub     Hub(&Usb);
HIDBoot<HID_PROTOCOL_KEYBOARD>    HidKeyboard(&Usb);

uint32_t next_time;

KbdRptParser Prs;

void setup()
{
    Serial.begin( 115200 );
    Serial1.begin( 9600 );

    if (Usb.Init() == -1)
		Serial.write(255);

    delay( 200 );

    next_time = millis() + 5000;

    HidKeyboard.SetReportParser(0, (HIDReportParser*)&Prs);
}

void loop()
{
    Usb.Task();
}

