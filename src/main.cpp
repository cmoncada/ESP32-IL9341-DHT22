#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <SPI.h>
#include <Fonts/FreeSansBold12pt7b.h>
#include <DHT.h>

#define TFT_CS 10
#define TFT_DC 7
#define TFT_MOSI 11
#define TFT_SCK 12
#define TFT_RESET 6
#define TFT_MISO 13

#define DHTPIN 15
#define DHTTYPE DHT22

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCK, TFT_RESET, TFT_MISO);
DHT dht(DHTPIN, DHTTYPE);

int margin = 10;
int internalMargin = 20;

uint16_t bgColor = 0xDF5F;
uint16_t textBgColor = 0xffff;
uint16_t textBodyColor = 0x08e8;
uint16_t divisorColor = 0x9c7a;

int percentageToPosition(int width, float percentage)
{
  int positionValue = (width * percentage) / 100;
  return positionValue;
}

void createTitle(int xPos, int yPos, uint16_t textColor, String text)
{
  tft.setCursor(xPos, yPos);
  tft.setFont(&FreeSansBold12pt7b);
  tft.setTextColor(textColor);
  tft.setTextSize(1);
  tft.println(text);
}

void createProgressBar(int posX, int posY, int progressValue, int width, int height, uint16_t positiveColor, uint16_t negativeColor, bool withLabel, String LabelText = "")
{
  if (withLabel)
  {
    tft.setCursor(posX, posY + 22);
    tft.setTextSize(1);
    tft.println(LabelText);
  }

  tft.fillRect(posX, posY, progressValue - posX, height, positiveColor);
  tft.fillRect(progressValue, posY, (width - progressValue) - posX, height, negativeColor);
}

void setup()
{
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(bgColor);
  dht.begin();
  tft.fillRect(margin, 10, 300, 220, textBgColor);
  tft.fillRect(internalMargin, 58, 280, 4, divisorColor);
  createTitle(internalMargin, 40, textBodyColor, "Temperature & Humidity");
}

void loop(void)
{
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  int progressValue = percentageToPosition(tft.width(), humidity);
  tft.setFont();

  tft.setCursor(internalMargin, 80);
  tft.setTextSize(2);
  tft.setTextColor(textBodyColor, textBgColor);
  tft.println("Temperature: " + String(temperature) + " C");
  tft.setCursor(internalMargin, 110);
  tft.println("Humidity: " + String(humidity) + " %");

  createProgressBar(internalMargin, 140, (temperature * 2), tft.width(), 20, 0xfb07, 0xdf7f, true, "Temperature Bar");
  createProgressBar(internalMargin, 180, progressValue, tft.width(), 20, 0x02f9, 0xdf7f, true, "Humidity Bar");  
  delay(100);
}