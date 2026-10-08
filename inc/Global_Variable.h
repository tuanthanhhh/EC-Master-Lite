#ifndef GLOBAL_VARIABLE_H
#define GLOBAL_VARIABLE_H
#include <QString>
#include <vector>
#include <QVector>
#include "soem.h"

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
#endif // GLOBAL_VARIABLE_H
