#ifndef PDOMAPPING_H
#define PDOMAPPING_H

#include <stdio.h>
#include <stdint.h>
#include "ec_info.h"
#include "ec_info.h"
#include "soem.h"

int PDO_mapping(ecx_contextt *context,uint16_t slave);
int PDO_mapping( uint16_t slave);

extern PDO_mapping_xml_t PDO_mapping_xml_input[EC_MAXSLAVE];
extern PDO_mapping_xml_t PDO_mapping_xml_output[EC_MAXSLAVE];
#endif // PDOMAPPING_H
