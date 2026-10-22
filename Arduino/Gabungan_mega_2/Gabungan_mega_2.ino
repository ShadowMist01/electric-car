float jarak;
String jarak_akhir;
float kecepatan[2];
float percepatan;
float percepatan_1 = 0;
float percepatan_2 = 0;

#include <Wire.h>
#include <RCSwitch.h>
#include <Adafruit_MLX90614.h> //Deklarasi sensor suhu
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
#define pin_sensorbat A0 // Deklarasi pin sensor tegangan battery
#define pin_sensormot A1  // Deklarasi pin sensor tegangan motor
#define pin_trottle A9 // Deklarasi pin pedal gas
int Vout_RPM; //deklarasi tegangan output sesuai perputaran motor
long RPM; //deklarasi konversi Vout_RPM menjadi RPM

int vinbatread; //deklarasi tegangan input dari baterai
int vintrottleread; //deklarasi tegangan input dari motor
float vinbat;
float voutbat; //deklarasi hasil dari tegangan baterai
float vouttrottle; //deklarasi hasil dari tegangan trottle
float percentagebatt; //deklarasi persentase kapasitas baterai
int capacitybatt;
uint8_t bitResolution1 = 10; //deklarasi sensor arus WCS1800
uint8_t bitResolution2 = 10;
uint8_t bitResolution3 = 10;
uint8_t bitResolution4 = 10;
uint8_t correctionValue1 = 0;
uint8_t correctionValue2 = 0;
uint8_t correctionValue3 = 0;
uint8_t correctionValue4 = 0;
uint8_t vin1 = A5; // Aout WCS1800 (Motor BLDC Kanan Belakang)
uint8_t vin2 = A6; // Aout WCS1800 (Motor BLDC Kanan Depan)
uint8_t vin3 = A7; // Aout WCS1800 (Motor BLDC Kiri Belakang)
uint8_t vin4 = A8; // Aout WCS1800 (Motor BLDC Kiri Depan)
uint8_t vccPin = 8;
uint8_t vccPin1 = 12;
uint16_t iteration = 100;
float quiescent_Output_voltage1 = 0.5;
float quiescent_Output_voltage2 = 0.5;
float quiescent_Output_voltage3 = 0.5;
float quiescent_Output_voltage4 = 0.5;
float voltageReference1 = 5000.0;
float voltageReference2 = 5000.0;
float voltageReference3 = 5000.0;
float voltageReference4 = 5000.0;
float sensitivity1 = 66.0;
float sensitivity2 = 66.0;
float sensitivity3 = 66.0;
float sensitivity4 = 66.0;
float averageSensorValue1 = 0;
float averageSensorValue2 = 0;
float averageSensorValue3 = 0;
float averageSensorValue4 = 0;
float voltage1;
float voltage2;
float voltage3;
float voltage4;
float current1;
float current2;
float current3;
float current4;
int sensorValue1;
int sensorValue2;
int sensorValue3;
int sensorValue4;
float kirim1;
long kirim2;
float kirim3;
float kirim4;
float kirim5;
float kirim6;
RCSwitch mySwitch = RCSwitch();
void setup()
{
  Wire.begin();
  mySwitch.enableTransmit(19);
  mySwitch.setPulseLength(138);
  mySwitch.setRepeatTransmit(2);
  pinMode (pin_sensorbat, INPUT); //mensetting pin analog menjadi pin input
  pinMode (pin_sensormot, INPUT);
  Serial.begin(9600);
  //while (!Serial);

  //if (!mlx.begin()) {
  // Serial.println("Error connecting to MLX sensor. Check wiring.");
  // while (1);
  // };

}
void loop()
{
  getCurrent1(); //menjalankan fungsi void dari sensor arus
  vinbatread = analogRead(pin_sensorbat); //membaca sinyal analog di pin analog baterai
  Vout_RPM = analogRead(pin_sensormot);
  vintrottleread = analogRead(pin_trottle); //membaca sinyal analog di pin analog pedal gas
  float vinbat(vinbatread); //merubah data sinyal analog dari integer menjadi float
  float vintrottle(vintrottleread);
  RPM = map(Vout_RPM, 0, 177, 0, 481);
  voutbat = ((vinbat / 1024) * 5) * ((56000 + 4665) / 4665);
  vouttrottle = (vintrottle / (2048 / 10));
  int capacitybatt(voutbat);
  if (capacitybatt > 46)
  {
    percentagebatt = (((capacitybatt - 46) * 100) / (54 - 46));
  }
  if (capacitybatt <= 46)
  {
    percentagebatt = 0;
  }
  //Serial.print(percentagebatt);
  //Serial.print(voutbat);
  //Serial.println(averageSensorValue1);
  //Serial.print(averageSensorValue2);
  //Serial.print(averageSensorValue3);
  Serial.println(averageSensorValue4);
  //Serial.print(RPM);
  //Serial.print(mlx.readAmbientTempC());
  percentagebatt = percentagebatt * 100; //5 max
  voutbat = voutbat * 100; //4  max
  averageSensorValue1 = averageSensorValue1 * 100; //3 max
  averageSensorValue2 = averageSensorValue2 * 100; //3 max
  averageSensorValue3 = averageSensorValue3 * 100; //3 max
  averageSensorValue4 = averageSensorValue4 * 100; //3 max
  RPM = RPM; //4 max
  //Serial.println(RPM);
  //Serial.println(averageSensorValue1);
  //Serial.println(averageSensorValue2);
  //Serial.println(averageSensorValue3);
  //Serial.println(averageSensorValue4);
  // mlx.readAmbientTempC() = mlx.readAmbientTempC() * 100; //4 max //SDA SCL rusak, sensor suhu tidak dapat digunakan
  int suhu = 0; //program sementara karena SDA SCL rusak, hapus jika arduino telah diganti (SDA SCL dapat berfungsi)
  kirim1 = (1 * 1000000000) + (percentagebatt * 10000) + voutbat;
  //kirim2 = (2 * 1000000000) + (RPM * 10000) + mlx.readAmbientTempC(); //karena sensor suhu tidak dapat digunakan
  kirim2 = (2 * 1000000000) + (RPM * 10000) + suhu;
  kirim3 = (3 * 1000) + averageSensorValue1;
  kirim4 = (4 * 1000) + averageSensorValue2;
  kirim5 = (5 * 1000) + averageSensorValue3;
  kirim6 = (6 * 1000) + averageSensorValue4;
  mySwitch.send(kirim1, 32);
  mySwitch.send(kirim2, 32);
  mySwitch.send(kirim3, 32);
  mySwitch.send(kirim4, 32);
  mySwitch.send(kirim5, 32);
  mySwitch.send(kirim6, 32);
  mySwitch.send(1234, 32); //data perumpamaan ke 8 agar pengiriman lebih cepat
 Serial.println(kirim1);
  //delay(1000);
  /*kecepatan[0] = 3.14 * 0.3556 * (RPM * 0.005);
    kecepatan[1] = kecepatan[0] * 1000 / 300;
    percepatan_1 = kecepatan[1];
    percepatan = percepatan_1 - percepatan_2;
    percepatan_2 = percepatan_1;
    jarak = kecepatan[0] + jarak;
    String data_simpan = String (percentagebatt) + "," //Persen Battery
                       + String (voutbat) + "," //Voltage Battery
                       + String (averageSensorValue1) + "," //Current Motor 1
                       + String (averageSensorValue2) + "," //Current Motor 2
                       + String (averageSensorValue3) + "," //Current Motor 3
                       + String (averageSensorValue4) + "," //Current Motor 4
                       + String (RPM) + ","
                       + String (kecepatan[1]) + "," //Kecepatan Motor
                       + String (percepatan) + "," //Percepatan Motor
                       + String (jarak) + "," ;//Jarak Motor
                     //+ String (suhubat); //Temperatur Baterai
    Serial.println(data_simpan);
    delay(20);*/
  averageSensorValue1 = 0;
  averageSensorValue2 = 0;
  averageSensorValue3 = 0;
  averageSensorValue4 = 0;
}
void getCurrent1() //fungsi void sensor arus
{
  for (int i = 0; i < iteration; i++)
  {
    sensorValue1 = analogRead(vin1); //membaca sinyal analog pada pin analog input
    sensorValue2 = analogRead(vin2);
    sensorValue3 = analogRead(vin3);
    sensorValue4 = analogRead(vin4);
    delay(2);
    voltage1 = (sensorValue1) * ( voltageReference1 / (pow(2, bitResolution1) - 1)) - ( quiescent_Output_voltage1 * voltageReference1 ) + correctionValue1 ;
    voltage2 = (sensorValue2) * ( voltageReference2 / (pow(2, bitResolution2) - 1)) - ( quiescent_Output_voltage2 * voltageReference2 ) + correctionValue2 ;
    voltage3 = (sensorValue3) * ( voltageReference3 / (pow(2, bitResolution3) - 1)) - ( quiescent_Output_voltage3 * voltageReference3 ) + correctionValue3 ;
    voltage4 = (sensorValue4) * ( voltageReference4 / (pow(2, bitResolution4) - 1)) - ( quiescent_Output_voltage4 * voltageReference4 ) + correctionValue4 ;
    current1 = voltage1 / sensitivity1 ;
    current2 = voltage2 / sensitivity2 ;
    current3 = voltage3 / sensitivity3 ;
    current4 = voltage4 / sensitivity4 ;
    averageSensorValue1 += current1;
    averageSensorValue2 += current2;
    averageSensorValue3 += current3;
    averageSensorValue4 += current4;
  }
  averageSensorValue1 /= iteration;
  averageSensorValue2 /= iteration;
  averageSensorValue3 /= iteration;
  averageSensorValue4 /= iteration;
}
void setupMPU()
{
  Wire.beginTransmission(0b1101000); //This is the I2C address of the MPU (b1101000/b1101001 for AC0 low/high datasheet sec. 9.2)
  Wire.write(0x6B); //Accessing the register 6B - Power Management (Sec. 4.28)
  Wire.write(0b00000000); //Setting SLEEP register to 0. (Required; see Note on p. 9)
  Wire.endTransmission();
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.endTransmission();
}
