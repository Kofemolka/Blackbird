#include "afx.h"
#include "char_msg.h"
#include "services.h"

static QueueHandle_t m_outQ;

void vObdTask(void * arg)
{
	char_msg_t charMsg;
	charMsg.charId = ID_CHAR_SPEED;

	for(;;)
	{
		charMsg.value += 10;

		xQueueSendToBack(m_outQ, &charMsg, portMAX_DELAY);

		NRF_LOG_INFO("Obd Task\n\r");
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}

ret_code_t xCreateObdTask(QueueHandle_t outQ)
{
	m_outQ = outQ;

	TaskHandle_t xObdTaskHandle;
	if (pdPASS != xTaskCreate(vObdTask, "OBD", 256, NULL, 2, &xObdTaskHandle))
	{
		APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
	}
}
