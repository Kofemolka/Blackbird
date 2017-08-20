#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "nrf_log.h"

#define stats_STACK_SIZE				350									//Stack size of Stats task
#define stats_DELAY						pdMS_TO_TICKS(30*1000)				//Rate at which stats should be updated

static TaskHandle_t 				    mxStatsTaskHandler;       			//Handle of the stats task

char* mapTaskStateStr[] = {
	"Running",
	"Ready",
	"Blocked",
	"Suspended",
	"Deleted"
};																			//Enum to String helper map

static void vRtosTaskStats(void* arg)
{
	TaskStatus_t *pxTaskStatusArray;
	volatile UBaseType_t uxArraySize, x;
	unsigned long ulTotalRunTime,ulStatsAsPercentage;

	for(;;)
	{
		uxArraySize = uxTaskGetNumberOfTasks();

		/* Allocate a TaskStatus_t structure for each task.  An array could be
		   allocated statically at compile time. */
	   pxTaskStatusArray = pvPortMalloc( uxArraySize * sizeof( TaskStatus_t ) );

	   if( pxTaskStatusArray != NULL )
	   {
		  /* Generate raw status information about each task. */
		  uxArraySize = uxTaskGetSystemState( pxTaskStatusArray,
									 uxArraySize,
									 &ulTotalRunTime );

		  /* For percentage calculations. */
		  ulTotalRunTime /= 100UL;

		  /* Avoid divide by zero errors. */
		  if( ulTotalRunTime > 0 )
		  {
			  NRF_LOG_INFO("Task stats:\n\r");
			  NRF_LOG_INFO(" [Name]   [State] [Priority] [Stack] [Ticks]  [%]\n\r");

			 /* For each populated position in the pxTaskStatusArray array,
			 format the raw data as human readable ASCII data. */
			 for( x = 0; x < uxArraySize; x++ )
			 {
				ulStatsAsPercentage = pxTaskStatusArray[x].ulRunTimeCounter / ulTotalRunTime;
				if(ulStatsAsPercentage>100) ulStatsAsPercentage = 100;

				NRF_LOG_INFO("%7s %9s %10d %7d %7d %3d%%\n\r",
						pxTaskStatusArray[x].pcTaskName,
						mapTaskStateStr[pxTaskStatusArray[x].eCurrentState],
						pxTaskStatusArray[x].uxCurrentPriority,
						pxTaskStatusArray[x].usStackHighWaterMark,
						pxTaskStatusArray[x].ulRunTimeCounter,
						ulStatsAsPercentage);
			 }
		  }

		  /* The array is no longer needed, free the memory it consumes. */
		  vPortFree( pxTaskStatusArray );
	   }

	   vTaskDelay(stats_DELAY);
	}
}

TaskHandle_t xCreateStatsTask(unsigned portBASE_TYPE uxPriority)
{
	if(NULL == mxStatsTaskHandler)
	{
		if (pdPASS != xTaskCreate(vRtosTaskStats, "STATS", stats_STACK_SIZE, NULL, uxPriority, &mxStatsTaskHandler))
		{
			APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
		}
	}

	return mxStatsTaskHandler;
}

