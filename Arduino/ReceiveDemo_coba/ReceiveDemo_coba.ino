/*
  Simple example for receiving

  https://github.com/sui77/rc-switch/
*/

#include <RCSwitch.h>
float receive[6] = {0, 0, 0, 0, 0};
int batt;
int comma;
float comma_2;
float percentagebatt;
float voutbat;
float RPM;
float averageSensorValue1;
float averageSensorValue2;
float averageSensorValue3;
float averageSensorValue4;
float suhubat;
String data_simpan;
float jarak = 0;
String jarak_akhir;
float kecepatan[2];
float percepatan;
float percepatan_1 = 0;
float percepatan_2 = 0;
char baca1;
String baca;
RCSwitch mySwitch = RCSwitch();

void setup() {
  Serial.begin(9600);
  mySwitch.enableReceive(2);  // Receiver on interrupt 0 => that is pin #2
}

void loop() {
  if (mySwitch.available())
  {
    Serial.println(mySwitch.getReceivedValue());
    if (mySwitch.getReceivedValue() >= 1000000000 && mySwitch.getReceivedValue() < 2000000000)
    {
      receive[0] = mySwitch.getReceivedValue();
    }
    else if (mySwitch.getReceivedValue() >= 2000000000 && mySwitch.getReceivedValue() < 3000000000)
    {
      receive[1] = mySwitch.getReceivedValue();
    }
    else if (mySwitch.getReceivedValue() >= 3000 && mySwitch.getReceivedValue() < 4000)
    {
      receive[2] = mySwitch.getReceivedValue();
    }
    else if (mySwitch.getReceivedValue() >= 4000 && mySwitch.getReceivedValue() < 5000)
    {
      receive[3] = mySwitch.getReceivedValue();
    }
    else if (mySwitch.getReceivedValue() >= 5000 && mySwitch.getReceivedValue() < 6000)
    {
      receive[4] = mySwitch.getReceivedValue();
    }
    else if (mySwitch.getReceivedValue() >= 6000 && mySwitch.getReceivedValue() < 7000)
    {
      receive[5] = mySwitch.getReceivedValue();
    }
    mySwitch.resetAvailable();
  }

  while (receive[0] > 0 && receive[1] > 0 && receive[2] > 0 && receive[3] > 0 && receive[4] > 0 && receive[5])
  {
    receive[0] = receive[0] - 1000000000;
    receive[1] = receive[1] - 2000000000;
    receive[2] = receive[2] - 3000;
    receive[3] = receive[3] - 4000;
    receive[4] = receive[4] - 5000;
    receive[5] = receive[5] - 6000;
    batt = receive[0] * 0.000001;
    comma = (receive[0] - (batt * 1000000)) * 0.0001;
    comma_2 = comma * 0.01;
    //Serial.print("comma = ");
    //Serial.println(comma_2);
    percentagebatt = batt + comma_2;
    voutbat = (receive[0] - (percentagebatt * 1000000)) * 0.01;
    RPM = receive[1] * 0.0001;
    suhubat = (receive[1] - (RPM * 10000)) * 0.01;
    averageSensorValue1 = receive[2]  * 0.01;
    averageSensorValue2 = receive[3]  * 0.01;
    averageSensorValue3 = receive[4]  * 0.01;
    averageSensorValue4 = receive[5]  * 0.01;
    String data_lihat = String (percentagebatt) + "," //Persen Battery
                        + String (voutbat) + "," //Voltage Battery
                        + String (averageSensorValue1) + "," //Current Motor 1
                        + String (averageSensorValue2) + "," //Current Motor 2
                        + String (averageSensorValue3) + "," //Current Motor 3
                        + String (averageSensorValue4) + "," //Current Motor 4
                        + String (RPM) + "," //Kecepatan Motor
                        + String (suhubat); //Temperatur Baterai
    Serial.println(data_lihat);
    receive[5] = 0;
    receive[4] = 0;
    receive[3] = 0;
    receive[2] = 0;
    receive[1] = 0;
    receive[0] = 0;
    /*kecepatan[0] = 3.14 * 0.3556 * (RPM * 0.005);
      kecepatan[1] = kecepatan[0] * 1000 / 300;
      percepatan_1 = kecepatan[1];
      percepatan = percepatan_1 - percepatan_2;
      percepatan_2 = percepatan_1;
      jarak = kecepatan[0] + jarak;
      kecepatan[1] = (kecepatan[1], 2);
      percepatan = (percepatan, 2);
      jarak = (jarak, 2);
      data_simpan = String (percentagebatt) + "," //Persen Battery
                  + String (voutbat) + "," //Voltage Battery
                  + String (averageSensorValue1) + "," //Current Motor 1
                  + String (averageSensorValue2) + "," //Current Motor 2
                  + String (averageSensorValue3) + "," //Current Motor 3
                  + String (averageSensorValue4) + "," //Current Motor 4
                  + String (kecepatan[1]) + "," //Kecepatan Motor
                  + String (percepatan) + "," //Percepatan Motor
                  + String (jarak) + "," //Jarak Motor
                  + String (suhubat); //Temperatur Baterai
      jarak_akhir = String (jarak);
      Serial.println(data_simpan);*/
  }
}
