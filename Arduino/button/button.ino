

#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
#include <TouchScreen.h>
#define MINPRESSURE 200
#define MAXPRESSURE 1000

// ALL Touch panels and wiring is DIFFERENT
// copy-paste results from TouchScreen_Calibr_native.ino
const int XP = 8, XM = A2, YP = A3, YM = 9; //320x480 ID=0x9486
const int TS_LEFT = 514, TS_RT = 521, TS_TOP = 534, TS_BOT = 532;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

Adafruit_GFX_Button on_btn, off_btn;
TSPoint p;
int pixel_x, pixel_y;     //Touch_getXY() updates global vars
/*bool Touch_getXY(void)
{
  pinMode(YP, OUTPUT);      //restore shared pins
  pinMode(XM, OUTPUT);
  digitalWrite(YP, HIGH);   //because TFT control pins
  digitalWrite(XM, HIGH);
  bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);
  if (pressed) {
    pixel_x = map(p.x, TS_LEFT, TS_RT, 0, tft.width()); //.kbv makes sense to me
    pixel_y = map(p.y, TS_TOP, TS_BOT, 0, tft.height());
  }
  return pressed;
}*/

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

void setup()
{
  int x = 0;
  Serial.begin(9600);
  uint16_t ID = tft.readID();
  Serial.print("TFT ID = 0x");
  Serial.println(ID, HEX);
  Serial.println("Calibrate for your Touch Panel");
  //if (ID == 0xD3D3) ID = 0x9486; // write-only shield
  tft.begin(ID);
  tft.setRotation(0);            //PORTRAIT
  tft.fillScreen(BLACK);
  on_btn.initButton(&tft,  60, 200, 100, 40, WHITE, CYAN, BLACK, "ON", 2);
  off_btn.initButton(&tft, 180, 200, 100, 40, WHITE, CYAN, BLACK, "OFF", 2);
  on_btn.drawButton(false);
  off_btn.drawButton(false);
  tft.fillRect(40, 80, 160, 80, RED);
}

/* two buttons are quite simple
*/
void loop()
{
  int x = 0;
  while (x == 0 )
  {
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    p = ts.getPoint();
    if (p.z < MINPRESSURE || p.z > MAXPRESSURE)continue;
    if (p.x > 0 && p.x < 1000  && p.y > 0 && p.y < 1000)
    {
      x = 1;
    }
    Serial.println("x =" + String(p.x)+" y =" + String(p.y));
  }
  while(x == 1)
  tft.fillRect(40, 80, 160, 80, GREEN);
}
