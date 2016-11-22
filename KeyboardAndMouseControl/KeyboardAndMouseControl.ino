/*
*
*  Author  : KevinsBobo
*  Email   : kevins.bobo@gmail.com
*  Website : http://kevins.pro/
*
*/

/**********************************************************

	此段代码用于 Arduino Micro 上，接收 Arduino Mini
	传来的键位数据，然后做以处理向电脑输出。

**********************************************************/

int microqwertybm[] = { 0, 0, 0, 0, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 49, 50, 51, 52, 53, 54, 55, 56, 57, 48, 176, 177, 178, 179, 32, 45, 61, 91, 93, 92, 92, 59, 39, 96, 44, 46, 47, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 144, 145, 19, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 144, 47, 42, 45, 43, 176, 49, 50, 51, 52, 53, 54, 55, 56, 57, 48, 46, 0, 0, 128, 129, 130, 131, 132, 133, 134, 135}; // Qwerty 按键顺序数组

int microdvorakbm[] = { 0, 0, 0, 0, 97, 120, 106, 101, 46, 117, 105, 100, 99, 104, 116, 110, 109, 98, 114, 108, 39, 112, 111, 121, 103, 107, 44, 113, 102, 59, 49, 50, 51, 52, 53, 54, 55, 56, 57, 48, 176, 177, 178, 179, 32, 91, 93, 47, 61, 92, 92, 115, 45, 96, 119, 118, 122, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 144, 145, 19, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 144, 47, 42, 45, 43, 176, 49, 50, 51, 52, 53, 54, 55, 56, 57, 48, 46, 0, 0, 128, 129, 130, 131, 132, 133, 134, 135}; // Dvorak 按键顺序数组

int changedvo = 0; // dvorak切换判断
int capled = 0; // 大写锁定灯控制
int ledchanged = 1; // 大写锁定灯切换辅助
int swich = 0; // 输出模式
int swichturn = 0; // 输出模式切换
int swichchanged = 1; // 输出模式切换辅助
int arrowctrl = 0; // 方向控制键按下判断
int moctrl = 0; // 鼠标 单次1/多次0 移动模式判断
int momove[6] = { 0 }; // 鼠标移动
int momoved[6] = { 0 }; // 鼠标移动判断

void setup() { // initialize the buttons' inputs:
  
	Serial.begin(115200);
	Serial1.begin(115200);
	pinMode(13, OUTPUT);
	digitalWrite(13, LOW);
	// initialize mouse control:
	Mouse.begin();
	Keyboard.begin();
}

