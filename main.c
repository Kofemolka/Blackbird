#include "afx.h"

#include "ble_advertising.h"

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

void odb_service_notify_testValue(char);

void vTestTask(void * arg)
{
	char counter = 0;
	for(;;)
	{
		odb_service_notify_testValue(counter++);

		NRF_LOG_INFO("Test Task: %03d\n\r", counter);
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}

void vSDTask(void* arg)
{
	for(;;)
	{
		intern_softdevice_events_execute();
		vTaskDelay(50);
	}
}

void ble_stack_init();
ret_code_t obd_service_init();

int main()
{
	ret_code_t err_code = NRF_LOG_INIT(NULL);
	APP_ERROR_CHECK(err_code);

	ble_stack_init();
	err_code = obd_service_init();
	APP_ERROR_CHECK(err_code);
	err_code = ble_advertising_start(BLE_ADV_MODE_FAST);
	APP_ERROR_CHECK(err_code);


	 TaskHandle_t xTestTaskHandle;
	if (pdPASS != xTaskCreate(vTestTask, "Test", 256, NULL, 2, &xTestTaskHandle))
	{
		APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
	}

	 TaskHandle_t xSDTaskHandle;
	if (pdPASS != xTaskCreate(vSDTask, "SD", 256, NULL, 0, &xSDTaskHandle))
	{
		APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
	}

	if (pdPASS != xCreateObdTask())
	{
		APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
	}

	xCreateStatsTask(0);


	NRF_LOG_INFO("Ready\n\r");
	vTaskStartScheduler();

	for(;;)
	{
		APP_ERROR_HANDLER(NRF_ERROR_FORBIDDEN);
	}
}
