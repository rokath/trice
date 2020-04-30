#include <trice.h>
//#include <triceCheck.h>
#include <triceConfig.h>
#include <triceConfigArduino.h>
//#include <triceConfigCompiler.h>
//#include <triceConfigStm32PutChar.h>
//#include <triceConfigStm32_LL.h>
//#include <xteaCrypto.h>




/* https://www.mikrocontroller-elektronik.de/wifi-board-nanoesp-bzw-pretzel-board/
Software Serial Examples.
No change necessary.
 */
#include <SoftwareSerial.h>

SoftwareSerial esp8266(11, 12);


int triceWrite( char* buf, int count ){
    return Serial.write(buf, count);
}

void setup()
{
 Serial.begin(19200);
 esp8266.begin(19200);
 esp8266.println("AT");
}

void loop() // run over and over
{
 Serial.write("Hallo Welt\n");
 /*
 TRICE0(Id(65535), "Hallo Welt\n");
 for( int i = 0; i < 20; i++ ){
    triceServeTransmit();
 }
 delay(100);
 for( int i = 0; i < 20; i++ ){
    triceServeTransmit();
 }
 delay(100);
 for( int i = 0; i < 20; i++ ){
    triceServeTransmit();
 }*/
 delay(1000);
}
