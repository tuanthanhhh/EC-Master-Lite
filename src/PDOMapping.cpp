#include "PDOMapping.h"
#include <vector>
#include "Global_Variable.h"
#include "ec_control.h"

int PDO_mapping(ecx_contextt *context, uint16_t slave)
{
    for(int i = 0; i < Ec_StartUp[slave].cmd.size();i++)
    {
        QString Data_str = Ec_StartUp[slave].cmd[i].Data;
        QStringList DataByte = Data_str.split(" ",Qt::SkipEmptyParts);

        vector<uint16_t> Data;

        for(int j = 0; j + 1 < DataByte.size();j = j + 2)
        {
            bool ok;
            uint8_t dataLow = DataByte[j].toUInt(&ok,16);
            uint8_t dataHigh = DataByte[j + 1].toUInt(&ok,16);
            uint16_t data = (dataHigh << 8) | dataLow;
            Data.push_back(data);
        }
        int wkc = ecx_SDOwrite(context,slave,Ec_StartUp[slave].cmd[i].Index,Ec_StartUp[slave].cmd[i].Sub_Index,Ec_StartUp[slave].cmd[i].CompleteAccess,Data.size()*sizeof(uint16_t),Data.data(),EC_TIMEOUTSAFE);
    }
    return 1;
}

int PDO_mapping( uint16_t slave)
{
    for(int i = 0; i < Ec_StartUp[slave].cmd.size();i++)
    {
        QString Data_str = Ec_StartUp[slave].cmd[i].Data;
        QStringList DataByte = Data_str.split(" ",Qt::SkipEmptyParts);

        vector<uint16_t> Data;

        for(int j = 0; j + 1 < DataByte.size();j = j + 2)
        {
            bool ok;
            uint8_t dataLow = DataByte[j].toUInt(&ok,16);
            uint8_t dataHigh = DataByte[j + 1].toUInt(&ok,16);
            uint16_t data = (dataHigh << 8) | dataLow;
            Data.push_back(data);
        }
        int wkc = ecx_SDOwrite(&ctx,slave,Ec_StartUp[slave].cmd[i].Index,Ec_StartUp[slave].cmd[i].Sub_Index,Ec_StartUp[slave].cmd[i].CompleteAccess,Data.size()*sizeof(uint16_t),Data.data(),EC_TIMEOUTSAFE);
    }
    return 1;
}
