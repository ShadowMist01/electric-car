/*
  Example for different sending methods

  https://github.com/sui77/rc-switch/

*/

#include <RCSwitch.h>
RCSwitch mySwitch = RCSwitch();
void setup() {

  Serial.begin(9600);


  mySwitch.enableTransmit(19); //mengeluarkan sinyal pada pin digital 19

  mySwitch.setPulseLength(137); //panjang frekuensi gelombang

  mySwitch.setRepeatTransmit(2); //pengulangan pengiriman
}

void loop() {
  mySwitch.send(1000000000, 32);
  mySwitch.send(2000000000, 32);
  mySwitch.send(30000000, 32);

}
