/*
 * event_handlers.c
 *
 *  Created on: Aug 21, 2017
 *      Author: Kofemolka
 */
#include "afx.h"

void freeRtosAssertHandler()
{
	for(;;);
}

void vApplicationStackOverflowHook( TaskHandle_t xTask,
                                    signed char *pcTaskName )
{
#if NRF_LOG_ENABLED
	NRF_LOG_ERROR("Stack overflow in %s task\r\n", nrf_log_push((char *)pcTaskName));
#endif //NRF_LOG_ENABLED
}

void vApplicationMallocFailedHook()
{
#if NRF_LOG_ENABLED
	NRF_LOG_ERROR("Heap overrun\r\n");
#endif //NRF_LOG_ENABLED
}

void app_error_fault_handler(uint32_t id, uint32_t pc, uint32_t info)
{
	//LEDS_ON(ALL_APP_LED);
	NRF_LOG_ERROR("Fatal\r\n");
	error_info_t* pErrorInfo = (error_info_t*)info;
    if(pErrorInfo != NULL)
    {
    	NRF_LOG_ERROR("Code: 0x%08x File: %s Line: %d\r\n", pErrorInfo->err_code, pErrorInfo->p_file_name, pErrorInfo->line_num);
    }

	//NRF_LOG_FINAL_FLUSH();
    app_error_save_and_stop(id, pc, info);
}
