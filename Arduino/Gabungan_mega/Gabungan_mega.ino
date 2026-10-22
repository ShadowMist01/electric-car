#include <Wire.h>
#include <RCSwitch.h>
#include <Adafruit_MLX90614.h> //Deklarasi sensor suhu
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
#define pin_sensorbat A0 // Deklarasi pin sensor tegangan battery
#define pin_sensormot A1  // Deklarasi pin sensor tegangan motor
#define vin1 A5 // Aout WCS1800 (Motor BLDC Kiri Depan)
#define vin2 A6 // Aout WCS1800 (Motor BLDC Kiri Belakang)
#define vin3 A7 // Aout WCS1800 (Motor BLDC Kanan Depan)
#define vin4 A8 // Aout WCS1800 (Motor BLDC Kanan Belakang)

int Vout_RPM; //deklarasi tegangan output sesuai perputaran motor
long RPM; //deklarasi konversi Vout_RPM menjadi RPM

int vinbatread; //deklarasi tegangan input dari baterai
float vinbat;
float voutbat; //deklarasi hasil dari tegangan baterai
float percentagebatt; //deklarasi persentase kapasitas baterai
int capacitybatt;
int sensorValue1;
int sensorValue2;
int sensorValue3;
int sensorValue4;
long kirim1;
long kirim2;
long kirim3;
long ampere1;
long ampere2;
long ampere3;
long ampere4;
long suhu;
long batt;
long percentbatt;
RCSwitch mySwitch = RCSwitch();
//long t;
void setup()
{
  Serial.begin(9600);
  Wire.begin();
  mySwitch.enableTransmit(19); 
  mySwitch.setPulseLength(137); //panjang gelombang
  mySwitch.setRepeatTransmit(2); //pengulangan paling cepat
  pinMode (pin_sensorbat, INPUT); //mensetting pin analog menjadi pin input
  pinMode (pin_sensormot, INPUT);
  pinMode (vin1, INPUT);
  pinMode (vin2, INPUT);
  pinMode (vin3, INPUT);
  pinMode (vin4, INPUT);
  while (!Serial);
  if (!mlx.begin()) {
    while (1);
  };

}
void loop()
{
  //t = millis();
  //membaca sinyal analog pada pin analog input
  sensorValue1 = analogRead(vin1); 
  sensorValue2 = analogRead(vin2);
  sensorValue3 = analogRead(vin3);
  sensorValue4 = analogRead(vin4);
  vinbatread = analogRead(pin_sensorbat); //membaca sinyal analog di pin analog baterai
  Vout_RPM = analogRead(pin_sensormot);
  float vinbat(vinbatread); //merubah data sinyal analog dari integer menjadi float
  RPM = map(Vout_RPM, 0, 177, 0, 481); //mengubah data tegangan menjadi RPM
  voutbat = ((vinbat / 1024) * 5) * ((56000 + 4665) / 4665); //menjabarkan pembagi tegangan
  ampere1 = map(sensorValue1, 539, 570, 1, 108); //kiri depan
  ampere2 = map(sensorValue2, 538, 562, 1, 81); //kiri belakang
  ampere3 = map(sensorValue3, 528, 555, 2, 114); //kanan depan
  ampere4 = map(sensorValue4, 511, 530, 1, 96); //kanan belakang
  batt = voutbat * 100; //4 digit max
  RPM = RPM; //3 digit max
  suhu = mlx.readObjectTempC() * 100; //4 digit max
  
  //menggabungkan data untuk mengurangi banyak data yang dikirim
  kirim1 = (1000000000) + (ampere1 * 1000000) + (ampere2 * 1000) + ampere3; 
  kirim2 = (2000000000) + (RPM * 10000) + suhu;
  kirim3 = (30000000) + (ampere4 * 10000) + batt;
  mySwitch.send(kirim1, 32);
  mySwitch.send(kirim2, 32);
  mySwitch.send(kirim3, 32);
  mySwitch.send(0, 32); //data perumpamaan ke 4 agar pengiriman lebih cepat
  //Serial.println(millis()-t);
}
