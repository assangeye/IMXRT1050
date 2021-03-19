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
/*
 * @brief   Application entry point.
 */
int main(void) {
	uint16_t i,j;
  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif

    PRINTF("Hello World\n");

    /* TODO: insert other function here. */
    LED_Config();
    drv_timer_init();
    drv_timer_register(kPIT_Chnl_0,1000000);
    /* Enter an infinite loop, just incrementing a counter. */
    while(1)
    {
    	for(i=0;i<10;i++)
    	{
    		for(j=0;j<0xffff;j++);
    	}
    	//GPIO_PortToggle(GPIO1, (1u << 9));
    }
    return 0 ;
}

void PIT_IRQHandler(void)
{
    if (PIT_GetStatusFlags(PIT, kPIT_Chnl_0)) {
        PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, kPIT_TimerFlag);
        GPIO_PortToggle(GPIO1, (1u << 9));
    }


    __DSB();
    __ISB();
}
