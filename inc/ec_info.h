#ifndef EC_INFO_H
#define EC_INFO_H
#include <stdint.h>
#include "tinyxml2.h"
#include <string>
#include <vector>
#include "soem.h"
#include "Global_Variable.h"

#define ERROR       0
#define WARNING     1
#define MESSAGE     2

#define IDENTITY 0x1018
#define DEVICE_NAME 0x1008
#define HARDWARE_VER 0x1009
#define SOFTWARE_VER 0x100A
#define VENDOR_NAME 0xF9F3
#define REVISION_NUMBER 0x1018

extern bool Is_EC_connected;
extern uint8_t slave_count;
extern uint8_t IOmap[4096];
using namespace std;

void readXMLFile(const std::string& filePath);
void readXMLConfigParam(const std::string& filePath);
std::vector<std::string> scanXMLFiles(const std::string& directoryPath);

typedef struct
{
    char slave_name[32];
    char device_name[32];
    char vendor_id[32];
    char vendor_name[32];
    char product_code[32];
    char serial_number[32];
    char revision_number[32];
    char hardware_ver[32];
    char software_ver[32];
}slave_info_t;


typedef struct
{
    uint16_t count;
    int index_ref[100];
    int subindex[100];
    int bitlen[100];
    char* subIndex_name[100];
    char* datatype[100];
    int address[100];
    int address_offset[100];
    int address_bit[100];
}PDO_subIndex_xml_t;

typedef struct
{
    uint16_t count;
    char* PDO_name_index[100];
    bool Is_Fixed[100];
    bool Is_Mandatory[100];
    int SM[100];
    uint16_t Size[100];
    int Index[100];			//Value of Index
    PDO_subIndex_xml_t PDO_subIndex[100];
}PDO_info_xml_t;

typedef struct
{
    char vendor_id[100];
    char product_code[32];
    char serial_number[32];
    char revision_number[32];
    char slave_name[32];
    uint16_t Physical_Addr;
    uint16_t AutoInc_Addr;
    uint16_t Identification;
}slave_info_xml_t;

typedef struct
{
    uint16_t count;
    int size;
    int PDO_Index[20];
}PDO_mapping_xml_t;

typedef struct
{
    int Mailbox_in;
    int Mailbox_out;
}Mailbox_size_xml_t;

typedef struct
{
    uint16_t Mailbox_InSize;
    uint16_t Mailbox_OutSize;
    uint16_t Mailbox_InAddr;
    uint16_t Mailbox_OutAddr;
    bool DataLinkLayer;
}Mailbox_Info_xml_t;

extern slave_info_xml_t slave_info_xml[EC_MAXSLAVE];
extern PDO_info_xml_t PDO_info_xml_Rx[EC_MAXSLAVE];
extern Mailbox_size_xml_t mailbox_size_xml[EC_MAXSLAVE];
extern Mailbox_Info_xml_t Mailbox_Info_xml[EC_MAXSLAVE];
extern PDO_info_xml_t PDO_info_xml_Tx[EC_MAXSLAVE];

extern PDO_mapping_xml_t PDO_mapping_xml_input[EC_MAXSLAVE];
extern PDO_mapping_xml_t PDO_mapping_xml_output[EC_MAXSLAVE];
extern slave_info_t slave_info[EC_MAXSLAVE];

#endif // EC_INFO_H
