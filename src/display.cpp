/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2021 Jason Skuby (mytechtoybox.com)
 */

#include <string>
#include "display.h"
#include "storage.h"
#include "pico/stdlib.h"
#include "OneBitDisplay.h"
#include <deque>

uint8_t ucBackBuffer[1024];
OBDISP obd;
string statusBar;
std::deque<string> history;
bool splashCheck = 0;

// For sleep/wake
unsigned long idleMillis;
unsigned long sleepWakeTimer;

uint8_t idleCheck = 0;
uint8_t sleepCheck;
uint8_t wakeCheck = 2;
#define numStars 16
uint8_t stars[numStars][2];

bool state_up = 0;
bool state_down = 0;
bool state_left = 0;
bool state_right = 0;

inline void clearScreen(int render = 0)
{
	obdFill(&obd, 0, render);
}

inline void drawHitbox(int startX, int startY, int buttonRadius, int buttonPadding, Gamepad *gamepad)
{
	const int buttonMargin = buttonPadding + (buttonRadius * 2);

	// UDLR
	obdPreciseEllipse(&obd, startX, startY, buttonRadius, buttonRadius, 1, gamepad->pressedLeft());
	obdPreciseEllipse(&obd, startX + buttonMargin, startY, buttonRadius, buttonRadius, 1, gamepad->pressedDown());
	obdPreciseEllipse(&obd, startX + (buttonMargin * 1.875), startY + (buttonMargin / 2), buttonRadius, buttonRadius, 1, gamepad->pressedRight());
	obdPreciseEllipse(&obd, startX + (buttonMargin * 2.25), startY + buttonMargin * 1.875, buttonRadius, buttonRadius, 1, gamepad->pressedUp());

	// 8-button
	obdPreciseEllipse(&obd, startX + (buttonMargin * 2.75), startY, buttonRadius, buttonRadius, 1, gamepad->pressedB3());
	obdPreciseEllipse(&obd, startX + (buttonMargin * 3.75), startY - (buttonMargin / 4), buttonRadius, buttonRadius, 1, gamepad->pressedB4());
	obdPreciseEllipse(&obd, startX + (buttonMargin * 4.75), startY - (buttonMargin / 4), buttonRadius, buttonRadius, 1, gamepad->pressedR1());
	obdPreciseEllipse(&obd, startX + (buttonMargin * 5.75), startY, buttonRadius, buttonRadius, 1, gamepad->pressedL1());

	obdPreciseEllipse(&obd, startX + (buttonMargin * 2.75), startY + buttonMargin, buttonRadius, buttonRadius, 1, gamepad->pressedB1());
	obdPreciseEllipse(&obd, startX + (buttonMargin * 3.75), startY + buttonMargin - (buttonMargin / 4), buttonRadius, buttonRadius, 1, gamepad->pressedB2());
	obdPreciseEllipse(&obd, startX + (buttonMargin * 4.75), startY + buttonMargin - (buttonMargin / 4), buttonRadius, buttonRadius, 1, gamepad->pressedR2());
	obdPreciseEllipse(&obd, startX + (buttonMargin * 5.75), startY + buttonMargin, buttonRadius, buttonRadius, 1, gamepad->pressedL2());
}

inline void drawWasdBox(int startX, int startY, int buttonRadius, int buttonPadding, Gamepad *gamepad)
{
	const int buttonMargin = buttonPadding + (buttonRadius * 2);

	// UDLR
	obdPreciseEllipse(&obd, startX, startY + buttonMargin * 0.5, buttonRadius, buttonRadius, 1, gamepad->pressedLeft());
	obdPreciseEllipse(&obd, startX + buttonMargin, startY + buttonMargin * 0.875, buttonRadius, buttonRadius, 1, gamepad->pressedDown());
	obdPreciseEllipse(&obd, startX + buttonMargin * 1.5, startY - buttonMargin * 0.125, buttonRadius, buttonRadius, 1, gamepad->pressedUp());
	obdPreciseEllipse(&obd, startX + (buttonMargin * 2), startY + buttonMargin * 1.25, buttonRadius, buttonRadius, 1, gamepad->pressedRight());

	// 8-button
	obdPreciseEllipse(&obd, startX + buttonMargin * 3.625, startY, buttonRadius, buttonRadius, 1, gamepad->pressedB3());
	obdPreciseEllipse(&obd, startX + buttonMargin * 4.625, startY - (buttonMargin / 4), buttonRadius, buttonRadius, 1, gamepad->pressedB4());
	obdPreciseEllipse(&obd, startX + buttonMargin * 5.625, startY - (buttonMargin / 4), buttonRadius, buttonRadius, 1, gamepad->pressedR1());
	obdPreciseEllipse(&obd, startX + buttonMargin * 6.625, startY, buttonRadius, buttonRadius, 1, gamepad->pressedL1());

	obdPreciseEllipse(&obd, startX + buttonMargin * 3.25, startY + buttonMargin, buttonRadius, buttonRadius, 1, gamepad->pressedB1());
	obdPreciseEllipse(&obd, startX + buttonMargin * 4.25, startY + buttonMargin - (buttonMargin / 4), buttonRadius, buttonRadius, 1, gamepad->pressedB2());
	obdPreciseEllipse(&obd, startX + buttonMargin * 5.25, startY + buttonMargin - (buttonMargin / 4), buttonRadius, buttonRadius, 1, gamepad->pressedR2());
	obdPreciseEllipse(&obd, startX + buttonMargin * 6.25, startY + buttonMargin, buttonRadius, buttonRadius, 1, gamepad->pressedL2());
}

