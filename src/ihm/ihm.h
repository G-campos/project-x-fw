#ifndef IHM_H
#define IHM_H

#include <Arduino.h>

enum Screen {
	SPLASH,
	MAIN,
	QRCODE,
	MENU
};

enum Screen screenShowing = SPLASH;

void ihm_init();
void ihm_loop();

#endif // IHM_H