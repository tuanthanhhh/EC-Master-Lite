#ifndef GLOBAL_VARIABLE_H
#define GLOBAL_VARIABLE_H
#include <QString>
#include <vector>
#include <QVector>
#include "soem.h"


//DIAGNOSTIC
#define DIAGNOSTIC_HISTORY_INDEX                0x10F3
#define DIAGNOSTIC_MAXIMUM_MESSAGE              0x01
#define DIAGNOSTIC_NEWEST_MESSAGE               0x02
#define DIAGNOSTIC_NEWEST_ACK_MESSAGE           0x03
#define DIAGNOSTIC_NEW_MESSAGE_AVAILBLE         0x04
#define DIAGNOSTIC_FLAGS                        0x05

//ESC_STATUS_INFO
#define OPERATION                               0x00
#define PDI_WATCHDOG                            0x01
#define LINK_DETECTION                          0x02
#define PHYSICAL_LINK_PORTA                     0x04
#define PHYSICAL_LINK_PORTB                     0x05
#define PHYSICAL_LINK_PORTC                     0x06
#define PHYSICAL_LINK_PORTD                     0x07

//MAILBOX
#define MAILBOX_IN                              0x01
#define MAILBOX_OUT                             0x00


typedef struct
{
    QString Protocol;
    QString Transition;
    QString Comment;
    QString Data;
    int Index;
    int Sub_Index;
    bool CompleteAccess;
    bool Fixed;
}InitCmds_t;

typedef struct
{
    int slave;
    std::vector<InitCmds_t> cmd;
}Ec_StartUp_t;

extern Ec_StartUp_t Ec_StartUp[EC_MAXSLAVE];
extern QVector<QString> Adapter_available;

extern int EC_MAILBOX_REQUEST_TIMEOUT[EC_MAXSLAVE];
extern int EC_MAILBOX_RESPONSE_TIMEOUT[EC_MAXSLAVE];
extern int EC_LINKDETECT_PORTB_TIMEOUT[EC_MAXSLAVE];
#endif // GLOBAL_VARIABLE_H
