#ifndef DATA_STRUCT_H
#define DATA_STRUCT_H
#include <QString>
#include <QVariantMap>
enum TestList
{
    ECAT_IN_TEST,
    ECAT_OUT_TEST,
    CHECK_ADDRESS_TEST,
    WRITE_SERIAL_NUMBER_TEST,
    ON_OFF_VALVE_TEST,
    NUM_OF_TEST
};

struct FinalTest_Log
{
    QString test_name;
    QString message;

    QVariantMap data;
};

struct Response_Time_IO
{
    uint8_t valve_control_1to8;
    uint8_t valve_control_9to16;
    uint8_t valve_control_17to24;
    uint8_t valve_control_25to32;
    uint8_t valve_control_33to40;
    uint8_t valve_control_41to48;

    uint8_t valve_readback_1to8;
    uint8_t valve_readback_9to16;
    uint8_t valve_readback_17to24;
    uint8_t valve_readback_25to32;
    uint8_t valve_readback_33to40;
    uint8_t valve_readback_41to48;
};

struct Response_Time_AutoTest
{
    int on_Time;
    int off_Time;
    int next_Step_Time;
    int max_Response_Time;
    int verify_Steps;
    int random_Valve;
    QString Valve_Test;
};
#endif // DATA_STRUCT_H
