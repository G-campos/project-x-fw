#include "display.h"

#include "config.h"

#include <SPI.h>
#include <TFT_eSPI.h>
#include <PushButton.h>

// enum rotation {
//   LANDSCAPE,
//   RIGHT_PORTRAIT,
//   LEFT_PORTRAIT,
//   INVERTED_LANDSCAPE
// };

// uint8_t rotation = LANDSCAPE;

TFT_eSPI tft = TFT_eSPI();

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define FONT_SIZE 2

PushButton bot1(35);
PushButton bot2(34);
PushButton bot3(39);

static long corBot[3] = {OFF, OFF, OFF};

void intro_screen() {
  
  tft.setCursor (55, 80);
  tft.setTextSize (3);
  tft.setTextColor(WHITE);
  tft.println("MENU");
  
  tft.setCursor (30, 120);
  tft.println("COM ARDUINO");
  
  tft.setCursor (30, 180);
  tft.setTextSize (2);
  tft.setTextColor(BLUE);
  tft.println("Brincando");

  tft.setCursor (30, 200);
  tft.setTextColor(WHITE);
  tft.println("    com");
  
  tft.setCursor (30, 220);
  tft.setTextColor(RED);
  tft.println("       Ideias");
  
  delay(3500);

  tft.fillScreen(BLACK);
}

void draw_buttons(int type) {
  switch (type) {
    case 1:
      tft.fillRect  (40, 20 , 160, 80, corBot[0]);

      tft.setTextColor(BLACK);
      tft.setTextSize (3);
      tft.setCursor(50, 50);
      tft.println(" QUARTO");
      break;

    case 2:
      tft.fillRect  (40, 120, 160, 80, corBot[1]);

      tft.setTextColor(BLACK);
      tft.setTextSize (3);
      tft.setCursor(50, 150);
      tft.println("  SALA ");
      break;

    case 3:
      tft.fillRect  (40, 220, 160, 80, corBot[2]);

      tft.setTextColor(BLACK);
      tft.setTextSize (3);
      tft.setCursor(60, 250);
      tft.println("COZINHA");
      break;

    default:
      tft.fillRect  (40, 20 , 160, 80, corBot[0]);
      tft.fillRect  (40, 120, 160, 80, corBot[1]);
      tft.fillRect  (40, 220, 160, 80, corBot[2]);

      tft.setTextColor(BLACK);
      tft.setTextSize (3);

      tft.setCursor(50, 50);
      tft.println(" QUARTO");

      tft.setCursor(50, 150);
      tft.println("  SALA ");

      tft.setCursor(60, 250);
      tft.println("COZINHA");
  }

}

void DetectButtons() {
  const char* TAG = "detect_buttons";

  if (bot1.pressed()) { // LOGICA PARA O BOTAO 1
    logger.info(TAG, "Button 1 pressionado");
    if (corBot[0] == ON) corBot[0] = OFF;
    else corBot[0] = ON;
    draw_buttons(1);
  } 
  
  if (bot2.pressed()) { // LOGICA PARA O BOTAO 2
    logger.info(TAG, "Button 2 pressionado");
    if (corBot[1] == ON) corBot[1] = OFF;
    else corBot[1] = ON;
    draw_buttons(2);
  }
  
  if (bot3.pressed()) { // LOGICA PARA O BOTAO 3
    logger.info(TAG, "Button 3 pressionado");
    if (corBot[2] == ON) corBot[2] = OFF;
    else corBot[2] = ON;
    draw_buttons(3);
  }

}

void display_init(){
  const char* TAG = "display_init";
  logger.info(TAG, "Inicializando o display...");
  
  tft.init();
  tft.setRotation(3); // 0-3
  tft.fillScreen(TFT_BLACK);

  intro_screen();
  draw_buttons(0);

  logger.info(TAG, "Display inicializado.");
}

void display_loop(){
  DetectButtons();
}