void loop() {
  // use serial input to control the mouse:
	if (Serial1.available() > 0) {
		int key = Serial1.read();
		if(key <= 109 && key >= 4){
			if(key == 57) ledchanged = 0;
			int keyp = (changedvo == 1) ? microdvorakbm[key] : microqwertybm[key];
			//Serial.println(-keyp);
			if(swich == 0 || swich == 2) Keyboard.press(keyp);
		}else if(key <= 234 && key >= 129){
			int keyp = (changedvo == 1) ? microdvorakbm[key - 125] : microqwertybm[key - 125];
			//Serial.println(-keyp);
			if(swich == 0 || swich == 2) Keyboard.release(keyp);
		}else if(key == 110){
			changedvo = 1 - changedvo;
		}else if(key == 121){
			arrowctrl = 1;
		}else if(key == 246){
			arrowctrl = 0;
		}else if(key == 111){
			moctrl = 1;
		}else if(key == 236){
			moctrl = 0;
		}else if(key == 113){
			//Serial.println("mouse right clickdown");
			Mouse.press(MOUSE_LEFT);
		}else if(key == 238){
			//Serial.println("mouse right clickup");
			Mouse.release(MOUSE_LEFT);
		}else if(key == 112){
			//Serial.println("mouse left clickdown");
			Mouse.press(MOUSE_RIGHT);
		}else if(key == 237){
			//Serial.println("mouse left clickup");
			Mouse.release(MOUSE_RIGHT);
		}else if(key == 119){
			//Serial.println("mouse centent clickdown");
			Mouse.press(MOUSE_MIDDLE);
		}else if(key == 244){
			//Serial.println("mouse centent clickup");
			Mouse.release(MOUSE_MIDDLE);
		}else if(key == 114){
			if(arrowctrl == 1){
				//Serial.println(218);
				Keyboard.press(218);
			}else if(moctrl == 1){
				//Serial.println("mouse move up");
				Mouse.move(0, -127);
			}else{
				momove[3] = 1;
			}
		
		}else if(key == 239){
			if(arrowctrl == 1){
				//Serial.println(-218);
				Keyboard.release(218);
			}else{
				momove[3] = 0;
			}
		
		}else if(key == 115){
			if(arrowctrl == 1){
				//Serial.println(216);
				Keyboard.press(216);
			}else if(moctrl == 1){
				//Serial.println("mouse move left");
				Mouse.move(-127, 0);
			}else{
				momove[1] = 1;
			}
		
		}else if(key == 240){
			if(arrowctrl == 1){
				//Serial.println(-216);
				Keyboard.release(216);
			}else{
				momove[1] = 0;
			}
		
		}else if(key == 116){
			if(arrowctrl == 1){
				//Serial.println(217);
				Keyboard.press(217);
			}else if(moctrl == 1){
				//Serial.println("mouse move down");
				Mouse.move(0, 127);
			}else{
				momove[2] = 1;
			}
		
		}else if(key == 241){
			if(arrowctrl == 1){
				//Serial.println(-217);
				Keyboard.release(217);
			}else{
				momove[2] = 0;
			}
		
		}else if(key == 117){
			if(arrowctrl == 1){
				//Serial.println(215);
				Keyboard.press(215);
			}else if(moctrl == 1){
				//Serial.println("mouse move right");
				Mouse.move(127, 0);
			}else{
				momove[0] = 1;
			}
		
		}else if(key == 242){
			if(arrowctrl == 1){
				//Serial.println(-215);
				Keyboard.release(215);
			}else{
				momove[0] = 0;
			}
		
		}else if(key == 118){
			if(moctrl == 1){
				//Serial.println("mouse to up");
				Mouse.move(0, 0, 1);
			}else{
				momove[4] = 1;
			}
		
		}else if(key == 243){
			momove[4] = 0;
		
		}else if(key == 120){
			if(moctrl == 1){
				//Serial.println("mouse to down");
				Mouse.move(0, 0, -1);
			}else{
				momove[5] = 1;
			}

		}else if(key == 245){
			momove[5] = 0;
		}
		else if(key >= 0 && key <= 3){
			swichchanged = 0;
			swichturn = key;
		}
		//Serial.println(key);
	}

	if(ledchanged == 0){
		capled = 1 - capled;
		if(capled == 0) digitalWrite(13, LOW);
		else if(capled == 1) digitalWrite(13, HIGH);
		ledchanged = 1;
	}

	if(swichchanged == 0){
		if((swich == 0 || swich == 2) && (swichturn == 1 || swichturn == 3)){
			swich = swichturn;
			//Serial.println("keybord is end");
			Mouse.end();
			Keyboard.end();
		}else if((swich == 1 || swich == 3) && (swichturn == 0 || swichturn == 2)){
			swich = swichturn;
			//Serial.println("keybord is begin");
			Mouse.begin();
			Keyboard.begin();
		}
		swichchanged = 1;
	}

	for(int i; i < 4; i++){
		if(momove[i] == 1){
			//Serial.println(i+215);
			switch( i ){
				case 0: Mouse.move(1, 0); break;
				case 1: Mouse.move(-1, 0); break;
				case 2: Mouse.move(0, 1); break;
				case 3: Mouse.move(0, -1); break;
			}
			delay(5);
			if(momoved[i] == 0) momoved[i] = 1;
		}else if(momoved[i] == 1 && momove[i] == 0){
			momoved[i] == 0;
		}
	}

	if(momove[4] == 1){
		//Serial.println("mouse to up");
		Mouse.move(0, 0, 1);
		momoved[4] == 1;
		delay(10);
	}else if(momoved[4] == 1 && momove[4] == 0){
		momoved[0] = 0;
	}

	if(momove[5] == 1){
		//Serial.println("mouse to down");
		Mouse.move(0, 0, -1);
		momoved[5] == 1;
		delay(10);
	}else if(momoved[5] == 1 && momove[5] == 0){
		momoved[5] = 0;
	}

}

