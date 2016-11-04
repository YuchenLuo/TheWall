#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#define PIN 6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);
#include <Arduino.h>
#include <SPI.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "BluefruitConfig.h"

#define FACTORYRESET_ENABLE         1
#define MINIMUM_FIRMWARE_VERSION    "0.6.6"
#define MODE_LED_BEHAVIOUR          "MODE"
Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

uint8_t charmap[26] = {8,7,6,5,4,3,2,1,0,11,12,13,14,15,16,17,18,19,29,28,27,26,25,24,23,22};
bool strobe = false;


void incrWalk(uint32_t c, uint8_t wait, uint16_t count);
void colorWalk(uint32_t c, uint8_t wait);
void allOn();
void showMessage( char* buf );


void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}
    
void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  
  ble.begin(VERBOSE_MODE);
  ble.factoryReset();
  ble.echo(true);
  ble.info();
  ble.verbose(false);

  int idx = 0;
  while (! ble.isConnected()) {
    incrWalk( 0, 100, 5  );
  }

  
  if ( ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION) ) {
    ble.sendCommandCheckOK("AT+HWModeLED=" MODE_LED_BEHAVIOUR);
  }
  
  colorWalk(0,50);
//  clearLeds();
  Serial.print("Start!");
}

void loop() {
  if(strobe) {
    strip.setPixelColor( random(30), rcol2() );
    for(int i = 0; i < 5; i++)
      strip.setPixelColor( random(30), 0 );
    strip.show();
  }
  // Check for incoming characters from Bluefruit
  ble.println("AT+BLEUARTRX");
  ble.readline();
  if (strcmp(ble.buffer, "OK") == 0) {
    return;
  }
  // Some data was found, its in the buffer
  //Serial.print(F("[Recv] ")); Serial.println(ble.buffer);
  ble.waitForOK();
  showMessage(ble.buffer);
  if( ble.buffer[0] == 33 )
    strobe = !strobe;
}




void clearLeds() {
  for (uint16_t i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor( i, 0 );
  }
  strip.show();
}
void allOn() {
  for (uint16_t i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor( i, rcol2() );
  }
  strip.show();
}


uint32_t rcol() {
  return strip.Color(random(150,256), random(150,256), random(150,256));
}
uint32_t rcol2() {
  uint8_t colour = random(3);
  if( colour == 0 ) {
      return strip.Color(random(255), random(32), random(32));
  } else if ( colour == 1 ) {
    return strip.Color(random(32), random(255), random(32));
  } else {
    return strip.Color(random(32), random(32), random(255));
  }
}

void showChar( char c ) {
  if( (c>96) && (c<123) ) {
    uint8_t pos = charmap[c-97];
    strip.setPixelColor( pos, rcol2() );
    strip.show();
    delay(1000);
    strip.setPixelColor( pos, 0 );
    strip.show();
    delay(250);
    return;
  } else if (isWhitespace(c)){
    allOn();
    delay(1000);
    clearLeds();
  }
  return;
}

void showMessage( char* buf ) {
    clearLeds();
    int i;
    for( i = 0; i < strlen(buf); i++ ) {
      showChar(buf[i]);
      if(!isAlphaNumeric(buf[i])&(!isWhitespace(buf[i]))) {
        return;
      }
    }
//    colorWalk(rcol(),50);
}

void colorWalk(uint32_t c, uint8_t wait) {
  
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    if(c)
        strip.setPixelColor(i,c);
    else
      strip.setPixelColor(i, rcol());
    strip.show();
    delay(wait);
    strip.setPixelColor(i, 0);
    strip.show();
  }
}

void incrWalk(uint32_t c, uint8_t wait, uint16_t count) {
  static int idx;
  for(int16_t i = 0; i < count; i++ ) {
    idx = idx % (strip.numPixels() - count );
    if(c)
      strip.setPixelColor(idx,c);
    else
      strip.setPixelColor(idx, rcol2());
    strip.show();
    delay(wait);
    idx++;
    clearLeds();
  }
}

