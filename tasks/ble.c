/*
 * ble.c
 *
 *  Created on: Aug 21, 2017
 *      Author: Kofemolka
 */
#include "afx.h"
#include "char_msg.h"
#include "services.h"

static QueueHandle_t m_inQ;

void vBleTask(void * arg)
{
	for(;;)
	{
		char_msg_t charMsg;
		if(xQueueReceive(m_inQ, &charMsg, portMAX_DELAY))
		{
			NRF_LOG_INFO("vBleTask: OnMsg\n\r");

			switch(charMsg.uuid)
			{
			case BLE_UUID_CHAR_SPEED:
				NRF_LOG_INFO("vBleTask: BLE_UUID_CHAR_SPEED\n\r");
				ble_service_notify(ID_CHAR_SPEED, charMsg.value);
				break;

			case BLE_UUID_CHAR_TEMP:
				NRF_LOG_INFO("vBleTask: BLE_UUID_CHAR_TEMP\n\r");
				ble_service_notify(ID_CHAR_TEMP, charMsg.value);
				break;
			}

		}
	}
}

ret_code_t xCreateBleTask(QueueHandle_t inQ)
{
	m_inQ = inQ;

	TaskHandle_t xBleTaskHandle;
	if (pdPASS != xTaskCreate(vBleTask, "BLE", 256, NULL, 2, &xBleTaskHandle))
	{
		APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
	}
}


