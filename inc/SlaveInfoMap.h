#ifndef SLAVEINFOMAP_H
#define SLAVEINFOMAP_H

#include "ec_info.h"
typedef struct
{
    uintptr_t address;
    uint16_t index;
    uint8_t subindex;
}info_slave_mapped_t;

const info_slave_mapped_t info_slave_mapped[] =
    {
        { offsetof(slave_info_t, device_name),				DEVICE_NAME,            0x00},
        { offsetof(slave_info_t,hardware_ver),				HARDWARE_VER,           0x00},
        { offsetof(slave_info_t, software_ver),             SOFTWARE_VER,           0x00},
        { offsetof(slave_info_t, vendor_name),				VENDOR_NAME,            0x00},
        { offsetof(slave_info_t,revision_number),           REVISION_NUMBER,        0x03},
        { NULL,												0x00,                   0x00}
};

void* sendPoint_info(uint16_t slave, uint16_t index,uint16_t subindex)
{
    int i = 0;
    while (info_slave_mapped[i].address != NULL)
    {
        if (info_slave_mapped[i].index == index && info_slave_mapped[i].subindex == subindex)
        {
            return (void*)((uint8_t*)&slave_info[slave] + info_slave_mapped[i].address);

        }
        i++;
    }
    return NULL;
}
#endif // SLAVEINFOMAP_H
