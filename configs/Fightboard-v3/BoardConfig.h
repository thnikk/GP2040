/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2021 Jason Skuby (mytechtoybox.com)
 *
 * Config for a Fightboard v3 using an RP2040 Zero.
 *
 */

#ifndef FIGHTBOARD_CONFIG_H_
#define FIGHTBOARD_CONFIG_H_

#include <vector>
#include <GamepadEnums.h>
#include <GamepadState.h>
#include "Pixel.hpp"
#include "enums.h"
#include "PlayerLEDs.h"

#define PIN_DPAD_UP    29
#define PIN_DPAD_LEFT  28
#define PIN_DPAD_DOWN  27
#define PIN_DPAD_RIGHT 26
#define PIN_BUTTON_B3  1
#define PIN_BUTTON_B4  2
#define PIN_BUTTON_R1  3
#define PIN_BUTTON_R2  7
#define PIN_BUTTON_B1  5
#define PIN_BUTTON_B2  6
#define PIN_BUTTON_L1  4
#define PIN_BUTTON_L2  8
#define PIN_BUTTON_S1  10
#define PIN_BUTTON_S2  12
#define PIN_BUTTON_L3  9
#define PIN_BUTTON_R3  13
#define PIN_BUTTON_A1  11
#define PIN_BUTTON_A2  17 // Not connected

#define DEFAULT_SOCD_MODE SOCD_MODE_NEUTRAL
#define BUTTON_LAYOUT BUTTON_LAYOUT_FIGHTBOARD

#define BOARD_LEDS_PIN 0

#define LED_BRIGHTNESS_MAXIMUM 100
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
#define LEDS_BUTTON_R2   5
#define LEDS_BUTTON_L1   3
#define LEDS_BUTTON_L2   4
#define LEDS_BUTTON_B1   7
#define LEDS_BUTTON_B2   6

// Use built-in LED for P1 indicator
#define PLED_TYPE PLED_TYPE_PWM
#define PLED1_PIN 16
//#define PLED2_PIN 18
//#define PLED3_PIN 19
//#define PLED4_PIN 20

// For optional display using the two free pins of the RP2040 Zero
#define HAS_I2C_DISPLAY 1
#define I2C_SDA_PIN 14
#define I2C_SCL_PIN 15
#define I2C_BLOCK i2c1
#define I2C_SPEED 800000
#define DISPLAY_FLIP 0

// Default theme
#define LEDS_BASE_ANIMATION_INDEX 3
#define LEDS_THEME_INDEX 18
#define LEDS_BUTTON_COLOR_INDEX 0

// Set idle timeout
#define IDLE_TIMEOUT_MS 300000

#endif
