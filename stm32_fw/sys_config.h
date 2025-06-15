#ifndef SYS_CONFIG_H
#define SYS_CONFIG_H

#include "Arduino.h"

#define TASK_INTERVALS  (50)

#define LCD_ADDR        (0x27)
#define LCD_COLS        (16)
#define LCD_ROWS        (2)

#define BUTTON_PRESSED  (HIGH)

#define BUTTON_SET_PIN  (PC10)
#define BUTTON_MODE_PIN (PC11)
#define BUTTON_UP_PIN   (PC12)
#define BUTTON_DOWN_PIN (PB7)

#define LED_ON          (LOW)
#define LED_OFF         (!LED_ON)

#define LED_1_PIN       (PC0)
#define LED_2_PIN       (PC1)
#define LED_3_PIN       (PC2)
#define LED_4_PIN       (PC3)

typedef enum {
  BUTTON_SET = 0,
  BUTTON_MODE,
  BUTTON_UP,
  BUTTON_DOWN,
  BUTTON_NUM,
} BtnFunc_t;

typedef enum {
  LED_1 = 0,
  LED_2,
  LED_3,
  LED_4,
  LED_NUM,
} LedFunc_t;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t  s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

#endif
