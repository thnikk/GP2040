/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2021 Jason Skuby (mytechtoybox.com)
 *
 * Config for a Fightboard v3 using an Adafruit KB2040.
 *
 */

#ifndef FIGHTBOARD_CONFIG_H_
#define FIGHTBOARD_CONFIG_H_

#include <vector>
#include <GamepadEnums.h>
#include <GamepadState.h>
#include "Pixel.hpp"
#include "enums.h"

#define PIN_DPAD_UP    0
#define PIN_DPAD_LEFT  1
#define PIN_DPAD_DOWN  2
#define PIN_DPAD_RIGHT 3
#define PIN_BUTTON_B3  28
#define PIN_BUTTON_B4  27
#define PIN_BUTTON_R1  26
#define PIN_BUTTON_R2  18
#define PIN_BUTTON_B1  20
#define PIN_BUTTON_B2  19
#define PIN_BUTTON_L1  10
#define PIN_BUTTON_L2  9
#define PIN_BUTTON_S1  5
#define PIN_BUTTON_S2  7
#define PIN_BUTTON_L3  8
#define PIN_BUTTON_R3  4
#define PIN_BUTTON_A1  6
#define PIN_BUTTON_A2  11

#define DEFAULT_SOCD_MODE SOCD_MODE_NEUTRAL
#define BUTTON_LAYOUT BUTTON_LAYOUT_WASD

#define BOARD_LEDS_PIN 29

#define LED_BRIGHTNESS_MAXIMUM 255
#define LED_BRIGHTNESS_STEPS 5
#define LED_FORMAT LED_FORMAT_GRB
#define LEDS_PER_PIXEL 1

#define LEDS_DPAD_LEFT  10
#define LEDS_DPAD_DOWN   9
#define LEDS_DPAD_RIGHT  8
#define LEDS_DPAD_UP    11
#define LEDS_BUTTON_B3   0
#define LEDS_BUTTON_B4   1
#define LEDS_BUTTON_R1   2
#define LEDS_BUTTON_R2   3
#define LEDS_BUTTON_L1   5
#define LEDS_BUTTON_L2   4
#define LEDS_BUTTON_B1   7
#define LEDS_BUTTON_B2   6

#define HAS_I2C_DISPLAY 0
#define I2C_SDA_PIN 12
#define I2C_SCL_PIN 13
#define I2C_BLOCK i2c0
#define I2C_SPEED 800000

#endif
