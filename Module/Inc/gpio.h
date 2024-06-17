/*
 * gpio.h
 *
 *  Created on: 2024. 6. 17.
 *      Author: Dexter
 */

#ifndef MODULE_INC_GPIO_H_
#define MODULE_INC_GPIO_H_


#include "DSP28x_Project.h"
#include "component.h"

#define GPIO_GENERAL_PURPOSE_IO     0x0U
#define CONFIG_GPIO_INPUT           0x0U
#define CONFIG_GPIO_OUTPUT          0x1U

#define GPIO_PIN_0_VAL      0x00000001U
#define GPIO_PIN_1_VAL      0x00000002U
#define GPIO_PIN_2_VAL      0x00000004U
#define GPIO_PIN_3_VAL      0x00000008U
#define GPIO_PIN_4_VAL      0x00000010U
#define GPIO_PIN_5_VAL      0x00000020U
#define GPIO_PIN_6_VAL      0x00000040U
#define GPIO_PIN_7_VAL      0x00000080U
#define GPIO_PIN_8_VAL      0x00000100U
#define GPIO_PIN_9_VAL      0x00000200U
#define GPIO_PIN_10_VAL     0x00000400U
#define GPIO_PIN_11_VAL     0x00000800U
#define GPIO_PIN_12_VAL     0x00001000U
#define GPIO_PIN_13_VAL     0x00002000U
#define GPIO_PIN_14_VAL     0x00004000U
#define GPIO_PIN_15_VAL     0x00008000U
#define GPIO_PIN_16_VAL     0x00010000U
#define GPIO_PIN_17_VAL     0x00020000U
#define GPIO_PIN_18_VAL     0x00040000U
#define GPIO_PIN_19_VAL     0x00080000U
#define GPIO_PIN_20_VAL     0x00100000U
#define GPIO_PIN_21_VAL     0x00200000U
#define GPIO_PIN_22_VAL     0x00400000U
#define GPIO_PIN_23_VAL     0x00800000U
#define GPIO_PIN_24_VAL     0x01000000U
#define GPIO_PIN_25_VAL     0x02000000U
#define GPIO_PIN_26_VAL     0x04000000U
#define GPIO_PIN_27_VAL     0x08000000U
#define GPIO_PIN_28_VAL     0x10000000U
#define GPIO_PIN_29_VAL     0x20000000U
#define GPIO_PIN_30_VAL     0x40000000U
#define GPIO_PIN_31_VAL     0x80000000U
#define GPIO_PIN_32_VAL     0x00000001U
#define GPIO_PIN_33_VAL     0x00000002U
#define GPIO_PIN_34_VAL     0x00000004U
#define GPIO_PIN_35_VAL     0x00000008U
#define GPIO_PIN_36_VAL     0x00000010U
#define GPIO_PIN_37_VAL     0x00000020U
#define GPIO_PIN_38_VAL     0x00000040U
#define GPIO_PIN_39_VAL     0x00000080U
#define GPIO_PIN_40_VAL     0x00000100U
#define GPIO_PIN_41_VAL     0x00000200U
#define GPIO_PIN_42_VAL     0x00000400U
#define GPIO_PIN_43_VAL     0x00000800U
#define GPIO_PIN_44_VAL     0x00001000U
#define GPIO_PIN_45_VAL     0x00002000U
#define GPIO_PIN_46_VAL     0x00004000U
#define GPIO_PIN_47_VAL     0x00008000U
#define GPIO_PIN_48_VAL     0x00010000U
#define GPIO_PIN_49_VAL     0x00020000U
#define GPIO_PIN_50_VAL     0x00040000U
#define GPIO_PIN_51_VAL     0x00080000U
#define GPIO_PIN_52_VAL     0x00100000U
#define GPIO_PIN_53_VAL     0x00200000U
#define GPIO_PIN_54_VAL     0x00400000U
#define GPIO_PIN_55_VAL     0x00800000U
#define GPIO_PIN_56_VAL     0x01000000U
#define GPIO_PIN_57_VAL     0x02000000U
#define GPIO_PIN_58_VAL     0x04000000U
#define GPIO_PIN_59_VAL     0x08000000U
#define GPIO_PIN_60_VAL     0x10000000U
#define GPIO_PIN_61_VAL     0x20000000U
#define GPIO_PIN_62_VAL     0x40000000U
#define GPIO_PIN_63_VAL     0x80000000U

