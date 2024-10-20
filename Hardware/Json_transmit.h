#ifndef __JSON_TRANSMIT_H
#define __JSON_TRANSMIT_H

#define DEBUG_Printf	    Usart1_printf					//串口1作为调试串口输出

typedef struct {
    float temperature;
    float humidity;
    float MQ2_ppm;
} SensorData;

void Sensor_Json_Data(SensorData data);

#endif