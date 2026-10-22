#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
//#include <TouchScreen.h>
//#define MINPRESSURE 200
//#define MAXPRESSURE 1000
//const int XP = 8, XM = A2, YP = A3, YM = 9; //320x480 ID=0x9486
const int TS_LEFT = 514, TS_RT = 521, TS_TOP = 534, TS_BOT = 532;
//TSPoint tp;
int pixel_x, pixel_y;     //Touch_getXY() updates global vars
uint16_t ID;
#define BLACK   0x0000
#define RED     0xF800
#define GREEN   0x07E0
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

//TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

#include <RCSwitch.h>
RCSwitch mySwitch = RCSwitch();

#include <SPI.h>
#include <SD.h>
File myFile;
long receive[3] = {0, 0, 0};
unsigned int batt;
float percentagebatt;
float voutbat;
long RPM;
float averageSensorValue1;
float averageSensorValue2;
float averageSensorValue3;
float averageSensorValue4;
unsigned int amp1;
unsigned int amp2;
unsigned int amp4;
float suhubat;
String data_simpan;
float jarak = 0;
float jarak_km;
String jarak_akhir;
float kecepatan[2];
float percepatan;
float percepatan_1 = 0;
float percepatan_2 = 0;
char baca1;
String baca;
String satuanjarak[2] = {"m", "km"};
int satuan = 0;
void setup()
{
  //Serial.begin(9600);
  tft.reset();
  ID = tft.readID();
  tft.begin(ID);
  tft.setRotation(1);            //layar landscape
  tft.fillScreen(BLACK);
  //pinMode(YP, OUTPUT);      
  //pinMode(XM, OUTPUT);
  tft.setTextColor(WHITE, BLACK);
  mySwitch.enableReceive(2);  // Receiver on interrupt 2 => that is pin #21
  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  if (!SD.begin(53))
  {
    tft.fillScreen(BLACK);
    tft.setTextColor(RED, BLACK);
    tft.setCursor(90, 32);
    tft.setTextSize(5);
    tft.print("Cek Koneksi");
    tft.setCursor(140, 90);
    tft.setTextSize(5);
    tft.print("Micro SD");
    tft.setCursor(30, 160);
    tft.setTextSize(5);
    tft.print("Kemudian Reset");
    tft.setCursor(130, 230);
    tft.setTextSize(5);
    tft.print("Arduino");
    while (1);
  }
  if (SD.exists("JARAK.txt") == true) //mengecek jika ada file jarak tempuh yang telah dibuat
  {
    myFile = SD.open("JARAK.txt");
    if (myFile)
    {
      while (myFile.available())
      {
        baca1 = myFile.read();
        baca = String(baca + baca1);
      }
      myFile.close();
    }
    jarak = baca.toFloat();
  }
  tft.fillRect(320, 130, 32, 38, WHITE);
  tft.fillRect(416, 130, 32, 38, WHITE);
  tft.fillRect(320, 260, 32, 38, WHITE);
  tft.fillRect(416, 260, 32, 38, WHITE);
  tft.fillRect(352, 139, 64, 20, WHITE);
  tft.fillRect(352, 269, 64, 20, WHITE);
  tft.fillRect(374, 139, 20, 130, WHITE);
  tft.setCursor(10, 140);
  tft.setTextSize(3);
  tft.print("Kecepatan:");
  tft.setCursor(230, 235);
  tft.print("m/s");
  tft.setCursor(10, 32);
  tft.setTextSize(2);
  tft.print("Jarak: ");
  tft.setCursor(150, 32);
  tft.print("Suhu Baterai: ");
  tft.setCursor(460, 0);
  tft.print("%");
  tft.setCursor(340, 0);
  tft.print("V /");
  tft.setCursor(268, 70);
  tft.print("C");
  tft.setCursor(335, 32);
  tft.print("Percepatan: ");
  tft.setCursor(360, 110);
  tft.print("A");
  tft.setCursor(450, 110);
  tft.print("A");
  tft.setCursor(360, 240);
  tft.print("A");
  tft.setCursor(450, 240);
  tft.setTextSize(2);
  tft.print("A");
  tft.setCursor(431, 70);
  tft.print("m/s");
  tft.setCursor(470, 64);
  tft.setTextSize(1);
  tft.print("2");
  tft.setCursor(263, 64);
  tft.print("o");
}

