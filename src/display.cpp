/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2021 Jason Skuby (mytechtoybox.com)
 */

#include <string>
#include "display.h"
#include "storage.h"
#include "pico/stdlib.h"
#include "OneBitDisplay.h"
#include "AnimationStation.hpp"
#include <deque>
#include <vector>

uint8_t ucBackBuffer[1024];
OBDISP obd;
string statusBar;
std::deque<string> history;
AnimationStation ledSettings;
uint8_t selection=0;
uint8_t menuDepth=0;

// For sleep/wake
unsigned long idleMillis;
unsigned long sleepWakeTimer;

uint8_t idleCheck = 0;
uint8_t sleepCheck;
uint8_t wakeCheck = 2;
#define numStars 16
uint8_t stars[numStars][2];

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

inline void drawFightboard(int startX, int startY, int buttonRadius, int buttonPadding, Gamepad *gamepad)
{
	const int buttonMargin = buttonPadding + (buttonRadius * 2);

	// UDLR
	obdPreciseEllipse(&obd, startX, startY + buttonMargin * 0.5, buttonRadius, buttonRadius, 1, gamepad->pressedLeft());
	obdPreciseEllipse(&obd, startX + buttonMargin, startY + buttonMargin * 0.875, buttonRadius, buttonRadius, 1, gamepad->pressedDown());
	obdPreciseEllipse(&obd, startX + buttonMargin * 1.5, startY - buttonMargin * 0.125, buttonRadius, buttonRadius, 1, gamepad->pressedUp());
	obdPreciseEllipse(&obd, startX + (buttonMargin * 2), startY + buttonMargin * 1.25, buttonRadius, buttonRadius, 1, gamepad->pressedRight());

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

vector<vector<string>> menu = {
    {"LED modes","Off", "Static color", "Rainbow cycle", "Rainbow chase", "Static theme"},
    {"Brightness", "25%", "50%", "75%", "100%"},
    {"Press color", "Black", "White", "Red", "Orange", "Yellow", "LimeGreen", "Green", "Seafoam", "Aqua", "SkyBlue", "Blue", "Purple", "Pink", "Magenta" },
    {"Theme", "Rainbow", "Xbox", "Xbox (All)", "Super Famicom", "Super Famicom (All)"},
    {"Input mode", "Switch", "XInput", "Directinput"},
    {"Directional mode", "D-pad", "Left analog", "Right analog"},
    {"SOCD mode", "Neutral", "Up priority", "Last input priority"}
};

inline void drawMainMenu(uint8_t sel)
{
    clearScreen(0);
    uint8_t length = menu.size();
    uint8_t offset = 0;
    obdWriteString(&obd, 0, 0, 0, (char *)"Main menu", FONT_12x16, 0, 0);

    uint8_t cap = 6;
    if (length >= 6 && sel >= 6) offset = ((sel)/ 6) * 6;
    if (length - offset < 6) cap = (length)%6;

    for (uint8_t i=0; i<cap; i++) {
        bool invert = 0;
        if (i == sel-offset) invert=1;
        obdWriteString(&obd, 0, 0, i+2, (char *)menu[i+offset][0].c_str(), FONT_6x8, invert, 0);
    }

    obdDumpBuffer(&obd, NULL);
}

uint8_t scrollpos = 0;
inline void drawMenu(uint8_t index, uint8_t active, uint8_t sel, Gamepad *gamepad)
{
    clearScreen();
    uint8_t length = menu[index].size();
    uint8_t offset = 0;
    //uint8_t titleLength = menu[index][0].length();
    //if (titleLength > 10) {
        //if (getMillis()%10 == 0) {
            //if (scrollpos <= (titleLength*12)-128) scrollpos++;
            //if (scrollpos == (titleLength*12)-128) scrollpos = 0;
        //}
    //}
    obdWriteString(&obd, scrollpos, 0, 0, (char *)menu[index][0].c_str(), FONT_12x16, 0, 0);

    uint8_t cap = 6;
    if (length > 7 && sel >= 6) offset = ((sel)/ 6) * 6;
    // This needs to be greater than 7 because of the index
    if (length - offset < 7) cap = (length-1)%6;

    for (uint8_t i=0; i<cap; i++) {
        bool invert = 0;
        if (i == sel-offset) invert=1;
        string line = menu[index][1+i+offset];
        if (i == active-offset) line+="*";
                                                            // +1 to ignore label entry, i for index, and offset to jump to a page
        obdWriteString(&obd, 0, 0, i+2, (char *)line.c_str(), FONT_6x8, invert, 0);
    }

    obdDumpBuffer(&obd, NULL);
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

void setHistory(Gamepad *gamepad)
{
	std::deque<string> pressed;

	if (gamepad->pressedUp())
		pressed.push_back("U");

	if (gamepad->pressedDown())
		pressed.push_back("D");

	if (gamepad->pressedLeft())
		pressed.push_back("L");

	if (gamepad->pressedRight())
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

	if (pressed.size() > 0) {
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

bool pressedNav[4];
bool splashCheck = 0;
uint8_t menuIndex = 0;
void DisplayModule::process(Gamepad *gamepad)
{
    if (gamepad->pressedL3() && gamepad->pressedR3() && menuDepth == 0) menuDepth = 1;
    if (gamepad->pressedLeft() && pressedNav[3] == 0 && menuDepth > 0){
        pressedNav[3] = 1;
        menuDepth -= 1;
        selection = 0;
        if (menuIndex > 0) menuIndex = 0;
    }
    if (!gamepad->pressedLeft()) pressedNav[3]=0;

    if (getMillis() < 2000) {
        if (splashCheck == 0){
            drawSplashScreen();
            splashCheck = 1;
        }
<<<<<<< HEAD
	} if (menuDepth == 1) {
        uint8_t length = menu.size();
        if (gamepad->pressedUp() && pressedNav[0] == 0){
            pressedNav[0] = 1;
            if (selection > 0) selection-=1;
        }
        if (!gamepad->pressedUp()) pressedNav[0]=0;
        if (gamepad->pressedDown() && pressedNav[1] == 0) {
            pressedNav[1] = 1;
            //
            if (selection < length-1) selection+=1;
        }
        if (!gamepad->pressedDown()) pressedNav[1]=0;
        if (gamepad->pressedRight() && pressedNav[2] == 0) {
            pressedNav[2] = 1;
            // If moving deeper
            if (menuDepth == 1) {
                menuDepth = 2;
                menuIndex = selection;
                selection = 0;
            } else {
                // Change settings here with function
            }
        }
        if (!gamepad->pressedRight()) pressedNav[2]=0;
        drawMainMenu(selection);
    }

    else if (menuDepth > 1) {
        uint8_t length = menu[menuIndex].size();

        if (gamepad->pressedUp() && pressedNav[0] == 0){
            pressedNav[0] = 1;
            if (selection > 0) selection-=1;
        }
        if (!gamepad->pressedUp()) pressedNav[0]=0;
        if (gamepad->pressedDown() && pressedNav[1] == 0) {
            pressedNav[1] = 1;
            if (selection < length-2) selection+=1;
        }
        if (!gamepad->pressedDown()) pressedNav[1]=0;
        if (gamepad->pressedRight() && pressedNav[2] == 0) {
            pressedNav[2] = 1;
            // Change settings here with function
        }
        if (!gamepad->pressedRight()) pressedNav[2]=0;

        // Get active settings for index
        int active = -1;
        switch (menuIndex) {
            // LED mode
            case 0:
                active = ledSettings.GetMode();
                break;
            case 4:
                switch (gamepad->options.inputMode){
                    case INPUT_MODE_SWITCH:     active = 0; break;
                    case INPUT_MODE_XINPUT:     active = 1; break;
                    case INPUT_MODE_HID:        active = 2; break;
                }
                break;
            case 5:
                switch (gamepad->options.dpadMode){
                    case DPAD_MODE_DIGITAL:      active = 0; break;
                    case DPAD_MODE_LEFT_ANALOG:  active = 1; break;
                    case DPAD_MODE_RIGHT_ANALOG: active = 2; break;
                }
                break;
            case 6:
                switch (gamepad->options.socdMode)
                {
                    case SOCD_MODE_NEUTRAL:               active = 0; break;
                    case SOCD_MODE_UP_PRIORITY:           active = 1; break;
                    case SOCD_MODE_SECOND_INPUT_PRIORITY: active = 2; break;
                }
                break;

        }



        drawMenu(menuIndex, active, selection, gamepad);

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
                drawFightboard(8, 22, 7, 3, gamepad);
                break;
        }

        obdDumpBuffer(&obd, NULL);
    }
}
