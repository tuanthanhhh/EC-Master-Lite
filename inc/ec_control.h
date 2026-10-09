#ifndef EC_CONTROL_H
#define EC_CONTROL_H

#include "soem.h"
#include <mutex>

#define OTYPE_VAR           0x0007
#define OTYPE_ARRAY         0x0008
#define OTYPE_RECORD        0x0009

#define ATYPE_Rpre          0x01
#define ATYPE_Rsafe         0x02
#define ATYPE_Rop           0x04
#define ATYPE_Wpre          0x08
#define ATYPE_Wsafe         0x10
#define ATYPE_Wop           0x20


extern bool Is_ECAT_Init;
extern bool Is_stop_thread;
extern std::mutex processData_mutex;
extern char ifbuf[1024];
extern ec_ODlistt ODlist;
extern ec_OElistt OElist;
extern ecx_contextt ctx;

extern uint64_t Tx_Frames;
extern uint64_t Rx_Frames;

extern int WKC;
extern int NAFP_cnt;
extern int Lost_frame_cnt;
extern int polling_rate;

extern uint8_t slave_state_request[EC_MAXSLAVE];
extern bool slave_disconnect[EC_MAXSLAVE];

extern bool Is_testing;
extern bool rescan_slave;
extern bool slave_reconnecting;
typedef struct
{
    uint16_t PDO_count;
    char name[100][64];
    uint16_t index[100];
    uint8_t subindex[100];
    uint8_t bitlen[100];
    uint32_t address[100];
}PDO_mapping_t;

extern PDO_mapping_t PDO_mapping_output[EC_MAXSLAVE];
extern PDO_mapping_t PDO_mapping_input[EC_MAXSLAVE];

int start_ec_master();
void run_ec_master(char *ifname);
int scan_ec_slave(char *ifname);
char* dtype2string(uint16_t dtype, uint16_t bitlen);
char* otype2string(uint16_t otype);
char* access2string(uint16_t access);
char* SDO2string(uint16_t slave, uint16_t index,uint8_t subidx,uint16_t dtype);
int si_PDOassign(uint16_t slave, uint16_t PDOassign, int mapoffset, int bitoffset);
int si_map_sdo(int slave);
int si_siiPDO(uint16_t slave, uint8_t t, int mapoffset, int bitoffset);
int si_map_sii(int slave);
void si_sdo(int cnt);
bool ec_isError();
int StartUp_PS(ecx_contextt* context,uint16_t slave);
#endif // EC_CONTROL_H