#define MUX_REG_MASK(val, pin)      ((val >> (pin * 2U)) & 0x02U)
#define DIR_REG_MASK(val, pin)      ((val >> pin) & 0x1U)

typedef enum _GPIO_Polarity {
    INT_FOLLING_EDGE = 0U,          /* 하강 엣지에서 인터럽트 발생 */
    INT_RISING_EDGE,                /* 상승 엣지에서 인터럽트 발생 */
    INT_IS_FOLLING_EDGE,            /* 하강 엣지에서 인터럽트 발생*/
    INT_FOLLING_AND_RISING_EDGE,    /* 상승 및 하강 에지에서 인터럽트 발생 */
} GPIO_Polarity;

typedef enum _GPIO_PinState {
    GPIO_PIN_RESET = 0U,
    GPIO_PIN_SET,
    GPIO_ERROR,
} GPIO_PinState;

typedef enum _GPIO_PortNum {
    GPIOA = 0,
    GPIOB,
    GPIOC,
} GPIO_PortNum;

typedef enum _GPIOA_PinNum {
    GPIO_PIN_0 = 0U,
    GPIO_PIN_1,
    GPIO_PIN_2,
    GPIO_PIN_3,
    GPIO_PIN_4,
    GPIO_PIN_5,
    GPIO_PIN_6,
    GPIO_PIN_7,
    GPIO_PIN_8,
    GPIO_PIN_9,
    GPIO_PIN_10,
    GPIO_PIN_11,
    GPIO_PIN_12,
    GPIO_PIN_13,
    GPIO_PIN_14,
    GPIO_PIN_15,
    GPIO_PIN_16,
    GPIO_PIN_17,
    GPIO_PIN_18,
    GPIO_PIN_19,
    GPIO_PIN_20,
    GPIO_PIN_21,
    GPIO_PIN_22,
    GPIO_PIN_23,
    GPIO_PIN_24,
    GPIO_PIN_25,
    GPIO_PIN_26,
    GPIO_PIN_27,
    GPIO_PIN_28,
    GPIO_PIN_29,
    GPIO_PIN_30,
    GPIO_PIN_31,
    GPIOA_PIN_MAX,
} GPIOA_PinNum;

typedef enum _GPIOB_PinNum {
    GPIO_PIN_32 = 0,
    GPIO_PIN_33,
    GPIO_PIN_34,
    GPIO_PIN_35,
    GPIO_PIN_36,
    GPIO_PIN_37,
    GPIO_PIN_38,
    GPIO_PIN_39,
    GPIO_PIN_40,
    GPIO_PIN_41,
    GPIO_PIN_42,
    GPIO_PIN_43,
    GPIO_PIN_44,
    GPIO_PIN_45,
    GPIO_PIN_46,
    GPIO_PIN_47,
    GPIO_PIN_48,
    GPIO_PIN_49,
    GPIO_PIN_50,
    GPIO_PIN_51,
    GPIO_PIN_52,
    GPIO_PIN_53,
    GPIO_PIN_54,
    GPIO_PIN_55,
    GPIO_PIN_56,
    GPIO_PIN_57,
    GPIO_PIN_58,
    GPIO_PIN_59,
    GPIO_PIN_60,
    GPIO_PIN_61,
    GPIO_PIN_62,
    GPIO_PIN_63,
    GPIOB_PIN_MAX,
} GPIOB_PinNum;

typedef enum _GPIOC_PinNum {
    GPIO_PIN_64 = 0U,
    GPIO_PIN_65,
    GPIO_PIN_66,
    GPIO_PIN_67,
    GPIO_PIN_68,
    GPIO_PIN_69,
    GPIO_PIN_70,
    GPIO_PIN_71,
    GPIO_PIN_72,
    GPIO_PIN_73,
    GPIO_PIN_74,
    GPIO_PIN_75,
    GPIO_PIN_76,
    GPIO_PIN_77,
    GPIO_PIN_78,
    GPIO_PIN_79,
    GPIO_PIN_80,
    GPIO_PIN_81,
    GPIO_PIN_82,
    GPIO_PIN_83,
    GPIO_PIN_84,
    GPIO_PIN_85,
    GPIO_PIN_86,
    GPIO_PIN_87,
    GPIOC_PIN_MAX,
} GPIOC_PinNum;

void GPIO_SetConfig(void);
int16 GPIO_WritePin(const GPIO_PortNum port, const Uint16 pin, const Uint32 val, const GPIO_PinState pinState);

#endif /* MODULE_INC_GPIO_H_ */
