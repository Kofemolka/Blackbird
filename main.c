#include "afx.h"

#include "ble_advertising.h"
#include "char_msg.h"
#include "services.h"

void vSDTask(void* arg)
{
	for(;;)
	{
		intern_softdevice_events_execute();
		vTaskDelay(50);
	}
}

void vTestEnvTask(void* arg)
{
	QueueHandle_t outQ = (QueueHandle_t)arg;

	char_msg_t charMsg;
	charMsg.uuid = BLE_UUID_CHAR_TEMP;

	uint8_t var = 0;
	for(;;)
	{
		if(++var > 10) var = 0;
		charMsg.value = 0xF0 + var;
		xQueueSendToBack(outQ, &charMsg, portMAX_DELAY);
		vTaskDelay(pdMS_TO_TICKS(1500));
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

	 TaskHandle_t xSDTaskHandle;
	if (pdPASS != xTaskCreate(vSDTask, "SD", 256, NULL, 0, &xSDTaskHandle))
	{
		APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
	}

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