inline void drawFightboard(int startX, int startY, int buttonRadius, int buttonPadding, bool invert, Gamepad *gamepad)
{
	const int buttonMargin = buttonPadding + (buttonRadius * 2);


std::string ret;

    // Check analog stick value
    //std::string num = std::to_string(gamepad->state.lx);
	//obdWriteString(&obd, 1, 0, 0, (char *)num.c_str(), FONT_6x8, 0, 0);

	// UDLR
    //obdPreciseEllipse(&obd, startX, startY + buttonMargin * 0.5, buttonRadius, buttonRadius, 1, !gpio_get(PIN_DPAD_LEFT));
    //obdPreciseEllipse(&obd, startX + buttonMargin, startY + buttonMargin * 0.875, buttonRadius, buttonRadius, 1, !gpio_get(PIN_DPAD_DOWN));
    //obdPreciseEllipse(&obd, startX + buttonMargin * 1.5, startY - buttonMargin * 0.125, buttonRadius, buttonRadius, 1, !gpio_get(PIN_DPAD_UP));
    //obdPreciseEllipse(&obd, startX + (buttonMargin * 2), startY + buttonMargin * 1.25, buttonRadius, buttonRadius, 1, !gpio_get(PIN_DPAD_RIGHT));
    obdPreciseEllipse(&obd, startX + buttonMargin, startY + (buttonMargin / 2), buttonRadius * 1.5, buttonRadius * 1.5, 1, 0);
    obdPreciseEllipse(&obd, 
            startX + buttonMargin + (buttonMargin/2 * state_right) - (buttonMargin/2 * state_left),
            startY + (buttonMargin / 2) + (buttonMargin/2 * state_down) - (buttonMargin/2 * state_up),
            buttonRadius * 1.25,
            buttonRadius * 1.25,
            1,
            1);
	
	// 8-button
	obdPreciseEllipse(&obd, startX + buttonMargin * 3.625, startY * 1.25, buttonRadius, buttonRadius, 1, gamepad->pressedB3());
	obdPreciseEllipse(&obd, startX + buttonMargin * 4.625, startY - (buttonMargin / 4), buttonRadius, buttonRadius, 1, gamepad->pressedB4());
	obdPreciseEllipse(&obd, startX + buttonMargin * 5.625, startY - (buttonMargin / 4), buttonRadius, buttonRadius, 1, gamepad->pressedR1());
	obdPreciseEllipse(&obd, startX + buttonMargin * 6.625, startY - (buttonMargin / 4), buttonRadius, buttonRadius, 1, gamepad->pressedL1());

	obdPreciseEllipse(&obd, startX + buttonMargin * 3.625, startY + buttonMargin * 1.25, buttonRadius, buttonRadius, 1, gamepad->pressedB1());
	obdPreciseEllipse(&obd, startX + buttonMargin * 4.625, startY + buttonMargin - (buttonMargin / 4), buttonRadius, buttonRadius, 1, gamepad->pressedB2());
	obdPreciseEllipse(&obd, startX + buttonMargin * 5.625, startY + buttonMargin - (buttonMargin / 4), buttonRadius, buttonRadius, 1, gamepad->pressedR2());
	obdPreciseEllipse(&obd, startX + buttonMargin * 6.625, startY + buttonMargin - (buttonMargin / 4), buttonRadius, buttonRadius, 1, gamepad->pressedL2());

    // Extra buttons
	obdPreciseEllipse(&obd, startX + buttonMargin * 4.5, startY + (buttonMargin * 1.5), 3, 3, 1, gamepad->pressedL3());
	obdPreciseEllipse(&obd, startX + buttonMargin * 5.0625, startY + (buttonMargin * 1.5), 3, 3, 1, gamepad->pressedS1());
	obdPreciseEllipse(&obd, startX + buttonMargin * 5.625, startY + (buttonMargin * 1.5), 3, 3, 1, gamepad->pressedA1());
	obdPreciseEllipse(&obd, startX + buttonMargin * 6.125+0.0625, startY + (buttonMargin * 1.5), 3, 3, 1, gamepad->pressedS2());
	obdPreciseEllipse(&obd, startX + buttonMargin * 6.75, startY + (buttonMargin * 1.5), 3, 3, 1, gamepad->pressedR3());
}

