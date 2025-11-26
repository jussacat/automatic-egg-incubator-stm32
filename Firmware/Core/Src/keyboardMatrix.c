/**
 * Copyright (C) Noomane Ridha
 * Free to use for any project
 *
 * keyboardMatrix.c - v1.0 - 25 June 2020
 */

#include "stm32f1xx_hal.h"  // <-- Fix missing HAL definitions
#include "keyboardMatrix.h"
#include "keyboardMatrix_def.h"

typedef enum {
    KEYPAD_LOW = 0,
    KEYPAD_HIGH
} writeReadKeypadState_t;

static uint16_t arrayOfRowPin[4]           = {KEYPAD_ROW_1_PIN, KEYPAD_ROW_2_PIN, KEYPAD_ROW_3_PIN, KEYPAD_ROW_4_PIN};
static uint16_t arrayOfColumnPin[4]        = {KEYPAD_COLUMN_1_PIN, KEYPAD_COLUMN_2_PIN, KEYPAD_COLUMN_3_PIN, KEYPAD_COLUMN_4_PIN};
static GPIO_TypeDef * arrayOfRowPort[4]    = {KEYPAD_ROW_1_PORT, KEYPAD_ROW_2_PORT, KEYPAD_ROW_3_PORT, KEYPAD_ROW_4_PORT};
static GPIO_TypeDef * arrayOfColumnPort[4] = {KEYPAD_COLUMN_1_PORT, KEYPAD_COLUMN_2_PORT, KEYPAD_COLUMN_3_PORT, KEYPAD_COLUMN_4_PORT};

static const char keys[4][4] = {
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
};


static void WriteRowBits(writeReadKeypadState_t state);
static writeReadKeypadState_t ReadColumnsBits(void);



void keyboardInit() {
    GPIO_InitTypeDef GPIO_Column_InitStruct = {0};
    GPIO_InitTypeDef GPIO_Row_InitStruct = {0};

    for (uint8_t i = 0; i < KEYPAD_MAX_COLUMN; i++) {
        __HAL_RCC_GPIOC_CLK_ENABLE(); // Giả sử dùng GPIOA, sửa theo đúng chân bạn đang dùng

        GPIO_Column_InitStruct.Pin = arrayOfColumnPin[i];
        GPIO_Column_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_Column_InitStruct.Pull = GPIO_PULLDOWN;
        HAL_GPIO_Init(arrayOfColumnPort[i], &GPIO_Column_InitStruct);

    }

    for (uint8_t i = 0; i < KEYPAD_MAX_ROW; i++) {
        __HAL_RCC_GPIOA_CLK_ENABLE(); // Giả sử dùng GPIOA, sửa theo đúng chân bạn đang dùng

        GPIO_Row_InitStruct.Pin = arrayOfRowPin[i];
        GPIO_Row_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_Row_InitStruct.Pull = GPIO_NOPULL;
        GPIO_Row_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(arrayOfRowPort[i], &GPIO_Row_InitStruct);
    }
}





char getKeyPressed() {
    uint8_t check = 0;
    char keyPressed = '\0';

    WriteRowBits(KEYPAD_HIGH);
    while (!ReadColumnsBits());
    WriteRowBits(KEYPAD_LOW);

    for (uint8_t i = 0; i < KEYPAD_MAX_ROW; i++) {
        HAL_GPIO_WritePin(arrayOfRowPort[i], arrayOfRowPin[i], GPIO_PIN_SET);
        for (uint8_t j = 0; j < KEYPAD_MAX_COLUMN; j++) {
            check = (uint8_t)HAL_GPIO_ReadPin(arrayOfColumnPort[j], arrayOfColumnPin[j]);
            if (check == 1) {
                keyPressed = keys[i][j];
                return keyPressed;
            }
        }
        HAL_GPIO_WritePin(arrayOfRowPort[i], arrayOfRowPin[i], GPIO_PIN_RESET);
}
    return keyPressed;
}

static void WriteRowBits(writeReadKeypadState_t state) {
    for (uint8_t i = 0; i < KEYPAD_MAX_ROW; i++) {
        HAL_GPIO_WritePin(arrayOfRowPort[i], arrayOfRowPin[i], state == KEYPAD_HIGH ? GPIO_PIN_SET : GPIO_PIN_RESET);
    }
}

static writeReadKeypadState_t ReadColumnsBits(void) {
    for (uint8_t i = 0; i < KEYPAD_MAX_COLUMN; i++) {
        uint8_t check = (uint8_t)HAL_GPIO_ReadPin(arrayOfColumnPort[i], arrayOfColumnPin[i]);
        if (check == 1)
            return KEYPAD_HIGH;
    }
    return KEYPAD_LOW;
}

