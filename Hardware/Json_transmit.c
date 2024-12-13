#include "cJSON.h"
#include "Json_transmit.h"
#include "Usart_1.h"
#include "Usart_2.h"
#include <stdio.h>
#include <string.h>

#define MAX_JSON_BUFFER_SIZE 128 // 预定义的最大JSON缓冲区大小

// 构建并发送JSON数据
void Sensor_Json_Data(SensorData data)
{
    // 创建JSON对象
    cJSON *json_data = cJSON_CreateObject();
    if (json_data == NULL)
    {
        // JSON对象创建失败，进行错误处理
        DEBUG_Printf("Error: Unable to create JSON object.\n");
        return;
    }

    // 向JSON对象中添加键值对
    cJSON_AddNumberToObject(json_data, "temperature", data.temperature);
    cJSON_AddNumberToObject(json_data, "humidity", data.humidity);
    cJSON_AddNumberToObject(json_data, "MQ2", data.MQ2_ppm);

    // 定义静态缓冲区用于存储JSON字符串
    char json_string[MAX_JSON_BUFFER_SIZE] = {0};

    // 将JSON对象转换为紧凑格式的字符串
    char *json_output = cJSON_PrintUnformatted(json_data);
    if (json_output == NULL)
    {
        // JSON字符串生成失败，进行错误处理
        DEBUG_Printf("Error: Unable to convert JSON object to string.\n");
        cJSON_Delete(json_data); // 释放JSON对象内存
        return;
    }

    // 确保生成的字符串不超过缓冲区大小
    if (strlen(json_output) < MAX_JSON_BUFFER_SIZE)
    {
        strcpy(json_string, json_output); // 拷贝生成的字符串到缓冲区
    }
    else
    {
        // JSON字符串太大，进行错误处理
        DEBUG_Printf("Error: JSON string exceeds buffer size.\n");
    }

    // 打印JSON字符串（调试用）
    // DEBUG_Printf("JSON Data: %s\r\n", json_string);
    // Usart2_printf("JSON Data: %s\r\n", json_string);

    // 释放JSON对象和生成的字符串内存
    cJSON_Delete(json_data);
    free(json_output);

    // 通过TCP发送json_string（假设已经实现了tcp发送函数）
    TCP_send(json_string, strlen(json_string));
}