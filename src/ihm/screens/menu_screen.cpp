#include "menu_screen.h"

#define BG_COLOR TFT_BLACK
#define TEXT_COLOR TFT_WHITE
#define HIGHLIGHT_COLOR TFT_YELLOW
#define FONT_SIZE 2

MenuScreen::MenuScreen(TFT_eSPI *display)
    : display(display) {
}

void MenuScreen::setCallbacks(ValueGetter getter, ValueSetter setter, Callback resetCb, Callback returnCb) {
  getValue = getter;
  setValue = setter;
  resetCallback = resetCb;
  returnToMainCallback = returnCb;
}

void MenuScreen::show() {
  render();
}

void MenuScreen::render() {
  display->fillScreen(BG_COLOR);
  display->setTextSize(FONT_SIZE);
  display->setTextColor(TFT_WHITE);

  switch (currentState) {
    case NAVIGATING:
      for (int i = 0; i < MAX_MENU_ITEMS; i++) {
        if (i == selectedIndex) {
          display->fillRect(10, 30 + i * 25, 220, 25, TFT_BLUE);
          display->setTextColor(TFT_WHITE, TFT_BLUE);
        } else {
          display->setTextColor(TFT_WHITE, BG_COLOR);
        }
        display->setCursor(15, 30 + i * 25);
        display->println(menuItems[i]);
      }
      break;

    case EDITING:
      display->setCursor(10, 100);
      display->println(menuItems[selectedIndex]);

      display->setCursor(10, 130);
      display->print("Valor atual: ");
      display->println(adjustValue);
      display->setCursor(10, 160);
      display->println("PGM p/ sair, UP p/ +, DOWN p/ -");
      break;

    case CONFIRM_RESET:
      display->setCursor(10, 100);
      display->println("Deseja resetar tudo?");
      display->setCursor(10, 130);
      display->println("UP = Sim, DOWN = Nao");
      break;
  }
}

void MenuScreen::navigateUp() {
  switch (currentState) {
    case NAVIGATING:
      selectedIndex = (selectedIndex - 1 + MAX_MENU_ITEMS) % MAX_MENU_ITEMS;
      render();
      break;

    case EDITING:
      adjustValue++;
      render();
      break;

    case CONFIRM_RESET:
      if (resetCallback) resetCallback();
      currentState = NAVIGATING;
      render();
      break;
  }
}

void MenuScreen::navigateDown() {
  switch (currentState) {
    case NAVIGATING:
      selectedIndex = (selectedIndex + 1) % MAX_MENU_ITEMS;
      render();
      break;

    case EDITING:
      adjustValue--;
      render();
      break;

    case CONFIRM_RESET:
      currentState = NAVIGATING;
      render();
      break;
  }
}

void MenuScreen::select() {
  switch (currentState) {
    case NAVIGATING:
      if (selectedIndex == RESET) {
        currentState = CONFIRM_RESET;
      } else if (selectedIndex == RETURN) {
        if (returnToMainCallback) returnToMainCallback();
        return;
      } else {
        currentState = EDITING;
        if (getValue) adjustValue = getValue(static_cast<MenuItem>(selectedIndex));
      }
      render();
      break;

    case EDITING:
      if (setValue) setValue(static_cast<MenuItem>(selectedIndex), adjustValue);
      currentState = NAVIGATING;
      render();
      break;

    case CONFIRM_RESET:
      currentState = NAVIGATING;
      render();
      break;
  }
}