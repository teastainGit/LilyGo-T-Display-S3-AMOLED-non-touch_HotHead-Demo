//For AMOLED version
#include "rm67162.h"
#include <TFT_eSPI.h>
#include "HotHead240x240.h"
#define WIDTH 536
#define HEIGHT 240
#define topbutton 0
#define lowerbutton 21
#define LED 38
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite spr = TFT_eSprite(&tft);

int progress = 0;

bool ticktock = true;

unsigned colour = 0xFFFF;

void setup() {
  Serial.begin(115200);
  pinMode(lowerbutton, INPUT);  //Right button pulled up, push = 0
  pinMode(topbutton, INPUT);    //Left button  pulled up, push = 0
  pinMode(LED, OUTPUT);
  rm67162_init();
  lcd_setRotation(3);
  spr.createSprite(WIDTH, HEIGHT);
  spr.setSwapBytes(1);
  spr.drawString(" Setup!", 77, 55, 4);
  spr.pushImage(296, 0, 240, 240, HotHead240x240);
  lcd_PushColors(0, 0, WIDTH, HEIGHT, (uint16_t *)spr.getPointer());
  digitalWrite(LED, HIGH);
  delay(2000);
  digitalWrite(LED, LOW);
  delay(2000);
}

void loop() {
  ticktock = !ticktock;
  if (ticktock) {
    Serial.println("tick");  //(Serial print slows progress bar Demo)
  } else {
    Serial.println("tock");
  }
  spr.fillSprite(TFT_BLACK);
  spr.pushImage(296, 0, 240, 240, HotHead240x240);
  spr.setTextColor(TFT_GREEN, TFT_BLACK);
  spr.drawString("  HotHead!  ", 80, 55, 4);
  spr.setTextSize(1);
  spr.drawRect(0, 0, 125, 35, TFT_BLUE);    //"Top"  text box
  spr.drawRect(0, 205, 125, 35, TFT_BLUE);  // "Bottom" text box
  if (digitalRead(0)) {                     //  normally open sw, pulled up.
    spr.setTextColor(TFT_GREEN, TFT_BLACK);
    digitalWrite(LED, LOW);
  } else {
    spr.setTextColor(TFT_RED, TFT_BLACK);
    digitalWrite(LED, HIGH);
  }
  spr.drawString("< GPIO 0", 10, 8, 4);
  if (digitalRead(21)) {
    spr.setTextColor(TFT_GREEN, TFT_BLACK);
  } else {
    spr.setTextColor(TFT_RED, TFT_BLACK);
  }
  spr.drawString("< GPIO 21", 10, 213, 4);
  spr.setTextColor(TFT_WHITE, TFT_BLACK);
  spr.drawString(String(progress) + "% Demo", 75, 85, 4);
  progress++;
  if (progress >= 100)
    progress = 0;
  spr.drawRoundRect(21, 120, 218, 37, 6, TFT_GREEN);  //rectangle colour

  if (progress > 0 && progress < 35) {
    colour = 0x001F;  //BLUE
  }
  if (progress > 35 && progress < 75) {
    colour = 0x07E0;  //GREEN
  }
  if (progress > 75 && progress < 99) {
    colour = 0xF800;  //RED
  }
  if (progress > 99) {
    colour = 0x0000;  //BLACK
  }

  spr.fillRect(30, 128, progress * 2, 20, colour);

  lcd_PushColors(0, 0, WIDTH, HEIGHT, (uint16_t *)spr.getPointer());
  delay(100);
}
