#include "afx.h"

void vObdTask(void * arg)
{
	char counter = 0;
	for(;;)
	{
		odb_service_notify_testValue(counter++);

		NRF_LOG_INFO("Obd Task\n\r");
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}

ret_code_t xCreateObdTask()
{
	TaskHandle_t xObdTaskHandle;
	if (pdPASS != xTaskCreate(vObdTask, "OBD", 256, NULL, 2, &xObdTaskHandle))
	{
		APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
	}
}
