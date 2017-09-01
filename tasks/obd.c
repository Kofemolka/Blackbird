#include "afx.h"
#include "char_msg.h"
#include "services.h"

static QueueHandle_t m_outQ;

bool kline_drv_process_request(uint8_t pid, uint8_t** response, uint8_t* respLength);

typedef uint16_t (*delConvertResult)(uint8_t* resp, uint8_t respLength);

uint16_t convertSpeed(uint8_t* resp, uint8_t respLength)
{
	if(respLength != 1)
		return 0;

	return resp[0] * 10; //example
}

typedef struct {
	uint8_t pid;
	uint8_t charId;
	delConvertResult convert;
} pid_2_char_t;

static pid_2_char_t m_pids[] = {
	{ .pid = 0x05, .charId = ID_CHAR_SPEED, .convert = convertSpeed }
};

void vObdTask(void * arg)
{
	char_msg_t charMsg;
	charMsg.charId = ID_CHAR_SPEED;

	for(;;)
	{
		for(int p=0; p<sizeof(m_pids)/sizeof(pid_2_char_t); p++)
		{
			uint8_t* pResponse;
			uint8_t respLength;
			if(kline_drv_process_request(m_pids[p].pid, &pResponse, &respLength))
			{
				charMsg.charId = m_pids[p].charId;
				charMsg.value = m_pids[p].convert(pResponse, respLength);

				xQueueSendToBack(m_outQ, &charMsg, portMAX_DELAY);
			}
		}

		//vTaskDelay(pdMS_TO_TICKS(1000));
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
