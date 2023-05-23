#ifndef MODBUS_H
#define MODBUS_H

struct ModbusRequest
{
    int type;

    int int_num;
    float float_num;
    char *str;
    int str_size;

    int frame_size;

    int device_address;
    int function_code;
    int crc16;
};
typedef struct ModbusRequest ModbusRequest;

struct ModbusResponse
{
    int type;

    int int_num;
    float float_num;
    char *str;
    int str_size;

    int device_address;
    int function_code;
    int crc16;
};
typedef struct ModbusResponse ModbusResponse;

ModbusResponse make_modbus_request(ModbusRequest *req);

#endif