inline void drawFightboardMirror(int startX, int startY, int buttonRadius, int buttonPadding, Gamepad *gamepad)
{
	const int buttonMargin = buttonPadding + (buttonRadius * 2);

    startX = 128 - startX;

    obdPreciseEllipse(&obd, startX - buttonMargin, startY + (buttonMargin / 2), buttonRadius * 1.5, buttonRadius * 1.5, 1, 0);
    obdPreciseEllipse(&obd, 
            startX - buttonMargin + (buttonMargin/2 * state_right) - (buttonMargin/2 * state_left),
            startY + (buttonMargin / 2) + (buttonMargin/2 * state_down) - (buttonMargin/2 * state_up),
            buttonRadius * 1.25,
            buttonRadius * 1.25,
            1,
            1);

	// UDLR
	//obdPreciseEllipse(&obd, startX, startY + buttonMargin * 0.5, buttonRadius, buttonRadius, 1, gamepad->pressedRight());
	//obdPreciseEllipse(&obd, startX - buttonMargin, startY + buttonMargin * 0.875, buttonRadius, buttonRadius, 1, gamepad->pressedDown());
	//obdPreciseEllipse(&obd, startX - buttonMargin * 1.5, startY - buttonMargin * 0.125, buttonRadius, buttonRadius, 1, gamepad->pressedUp());
	//obdPreciseEllipse(&obd, startX - (buttonMargin * 2), startY + buttonMargin * 1.25, buttonRadius, buttonRadius, 1, gamepad->pressedLeft());

	// 8-button
	obdPreciseEllipse(&obd, startX - buttonMargin * 3.625, startY * 1.25, buttonRadius, buttonRadius, 1, gamepad->pressedB3());
	obdPreciseEllipse(&obd, startX - buttonMargin * 4.625, startY - (buttonMargin / 4), buttonRadius, buttonRadius, 1, gamepad->pressedB4());
	obdPreciseEllipse(&obd, startX - buttonMargin * 5.625, startY - (buttonMargin / 4), buttonRadius, buttonRadius, 1, gamepad->pressedR1());
	obdPreciseEllipse(&obd, startX - buttonMargin * 6.625, startY - (buttonMargin / 4), buttonRadius, buttonRadius, 1, gamepad->pressedL1());

	obdPreciseEllipse(&obd, startX - buttonMargin * 3.625, startY + buttonMargin * 1.25, buttonRadius, buttonRadius, 1, gamepad->pressedB1());
	obdPreciseEllipse(&obd, startX - buttonMargin * 4.625, startY + buttonMargin - (buttonMargin / 4), buttonRadius, buttonRadius, 1, gamepad->pressedB2());
	obdPreciseEllipse(&obd, startX - buttonMargin * 5.625, startY + buttonMargin - (buttonMargin / 4), buttonRadius, buttonRadius, 1, gamepad->pressedR2());
	obdPreciseEllipse(&obd, startX - buttonMargin * 6.625, startY + buttonMargin - (buttonMargin / 4), buttonRadius, buttonRadius, 1, gamepad->pressedL2());

    // Extra buttons
	obdPreciseEllipse(&obd, startX - buttonMargin * 4.5, startY + (buttonMargin * 1.5), 3, 3, 1, gamepad->pressedR3());
	obdPreciseEllipse(&obd, startX - buttonMargin * 5.0625, startY + (buttonMargin * 1.5), 3, 3, 1, gamepad->pressedS2());
	obdPreciseEllipse(&obd, startX - buttonMargin * 5.625, startY + (buttonMargin * 1.5), 3, 3, 1, gamepad->pressedA1());
	obdPreciseEllipse(&obd, startX - buttonMargin * 6.125+0.0625, startY + (buttonMargin * 1.5), 3, 3, 1, gamepad->pressedS1());
	obdPreciseEllipse(&obd, startX - buttonMargin * 6.75, startY + (buttonMargin * 1.5), 3, 3, 1, gamepad->pressedL3());
}

