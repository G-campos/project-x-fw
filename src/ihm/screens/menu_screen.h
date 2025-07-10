#ifndef MENUSCREEN_H
#define MENUSCREEN_H

#include <TFT_eSPI.h>
#include <SPI.h>
#include <functional>

class MenuScreen {
  public:
    enum MenuItem {
      VELO_MIN,
      MULTISPEED_1,
      MULTISPEED_2,
      RAMP_ACCEL,
      RAMP_DECEL,
      RESET,
      RETURN,
      MAX_MENU_ITEMS
    };

    enum State {
      NAVIGATING,
      EDITING,
      CONFIRM_RESET
    };

    // Tipos de função para callbacks
    using ValueGetter = std::function<int(MenuItem)>;
    using ValueSetter = std::function<void(MenuItem, int)>;
    using Callback = std::function<void()>;

    MenuScreen(TFT_eSPI* tft);

    void setCallbacks(ValueGetter getter, ValueSetter setter, Callback resetCb, Callback returnCb);

    void show();
    void navigateUp();
    void navigateDown();
    void select();

  private:
    TFT_eSPI* display;

    const char* menuItems[MAX_MENU_ITEMS] = {
      "Velo. Minima",
      "Multispeed 1",
      "Multispeed 2",
      "Ramp. Aceler",
      "Ramp. Desace",
      "Reset",
      "Voltar"
    };

    int selectedIndex = 0;
    int adjustValue = 0;
    State currentState = NAVIGATING;

    // Callbacks
    ValueGetter getValue;
    ValueSetter setValue;
    Callback resetCallback;
    Callback returnToMainCallback;

    void render();
};

#endif