/*
 * ble.c
 *
 *  Created on: Aug 21, 2017
 *      Author: Kofemolka
 */
#include "afx.h"
#include "char_msg.h"
#include "services.h"
#include "ble_advertising.h"
#include "softdevice_handler.h"

static QueueHandle_t m_inQ;

// Forward declarations
void ble_service_init(void);
ret_code_t obd_service_init(void);
void ble_service_notify(uint16_t charId, uint16_t value);

void vBleTask(void * arg)
{
	ble_service_init();
	APP_ERROR_CHECK(obd_service_init());
	APP_ERROR_CHECK(ble_advertising_start(BLE_ADV_MODE_FAST));

	for(;;)
	{
		char_msg_t charMsg;
		if(xQueueReceive(m_inQ, &charMsg, pdMS_TO_TICKS(50)))
		{
			ble_service_notify(charMsg.charId, charMsg.value);
		}

		intern_softdevice_events_execute();
	}
}

ret_code_t xCreateBleTask(QueueHandle_t inQ)
{
	m_inQ = inQ;

	TaskHandle_t xBleTaskHandle;
	if (pdPASS != xTaskCreate(vBleTask, "BLE", 256, NULL, configMAX_PRIORITIES-1, &xBleTaskHandle))
	{
		APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
	}
}