inline void drawArcadeStick(int startX, int startY, int buttonRadius, int buttonPadding, Gamepad *gamepad)
{
	const int buttonMargin = buttonPadding + (buttonRadius * 2);

	// UDLR
	obdPreciseEllipse(&obd, startX, startY + buttonMargin / 2, buttonRadius, buttonRadius, 1, gamepad->pressedLeft());
	obdPreciseEllipse(&obd, startX + (buttonMargin * 0.875), startY - (buttonMargin / 4), buttonRadius, buttonRadius, 1, gamepad->pressedUp());
	obdPreciseEllipse(&obd, startX + (buttonMargin * 0.875), startY + buttonMargin * 1.25, buttonRadius, buttonRadius, 1, gamepad->pressedDown());
	obdPreciseEllipse(&obd, startX + (buttonMargin * 1.625), startY + buttonMargin / 2, buttonRadius, buttonRadius, 1, gamepad->pressedRight());

	// 8-button
	obdPreciseEllipse(&obd, startX + buttonMargin * 3.125, startY, buttonRadius, buttonRadius, 1, gamepad->pressedB3());
	obdPreciseEllipse(&obd, startX + buttonMargin * 4.125, startY - (buttonMargin / 4), buttonRadius, buttonRadius, 1, gamepad->pressedB4());
	obdPreciseEllipse(&obd, startX + buttonMargin * 5.125, startY - (buttonMargin / 4), buttonRadius, buttonRadius, 1, gamepad->pressedR1());
	obdPreciseEllipse(&obd, startX + buttonMargin * 6.125, startY, buttonRadius, buttonRadius, 1, gamepad->pressedL1());

	obdPreciseEllipse(&obd, startX + buttonMargin * 2.875, startY + buttonMargin, buttonRadius, buttonRadius, 1, gamepad->pressedB1());
	obdPreciseEllipse(&obd, startX + buttonMargin * 3.875, startY + buttonMargin - (buttonMargin / 4), buttonRadius, buttonRadius, 1, gamepad->pressedB2());
	obdPreciseEllipse(&obd, startX + buttonMargin * 4.875, startY + buttonMargin - (buttonMargin / 4), buttonRadius, buttonRadius, 1, gamepad->pressedR2());
	obdPreciseEllipse(&obd, startX + buttonMargin * 5.875, startY + buttonMargin, buttonRadius, buttonRadius, 1, gamepad->pressedL2());
}

inline void drawSplashScreen() // There has got to be a better way to do this.
{
	const unsigned char splashImage[] = {
        // 'logo', 128x64px
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 
        0x28, 0x08, 0x00, 0xf8, 0x00, 0xf8, 0x88, 0xc8, 0x00, 0xf8, 0x20, 0xf8, 0x00, 0x08, 0xf8, 0x08, 
        0x00, 0xf8, 0xa8, 0x50, 0x00, 0x70, 0x88, 0x70, 0x00, 0xf0, 0x48, 0xf0, 0x00, 0xf8, 0x28, 0xd0, 
        0x00, 0xf8, 0x88, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x40, 0x20, 0x20, 0x20, 0x10, 0x10, 
        0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x20, 0x20, 0x20, 0x40, 0x40, 0x80, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x80, 0x40, 0x40, 0x20, 0x20, 0x20, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 
        0x10, 0x10, 0x20, 0x20, 0x20, 0x40, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x80, 0x60, 0x10, 0x0c, 0x02, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x01, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01, 0x01, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x02, 0x0c, 0x10, 0x60, 0x80, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0xfc, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 
        0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xfc, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x1f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x1f, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x03, 0x04, 0x18, 0x20, 0x40, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 
        0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x40, 0x40, 
        0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x40, 0x20, 0x18, 0x04, 0x03, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x02, 0x02, 0x02, 0x04, 0x04, 
        0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x02, 0x02, 0x02, 0x01, 0x01, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x01, 0x01, 0x02, 0x02, 0x02, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 
        0x04, 0x04, 0x02, 0x02, 0x02, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x02, 0x07, 0x0a, 0x00, 0x0f, 0x04, 0x0c, 0x00, 0x0c, 0x04, 0x0c, 0x00, 
        0x0d, 0x00, 0x0f, 0x04, 0x0a, 0x00, 0x0f, 0x04, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};

	int i;
    for (i=0; i<sizeof(splashImage); i++) {
        obdDrawGFX(&obd, (uint8_t *)&splashImage[128 * i], 0, 0, 0, i, 128, 1, 1);
    }
}

