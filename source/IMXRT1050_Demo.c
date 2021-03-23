/*
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/**
 * @file    IMXRT1050_Demo.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MIMXRT1052.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */
#include "fsl_gpio.h"
#include "fsl_pit.h"
#include "fsl_lpuart.h"
/* TODO: insert other definitions and declarations here. */
typedef struct gpio_confg_struct
{

	GPIO_Type *m_GPIO;
	uint32_t m_pin;
	gpio_pin_config_t m_gpio_pin_config;
}gpio_confg_struct_t;

static void LED_Config(void)
{
	gpio_confg_struct_t m_gpio_config;

	m_gpio_config.m_GPIO = GPIO1;
	m_gpio_config.m_pin = 9;
	m_gpio_config.m_gpio_pin_config.direction = kGPIO_DigitalOutput;
	m_gpio_config.m_gpio_pin_config.interruptMode = kGPIO_NoIntmode;
	m_gpio_config.m_gpio_pin_config.outputLogic = 0;

	GPIO_PinInit(m_gpio_config.m_GPIO,m_gpio_config.m_pin,&m_gpio_config.m_gpio_pin_config);
}

static int drv_timer_init(void)
{
	pit_config_t pitConfig;

    PIT_GetDefaultConfig(&pitConfig);
    PIT_Init(PIT, &pitConfig);

    NVIC_SetPriority(PIT_IRQn, 2);
    EnableIRQ(PIT_IRQn);

    return 0;
}
static int drv_timer_register(pit_chnl_t id, uint32_t us)
{
    PIT_SetTimerPeriod(PIT, id, USEC_TO_COUNT(us, CLOCK_GetFreq(kCLOCK_PerClk)));
    PIT_EnableInterrupts(PIT, id, kPIT_TimerInterruptEnable);
    PIT_StartTimer(PIT, id);
    return 0;
}
static uint32_t drv_uart_clksrc(void)
{
    uint32_t freq;

    /* To make it simple, we assume default PLL and divider settings, and the only variable
       from application is use PLL3 source or OSC source */
    if (CLOCK_GetMux(kCLOCK_UartMux) == 0) { /* PLL3 div6 80M */
        freq = (CLOCK_GetPllFreq(kCLOCK_PllUsb1) / 6U) / (CLOCK_GetDiv(kCLOCK_UartDiv) + 1U);
    } else {
        freq = CLOCK_GetOscFreq() / (CLOCK_GetDiv(kCLOCK_UartDiv) + 1U);
    }

    return freq;
}
static int drv_Uart_init(void)
{
	lpuart_config_t config;

    LPUART_GetDefaultConfig(&config);
    config.baudRate_Bps = 115200;
    config.enableTx = true;
    config.enableRx = true;

    LPUART_Init(LPUART1, &config, drv_uart_clksrc());
    return 0;
}

/*
 * @brief   Application entry point.
 */
int main(void)
{
	uint16_t i,j;
	char pdata;

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();


    /* TODO: insert other function here. */
    LED_Config();
    drv_timer_init();
    drv_timer_register(kPIT_Chnl_0,1000000);

    drv_Uart_init();
    /* Enter an infinite loop, just incrementing a counter. */
    while(1)
    {
    	pdata = LPUART_ReadByte(LPUART1);
    	if(pdata == 0xaa)
    	{
    		LPUART_WriteByte(LPUART1,0x55);
    	}
    	//GPIO_PortToggle(GPIO1, (1u << 9));
    }
    return 0 ;
}

void PIT_IRQHandler(void)
{
    if (PIT_GetStatusFlags(PIT, kPIT_Chnl_0))
    {
        PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, kPIT_TimerFlag);
        GPIO_PortToggle(GPIO1, (1u << 9));
        //LPUART_WriteByte(LPUART1,0xAA);
    }
    __DSB();
    __ISB();
}
