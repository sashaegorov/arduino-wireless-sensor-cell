#include <Adafruit_BMP085.h>
#include <util/delay.h>
#include <U8x8lib.h>
#include <DHT.h>
#include <DHT_U.h>

#define PRECISION 1
#define PASCALS_PER_MMHG 133.322368

// SSD1306 display
#define DISPLAY_SCK A3
#define DISPLAY_SDA A2
U8X8_SSD1306_128X32_UNIVISION_SW_I2C u8x8(DISPLAY_SCK, DISPLAY_SDA, U8X8_PIN_NONE);

// BMP180 sensor
Adafruit_BMP085 bmp;

// DHT22 sensor
#define DHTPIN 2
#define DHTTYPE DHT22 // DHT 22 (AM2302)
DHT_Unified dht(DHTPIN, DHTTYPE);

void setup(void)
{
  u8x8.begin();
  u8x8.setPowerSave(0);
  u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);

  u8x8.drawString(0, 0, "Pressure sensor");
  if (!bmp.begin()) {
    u8x8.drawString(0, 1, "has failed!");
    while (1) {}
  } else {
    u8x8.drawString(0, 1, "has started...");
  }
  _delay_ms(500);
  u8x8.clearDisplay();
}

float tData;
String t = String("t");
String tAlt = String("t*");
String c = String("*C");

long pData;
float mmHgData;
String Press = String("p");
String pa = String("Pa");
String mmhg = String("mmHg");

String Humidity = String("H");
String Percent = String("%");


void loop(void)
{
  // Humidity
  // ========
  sensors_event_t dhtTempEvent;
  sensors_event_t dhtHumidEvent;
  dht.temperature().getEvent(&dhtTempEvent);
  dht.humidity().getEvent(&dhtHumidEvent);

  if (isnan(dhtTempEvent.temperature) || isnan(dhtHumidEvent.relative_humidity)) {
    u8x8.drawString(0, 3, "DHT22 failed");
  } else {
    String tAltStatus = String(
                          tAlt + String(dhtTempEvent.temperature, PRECISION) + c + "/" +
                          Humidity + String(dhtHumidEvent.relative_humidity, 0) + Percent
                        );
    char tAltStatusOut[tAltStatus.length()];
    tAltStatus.toCharArray(tAltStatusOut, tAltStatus.length() + 1);
    u8x8.setInverseFont(1);
    u8x8.drawString(0, 0, tAltStatusOut);
    u8x8.setInverseFont(0);
  }

  
  // Temperature
  // ===========
  tData = bmp.readTemperature(); // Celsius
  String T = String(t + String(tData, PRECISION) + c);
  char tempOut[T.length()];
  T.toCharArray(tempOut, T.length() + 1);
 
  u8x8.drawString(0, 1, tempOut);

  // Pressure
  // ========

  pData = bmp.readPressure(); // Pascals
  String pressureStatus = String(Press + String(pData, DEC) + pa);
  char pressureStatusOut[pressureStatus.length()];
  pressureStatus.toCharArray(pressureStatusOut, pressureStatus.length() + 1);
  u8x8.drawString(0, 2, pressureStatusOut);

  mmHgData = (float)pData / PASCALS_PER_MMHG;
  String hhGhStatus = String(Press + String(mmHgData, PRECISION) + mmhg);
  char hhGhStatusOut[hhGhStatus.length()];
  hhGhStatus.toCharArray(hhGhStatusOut, hhGhStatus.length() + 1);
  u8x8.drawString(0, 3, hhGhStatusOut);

  _delay_ms(1000);
}