inline void drawStatusBar()
{
	obdWriteString(&obd, 0, 0, 0, (char *)statusBar.c_str(), FONT_6x8, 0, 0);
}

inline void drawHistory()
{
	std::string ret;

	for (auto it = history.crbegin(); it != history.crend(); ++it) {
		if (ret.size() < 22) {
			std::string newRet = ret;
			if (!newRet.empty())
				newRet = " " + newRet;

			newRet = *it + newRet;

			if (newRet.size() < 22) {
				ret = newRet;
			}
			else {
				break;
			}
  		}
	}

	obdWriteString(&obd, 0, 0, 7, (char *)ret.c_str(), FONT_6x8, 0, 0);
}

inline void drawSleep()
{
    if (getMillis() - idleMillis - IDLE_TIMEOUT_MS < 2000) {
    // Clear buffer
    clearScreen(0);

    // Draw stars
    for (uint8_t x=0;x<numStars;x++) {
        uint8_t starSize = rand() % 3;
        obdDrawLine(&obd, stars[x][0] - starSize, stars[x][1], stars[x][0]+starSize, stars[x][1], 1, 0);
        obdDrawLine(&obd, stars[x][0], stars[x][1]-starSize, stars[x][0], stars[x][1]+starSize, 1, 0);
    }

    // Draw moon
    obdEllipse(&obd, 64, 32, 28, 28, 1, 1);
    obdEllipse(&obd, 70, 26, 25, 25, 0, 1);

    // Draw buffer
    obdDumpBuffer(&obd, NULL);
    } else clearScreen(1);
    //if (getMillis() - sleepWakeTimer > 2000) {
        //clearScreen(1);
        //sleepCheck = 2; 
    //}
}

void setStatusBar(Gamepad *gamepad)
{
	// Limit to 21 chars with 6x8 font for now
	statusBar.clear();

	switch (gamepad->options.inputMode)
	{
		case INPUT_MODE_HID:    statusBar += "DINPUT"; break;
		case INPUT_MODE_SWITCH: statusBar += "SWITCH"; break;
		case INPUT_MODE_XINPUT: statusBar += "XINPUT"; break;
		case INPUT_MODE_CONFIG: statusBar += "CONFIG"; break;
	}

	switch (gamepad->options.dpadMode)
	{

		case DPAD_MODE_DIGITAL:      statusBar += "         DPAD"; break;
		case DPAD_MODE_LEFT_ANALOG:  statusBar += "         LEFT"; break;
		case DPAD_MODE_RIGHT_ANALOG: statusBar += "        RIGHT"; break;
	}

	switch (gamepad->options.socdMode)
	{
		case SOCD_MODE_NEUTRAL:               statusBar += "-N"; break;
		case SOCD_MODE_UP_PRIORITY:           statusBar += "-U"; break;
		case SOCD_MODE_SECOND_INPUT_PRIORITY: statusBar += "-L"; break;
	}
}

// Get universal directional keys for SOCD cleaned outputs
void getSOCD(Gamepad *gamepad){
    
    // Get the dpad mode
	switch (gamepad->options.dpadMode)
	{
		case DPAD_MODE_DIGITAL:
            state_up = gamepad->pressedUp();
            state_down = gamepad->pressedDown();
            state_left = gamepad->pressedLeft();
            state_right = gamepad->pressedRight();
            break;
		case DPAD_MODE_LEFT_ANALOG:
            if (gamepad->state.ly < 32767) state_up = 1;
            else state_up = 0;
            if (gamepad->state.ly > 32767) state_down = 1;
            else state_down = 0;
            if (gamepad->state.lx > 32767) state_right = 1;
            else state_right = 0;
            if (gamepad->state.lx < 32767) state_left = 1;
            else state_left = 0;
            break;
		case DPAD_MODE_RIGHT_ANALOG:
            if (gamepad->state.ry < 32767) state_up = 1;
            else state_up = 0;
            if (gamepad->state.ry > 32767) state_down = 1;
            else state_down = 0;
            if (gamepad->state.rx > 32767) state_right = 1;
            else state_right = 0;
            if (gamepad->state.rx < 32767) state_left = 1;
            else state_left = 0;
            break;
	}
}

