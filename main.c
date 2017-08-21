#include "afx.h"

#include "char_msg.h"
#include "services.h"

void vTestEnvTask(void* arg)
{
	QueueHandle_t outQ = (QueueHandle_t)arg;

	char_msg_t charMsg;
	charMsg.charId = ID_CHAR_TEMP;

	uint8_t var = 0;
	for(;;)
	{
		if(++var > 10) var = 0;
		charMsg.value = 0xF0 + var;
		xQueueSendToBack(outQ, &charMsg, portMAX_DELAY);
		vTaskDelay(pdMS_TO_TICKS(1500));
	}
}

//External functions declaration
ret_code_t xCreateObdTask(QueueHandle_t outQ);
ret_code_t xCreateBleTask(QueueHandle_t inQ);
TaskHandle_t xCreateStatsTask(unsigned portBASE_TYPE uxPriority);

int main()
{
	ret_code_t err_code = NRF_LOG_INIT(NULL);
	APP_ERROR_CHECK(err_code);

	QueueHandle_t bleInQ = xQueueCreate(5, sizeof(char_msg_t));
	if (NULL == bleInQ)
	{
		APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
	}

	if (pdPASS != xCreateObdTask(bleInQ))
	{
		APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
	}

	if (pdPASS != xCreateBleTask(bleInQ))
	{
		APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
	}

	TaskHandle_t xTestEnvTaskHandle;
	if (pdPASS != xTaskCreate(vTestEnvTask, "Test", 100, bleInQ, 2, &xTestEnvTaskHandle))
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