void loop()
{
  while (!Serial)//program pengecekan koneksi SD Card
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  if (!SD.begin(53)) 
  {
    tft.fillScreen(BLACK);
    tft.setTextColor(RED, BLACK);
    tft.setCursor(90, 32);
    tft.setTextSize(5);
    tft.print("Cek Koneksi");
    tft.setCursor(140, 90);
    tft.print("Micro SD");
    tft.setCursor(30, 160);
    tft.print("Kemudian Reset");
    tft.setCursor(130, 230);
    tft.print("Arduino");
    while (1);
  }
  if (mySwitch.available())
  {
    //membedah program sesuai urutan
    if (mySwitch.getReceivedValue() >= 1000000000 && mySwitch.getReceivedValue() < 2000000000) 
    {
      receive[0] = mySwitch.getReceivedValue();
    }
    else if (mySwitch.getReceivedValue() >= 2000000000 && mySwitch.getReceivedValue() < 3000000000)
    {
      receive[1] = mySwitch.getReceivedValue();
    }
    else if (mySwitch.getReceivedValue() >= 30000000 && mySwitch.getReceivedValue() < 40000000)
    {
      receive[2] = mySwitch.getReceivedValue();
    }
    mySwitch.resetAvailable();
  }
  while (receive[0] > 0 && receive[1] > 0 && receive[2] > 0) //ketika semua data telah diterima, masuk ke program penampil
  {
    while (!Serial) //mengecek koneksi micro sd card
    {
      ; // wait for serial port to connect. Needed for native USB port only
    }
    if (!SD.begin(53))
    {
      tft.fillScreen(BLACK);
      tft.setTextColor(RED, BLACK);
      tft.setCursor(90, 32);
      tft.setTextSize(5);
      tft.print("Cek Koneksi");
      tft.setCursor(140, 90);
      tft.print("Micro SD");
      tft.setCursor(30, 160);
      tft.print("Kemudian Reset");
      tft.setCursor(130, 230);
      tft.print("Arduino");
      while (1);
    }
     //membedah seluruh data yang diterima sesuai data yang ada 
    receive[0] -= 1000000000;
    receive[1] -= 2000000000;
    receive[2] -= 30000000;
    amp1 = receive[0] * 0.000001;
    averageSensorValue1 = amp1 * 0.01;
    amp2 = (receive[0] - (averageSensorValue1 * 100000000)) * 0.001;
    averageSensorValue2 = amp2 * 0.01;
    averageSensorValue3 = (receive[0] - ((averageSensorValue1 * 100000000) + (averageSensorValue2 * 100000))) * 0.01;
    RPM = receive[1] * 0.0001;
    suhubat = (receive[1] - (RPM * 10000)) * 0.01;
    amp4 = receive[2] * 0.0001;
    averageSensorValue4 = amp4 * 0.01;
    voutbat = (receive[2] - (averageSensorValue4 * 1000000)) * 0.01;

    //program kapasitas baterai
    int capacitybatt(voutbat);
    if (capacitybatt > 46) 
    {
      percentagebatt = (((capacitybatt - 46) * 100) / (54 - 46));
    }
    if (capacitybatt <= 46)
    {
      percentagebatt = 0;
    }
    //menghitung data kecepatan, percepatan, dan jarak
    kecepatan[0] = 3.14 * 0.3606 * (RPM * 0.008); 
    kecepatan[1] = kecepatan[0] * 1000 / 500;
    percepatan_1 = kecepatan[1];
    percepatan = percepatan_1 - percepatan_2;
    percepatan_2 = percepatan_1;
    jarak = kecepatan[0] + jarak;
    //Serial.println(millis() - t);
    
    //program penampil data
    if (percentagebatt >= 75) 
    {
      tft.setTextColor(GREEN, BLACK);
    }
    else if (percentagebatt < 75 && percentagebatt >= 50)
    {
      tft.setTextColor(YELLOW, BLACK);
    }
    else if (percentagebatt < 50)
    {
      tft.setTextColor(RED, BLACK);
    }
    tft.setCursor(385, 0);
    tft.setTextSize(2);
    tft.print(percentagebatt); //input percentagebatt
    tft.setCursor(275, 0);
    tft.print(voutbat); //input voutbatt
    tft.setCursor(340, 0);
    tft.print("V /");
    tft.setCursor(460, 0);
    tft.print("%");
    tft.setTextColor(WHITE, BLACK);

    tft.setCursor(310, 110);
    tft.print(averageSensorValue1);  //Arus motor (Kiri Depan)
    tft.setCursor(400, 110);
    tft.print(averageSensorValue3);  //Arus motor (Kanan Depan)
    tft.setCursor(310, 240);
    tft.print(averageSensorValue2);  //Arus motor (Kiri Belakang)
    tft.setCursor(400, 240);
    tft.print(averageSensorValue4);  //Arus motor (Kanan Belakang)
    if (jarak < 1000)
    {
      satuan = 0;
    }
    else if (jarak > 999)
    {
      satuan = 1;
    }
    tft.setCursor(123, 70);
    tft.print(satuanjarak[satuan]);

    tft.setCursor(10, 64);
    tft.setTextSize(3);
    if (jarak < 1000)
    {
      tft.print(jarak);  //input jarak
    }
    else if (jarak > 999)
    {
      jarak_km = jarak * 0.001;
      tft.print(jarak_km);  //input jarak
    }
    tft.setCursor(170, 64);
    tft.print(suhubat);  //input suhu baterai
    tft.setCursor(335, 64);
    if (percepatan >= 0)
    {
      tft.print(percepatan);  //input percepatan
      tft.fillRect(405, 60, 26, 26, BLACK);
    }
    else if (percepatan < 10)
    {
      tft.print(percepatan);  //input percepatan
    }
    tft.setCursor(10, 210);
    tft.setTextSize(7);
    tft.print(kecepatan[1]); //input kecepatan
    //menggabung data untuk penyimpanan data
    data_simpan = String (percentagebatt) + "," //Persen Battery
                  + String (voutbat) + "," //Voltage Battery
                  + String (averageSensorValue1) + "," //Current Motor 1 (Kiri Depan)
                  + String (averageSensorValue2) + "," //Current Motor 2 (Kiri Belakang)
                  + String (averageSensorValue3) + "," //Current Motor 3 (Kanan Depan)
                  + String (averageSensorValue4) + "," //Current Motor 4 (Kanan Belakang)
                  + String (kecepatan[1]) + "," //Kecepatan Motor
                  + String (percepatan) + "," //Percepatan Motor
                  + String (jarak) + "," //Jarak Motor
                  + String (suhubat); //Temperatur Baterai
    jarak_akhir = String (jarak); //merekam jarak terakhir
    if (SD.exists("JARAK.txt") == true)
    {
      SD.remove("JARAK.txt");
    }

    myFile = SD.open("JARAK.txt", FILE_WRITE);
    // if the file opened okay, write to it:
    if (myFile)
    {
      myFile.println(jarak_akhir);
      myFile.close();
    }
    //merekam seluruh data monitor
    if (SD.exists("MONITOR.csv") == false)
    {
      myFile = SD.open("MONITOR.csv", FILE_WRITE);
      // if the file opened okay, write to it:
      if (myFile)
      {
        myFile.println("Persen Baterai (%),Tegangan Baterai (Volt),Arus Motor 1 (Ampere)(Kiri Depan),Arus Motor 2 (Ampere)(Kiri Belakang),Arus Motor 3 (Ampere)(Kanan Depan),Arus Motor 4 (Ampere)(Kanan Belakang),Kecepatan(m/s),Percepatan(m/s^2),Jarak(Meter),Suhu Baterai (*C)");
        myFile.println(data_simpan);
        myFile.close();
      }
    }
    else if (SD.exists("MONITOR.csv") == true)
    {
      myFile = SD.open("MONITOR.csv", FILE_WRITE);
      // if the file opened okay, write to it:
      if (myFile)
      {
        myFile.println(data_simpan);
        myFile.close();
      }
    }
    //mereset bagian penampil data, agar dapat menerima data kembali
    receive[2] = 0;
    receive[1] = 0;
    receive[0] = 0;
  }
}