void setHistory(Gamepad *gamepad)
{
	std::deque<string> pressed;

	if (state_up)
		pressed.push_back("U");

	if (state_down)
		pressed.push_back("D");

	if (state_left)
		pressed.push_back("L");

	if (state_right)
		pressed.push_back("R");

	if (gamepad->pressedB1())
		pressed.push_back("1K");

	if (gamepad->pressedB2())
		pressed.push_back("2K");

	if (gamepad->pressedR2())
		pressed.push_back("3K");

	if (gamepad->pressedL2())
		pressed.push_back("4K");

	if (gamepad->pressedB3())
		pressed.push_back("1P");

	if (gamepad->pressedB4())
		pressed.push_back("2P");

	if (gamepad->pressedR1())
		pressed.push_back("3P");

	if (gamepad->pressedL1())
		pressed.push_back("4P");

	if (gamepad->pressedL3())
		pressed.push_back("L3");

	if (gamepad->pressedS1())
		pressed.push_back("S1");

	if (gamepad->pressedA1())
		pressed.push_back("A1");

	if (gamepad->pressedS2())
		pressed.push_back("S2");

	if (gamepad->pressedR3())
		pressed.push_back("R3");

	if (pressed.size() > 0 || state_up || state_down || state_left ||state_right)  {
	    std::string newInput;
	    for(const auto &s : pressed) {
	        if(!newInput.empty())
	            newInput += "+";
	        newInput += s;
	    }

		if (history.empty() || history.back() != newInput) {
			history.push_back(newInput);
		}
        idleMillis = getMillis();
        idleCheck = 0;
	}

	if (history.size() > 10) {
		history.pop_front();
	}
}

void DisplayModule::setup()
{
	BoardOptions options = getBoardOptions();
	enabled = options.hasI2CDisplay && options.i2cSDAPin != -1 && options.i2cSCLPin != -1;
	if (enabled)
	{
		obdI2CInit(&obd,
			options.displaySize,
			options.displayI2CAddress,
			options.displayFlip,
			options.displayInvert,
			DISPLAY_USEWIRE,
			options.i2cSDAPin,
			options.i2cSCLPin,
			options.i2cBlock == 0 ? i2c0 : i2c1,
			-1,
			options.i2cSpeed);

		obdSetContrast(&obd, 0xFF);
		obdSetBackBuffer(&obd, ucBackBuffer);
		clearScreen(1);
	}
}

void DisplayModule::loop()
{
	// All screen updates should be handled in process() as they need to display ASAP
}

void DisplayModule::process(Gamepad *gamepad)
{
    getSOCD(gamepad);
    if (getMillis() < 2000) {
        if (splashCheck == 0){
            drawSplashScreen();
            splashCheck = 1;
        }
    } else if (getMillis() - idleMillis > IDLE_TIMEOUT_MS) {
        // Check history to update idleMillis with keypresses (this should be done somewhere else)
        if (sleepCheck == 0) {
            for (uint8_t x=0;x<numStars;x++){
                for(uint8_t y=0;y<2;y++){
                    if (y==0) stars[x][y] = rand() % 128;
                    if (y==1) stars[x][y] = rand() % 64;
                }
            }
            //sleepWakeTimer = getMillis(); // Reset the timer
            //clearScreen(1); // Clear the screen
            sleepCheck = 1; // Get out of here stalker
        }
        //if (sleepCheck == 1) {
            drawSleep();
        //}
        setHistory(gamepad);
    } else {
        sleepCheck = 0;
		clearScreen();

        setStatusBar(gamepad);

        drawStatusBar();
        setHistory(gamepad);
        drawHistory();
        switch (BUTTON_LAYOUT)
        {
            case BUTTON_LAYOUT_ARCADE:
                drawArcadeStick(8, 28, 8, 2, gamepad);
                break;

            case BUTTON_LAYOUT_HITBOX:
                drawHitbox(8, 20, 8, 2, gamepad);
                break;

            case BUTTON_LAYOUT_WASD:
                drawWasdBox(8, 22, 7, 3, gamepad);
                break;
            case BUTTON_LAYOUT_FIGHTBOARD:
                drawFightboard(8, 22, 7, 3, 0, gamepad);
                break;
            case BUTTON_LAYOUT_FIGHTBOARD_MIRROR:
                drawFightboardMirror(8, 22, 7, 3, gamepad);
                //drawFightboard(8, 22, 7, 3, 1, gamepad);
                break;
        }

        obdDumpBuffer(&obd, NULL);
    }
}
