#include <SoftwareSerial.h>

#define RX 8
#define TX 9

#define ECU_ID 0x11
#define FORMAT_SINGLE 0x81
#define FORMAT_DATA	  0x80
#define MAX_PACKET_SIZE 10

SoftwareSerial ss(RX, TX);

char logBuf[100];

struct ecu_packet_t {
	uint8_t format;
	uint8_t sender;
	uint8_t payload[MAX_PACKET_SIZE-4];
	uint8_t length;
};

static uint8_t calcChecksum(uint8_t *data, uint8_t len)
{
	uint8_t crc = 0;

	for (uint8_t i = 0; i < len; i++)
	{
		crc = crc + data[i];
	}
	return crc;
}

bool receivePacket(uint8_t format, struct ecu_packet_t* pRequest)
{
	if (pRequest == NULL)
		return false;
	
	const uint16_t MAXSENDTIME = 2000;	

	uint8_t input[MAX_PACKET_SIZE];

	pRequest->format = format;
	input[0] = format;
	
	uint16_t startTime = millis();
	uint8_t byteCounter = 0;
	uint8_t payloadCounter = 0;

	while ((byteCounter <= 10) && ((millis() - startTime) < MAXSENDTIME))
	{
		if (ss.available())
		{
			uint8_t c = ss.read();			
			sprintf(logBuf, "c=0x%X", c);
			//Serial.println(logBuf);
			startTime = millis();

			input[byteCounter + 1] = c;

			switch (byteCounter)
			{
				case 0: //ECU
					if (c != ECU_ID)
					{
						return false;
					}
					break;

				case 1: //Sender				
					pRequest->sender = c;
					break;
				
				case 2: //Payload length or single byte payload
					if (format == FORMAT_SINGLE)
					{
						pRequest->length = 1;
						pRequest->payload[0] = c;
						payloadCounter = 1;
					}
					else
					{
						pRequest->length = c;
					}
					break;

				default: //Payload or checksum
					if (payloadCounter == pRequest->length)
					{
						uint8_t cnc = calcChecksum(input, byteCounter+1);
						for (int i = 0; i < byteCounter; i++)
						{
							sprintf(logBuf, "Input=0x%X", input[i]); //Serial.println(logBuf);							
						}

						//Serial.print("CNC="); Serial.println(cnc);
						return c == cnc;
					}
					else
					{
						pRequest->payload[payloadCounter++] = c;
					}
					break;
			}

			byteCounter++;
		}
	}

	return false;
}

void reply(ecu_packet_t* pPacket)
{
	if (pPacket == NULL)
		return;

	uint8_t output[MAX_PACKET_SIZE];
	uint8_t counter = 0;
	//Response:
	//format
	//client
	//ECU
	//Resp
	//CNC
	output[counter++] = pPacket->format;
	output[counter++] = pPacket->sender;
	output[counter++] = ECU_ID;
	if (pPacket->format == FORMAT_SINGLE)
	{
		output[counter++] = FORMAT_SINGLE;
	}
	else
	{
		output[counter++] = pPacket->length;
		for (int i = 0; i < pPacket->length; i++)
		{
			output[counter++] = pPacket->payload[i];
		}
	}

	Serial.println("Reply:");

	for (int i = 0; i < counter; i++)
	{
		sprintf(logBuf, ">: 0x%X", output[i]);	Serial.println(logBuf);
		ss.write(output[i]);
	}

	uint8_t cnc = calcChecksum(output, counter);
	sprintf(logBuf, ">: 0x%X", cnc);	Serial.println(logBuf);

	ss.write(cnc);	
}

void setup()
{
	Serial.begin(9600);
	ss.begin(9600);

  	Serial.println("Ready");
}

void loop()
{
	if (Serial.available())
	{
		uint8_t c = Serial.read();
		ss.write(c);
	}

	if (ss.available())
	{
		uint8_t c = ss.read();

		sprintf(logBuf, "0x%X", c);
		//Serial.println(logBuf);

		if (c == FORMAT_DATA || c == FORMAT_SINGLE)
		{
			ecu_packet_t request;
			
			if (receivePacket(c, &request))
			{
				Serial.println("Packet:");
				sprintf(logBuf, "Format: 0x%X", request.format);	Serial.println(logBuf);
				sprintf(logBuf, "Sender: 0x%X", request.sender);	Serial.println(logBuf);
				for (int i = 0; i < request.length; i++)
				{
					sprintf(logBuf, "<: 0x%X", request.payload[i]);	Serial.println(logBuf);
				}

				if (request.length == 1 && request.payload[0] == FORMAT_SINGLE) //Hello?
				{
					ecu_packet_t resp;
					resp.format = FORMAT_DATA;
					resp.sender = request.sender;
					resp.length = 3;
					resp.payload[0] = 0xC1;
					resp.payload[1] = 0xEA;
					resp.payload[2] = 0x8F;

					reply(&resp);
				} 
				else if (request.length == 2 && request.payload[0] == 0x10 && request.payload[1] == 0x80)
				{
					ecu_packet_t resp;
					resp.format = FORMAT_DATA;
					resp.sender = request.sender;
					resp.length = 2;
					resp.payload[0] = 0x50;
					resp.payload[1] = 0x80;					

					reply(&resp);
				}
				else if (request.length == 2 && request.payload[0] == 0x21) //Current data
				{
					ecu_packet_t resp;
					resp.format = FORMAT_DATA;
					resp.sender = request.sender;
					resp.length = 1;	
					resp.payload[0] = 0;					

					switch (request.payload[1]) //PID
					{
					case 0x05: //Engine Coolant Temperature 1 Byte - OK
						resp.payload[0] = millis() % 100;
						break;
					case 0x0C: //Engine RPM 2Byte - OK
						resp.payload[0] = millis() % 250;
						break;
					case 0x0D: //Speed 1 Byte - OK
						resp.payload[0] = millis() % 150;
						break;
					}		

					reply(&resp);
				}

			}
		}		
	}  

}

//Request:
//format
//ECU
//Client
//Request
//R+
//CNC

//Response:
//format
//client
//ECU
//Resp
//CNC
