#include "ec_info.h"
#include <QString>
#include <QStringList>
#include "Global_Variable.h"

bool Is_EC_connected = false;
uint8_t slave_count = 0;

using namespace tinyxml2;
slave_info_xml_t slave_info_xml[EC_MAXSLAVE];
PDO_info_xml_t PDO_info_xml_Rx[EC_MAXSLAVE];
Mailbox_size_xml_t mailbox_size_xml[EC_MAXSLAVE];
Mailbox_Info_xml_t Mailbox_Info_xml[EC_MAXSLAVE];
PDO_info_xml_t PDO_info_xml_Tx[EC_MAXSLAVE];
Ec_StartUp_t Ec_StartUp[EC_MAXSLAVE];
PDO_mapping_xml_t PDO_mapping_xml_input[EC_MAXSLAVE];
PDO_mapping_xml_t PDO_mapping_xml_output[EC_MAXSLAVE];
slave_info_t slave_info[EC_MAXSLAVE];

void readXMLFile(const std::string& filePath)
{
    XMLDocument doc;
    if (doc.LoadFile(filePath.c_str()) != XML_SUCCESS) {
        return;
    }
    XMLElement* root = doc.RootElement();
    if (!root) {
        return;
    }
    XMLElement* config = root->FirstChildElement("Config");
    XMLElement* slave = root->FirstChildElement("Config")->FirstChildElement("Slave");

    memset(slave_info_xml,0,sizeof(slave_info_xml));
    memset(PDO_info_xml_Tx,0,sizeof(PDO_info_xml_Tx));
    memset(PDO_info_xml_Rx,0,sizeof(PDO_info_xml_Rx));
    memset(PDO_mapping_xml_input,0,sizeof(PDO_mapping_xml_input));
    memset(PDO_mapping_xml_output,0,sizeof(PDO_mapping_xml_output));
    memset(mailbox_size_xml,0,sizeof(mailbox_size_xml));

    slave_count = 0;
    for (XMLElement* slave = config->FirstChildElement("Slave"); slave != NULL; slave = slave->NextSiblingElement("Slave"))
    {
        XMLElement* slaveInfo = slave->FirstChildElement("Info");

        XMLElement* VendorID = slaveInfo->FirstChildElement("VendorId");
        XMLElement* ProductCode = slaveInfo->FirstChildElement("ProductCode");
        XMLElement* RevNumber = slaveInfo->FirstChildElement("RevisionNo");
        XMLElement* SerialNumber = slaveInfo->FirstChildElement("SerialNo");
        XMLElement* SlaveName = slaveInfo->FirstChildElement("Name");

        if (slaveInfo) {
            int Value = 0;
            VendorID->QueryIntText(&Value);


            sprintf((char*)slave_info_xml[slave_count].vendor_id, "0x%02X ", Value);
            ProductCode->QueryIntText(&Value);
            sprintf((char*)slave_info_xml[slave_count].product_code, "0x%02X", Value);
            RevNumber->QueryIntText(&Value);
            sprintf((char*)slave_info_xml[slave_count].revision_number, "0x%02X", Value);
            SerialNumber->QueryIntText(&Value);
            sprintf((char*)slave_info_xml[slave_count].serial_number, "0x%02X", Value);

            memcpy((char*)slave_info_xml[slave_count].slave_name, SlaveName->GetText(), strlen(SlaveName->GetText()));

            QString Data_Str;
            Data_Str = slaveInfo->FirstChildElement("PhysAddr")->GetText();
            slave_info_xml[slave_count].Physical_Addr = Data_Str.toUInt();
            Data_Str = slaveInfo->FirstChildElement("AutoIncAddr")->GetText();
            slave_info_xml[slave_count].AutoInc_Addr = Data_Str.toUInt();
            if(slaveInfo->FirstChildElement("Identification"))
            {
                slave_info_xml[slave_count].Identification = slaveInfo->FirstChildElement("Identification")->FindAttribute("Value")->IntValue();
            }
        }
        slave_count++;

        XMLElement* PDO_info = slave->FirstChildElement("ProcessData");

        int count = 0;
        for (XMLElement* TxPDO = PDO_info->FirstChildElement("TxPdo"); TxPDO != NULL; TxPDO = TxPDO->NextSiblingElement("TxPdo"))
        {
            const char* Fixed_Flag = TxPDO->Attribute("Fixed");
            if (Fixed_Flag == nullptr || strcmp(Fixed_Flag, "true") != 0)
            {
                PDO_info_xml_Tx[slave_count].Is_Fixed[count] = FALSE;
            }
            else
            {
                PDO_info_xml_Tx[slave_count].Is_Fixed[count] = TRUE;
            }

            const char* Mandatory_Flag = TxPDO->Attribute("Mandatory");
            if (Mandatory_Flag == nullptr || strcmp(Mandatory_Flag, "true") != 0)
            {
                PDO_info_xml_Tx[slave_count].Is_Mandatory[count] = FALSE;
            }
            else
            {
                PDO_info_xml_Tx[slave_count].Is_Mandatory[count] = TRUE;
            }

            TxPDO->QueryIntAttribute("Sm", &PDO_info_xml_Tx[slave_count].SM[count]);

            const char* index = TxPDO->FirstChildElement("Index")->GetText();
            if (index != NULL)
            {
                index = index + 2;
                int index_value = strtol(index, NULL, 16);
                PDO_info_xml_Tx[slave_count].Index[count] = index_value;
            }
            XMLElement* PDO_name = TxPDO->FirstChildElement("Name");
            if (PDO_name)
            {
                PDO_info_xml_Tx[slave_count].PDO_name_index[count] = (char*)PDO_name->GetText();
            }
            int count_subIndex = 0;
            PDO_info_xml_Tx[slave_count].Size[count] = 0;
            for (XMLElement* entry = TxPDO->FirstChildElement("Entry");entry != NULL; entry = entry->NextSiblingElement("Entry"))
            {

                XMLElement* Index_ref = entry->FirstChildElement("Index");
                if (Index_ref)
                {
                    const char* index_c = Index_ref->GetText();
                    index_c = index_c + 2;
                    int index_value = strtol(index_c, NULL, 16);
                    PDO_info_xml_Tx[slave_count].PDO_subIndex[count].index_ref[count_subIndex] = index_value;
                }
                XMLElement* subIndex_name = entry->FirstChildElement("Name");
                if (subIndex_name)
                {
                    PDO_info_xml_Tx[slave_count].PDO_subIndex[count].subIndex_name[count_subIndex] = (char*)subIndex_name->GetText();
                }
                else
                {
                    PDO_info_xml_Tx[slave_count].PDO_subIndex[count].subIndex_name[count_subIndex] = NULL;
                }

                XMLElement* subIndex = entry->FirstChildElement("SubIndex");
                if (subIndex)
                {
                    PDO_info_xml_Tx[slave_count].PDO_subIndex[count].subindex[count_subIndex] = atoi(subIndex->GetText());
                }

                XMLElement* bitlen = entry->FirstChildElement("BitLen");
                if(bitlen)
                {
                    PDO_info_xml_Tx[slave_count].PDO_subIndex[count].bitlen[count_subIndex] = atoi(bitlen->GetText());
                    PDO_info_xml_Tx[slave_count].Size[count] += atoi(bitlen->GetText());
                }

                XMLElement* datatype = entry->FirstChildElement("DataType");
                if (datatype)
                {
                    PDO_info_xml_Tx[slave_count].PDO_subIndex[count].datatype[count_subIndex] = (char*)datatype->GetText();
                }

                count_subIndex++;
            }
            PDO_info_xml_Tx[slave_count].PDO_subIndex[count].count = count_subIndex;
            count++;
        }
        PDO_info_xml_Tx[slave_count].count = count;
        count = 0;

        for (XMLElement* RxPDO = PDO_info->FirstChildElement("RxPdo"); RxPDO != NULL; RxPDO = RxPDO->NextSiblingElement("RxPdo"))
        {
            const char* Fixed_Flag = RxPDO->Attribute("Fixed");
            if (Fixed_Flag == nullptr || strcmp(Fixed_Flag, "true")!= 0)
            {
                PDO_info_xml_Rx[slave_count].Is_Fixed[count] = FALSE;
            }
            else
            {
                PDO_info_xml_Rx[slave_count].Is_Fixed[count] = TRUE;
            }


            const char* Mandatory_Flag = RxPDO->Attribute("Mandatory");
            if (Mandatory_Flag == nullptr || strcmp(Mandatory_Flag, "true") != 0)
            {
                PDO_info_xml_Rx[slave_count].Is_Mandatory[count] = FALSE;
            }
            else
            {
                PDO_info_xml_Rx[slave_count].Is_Mandatory[count] = TRUE;
            }

            RxPDO->QueryIntAttribute("Sm", &PDO_info_xml_Rx[slave_count].SM[count]);

            const char* index = RxPDO->FirstChildElement("Index")->GetText();
            if (index != NULL)
            {
                index = index + 2;
                int index_value = strtol(index, NULL, 16);
                PDO_info_xml_Rx[slave_count].Index[count] = index_value;
            }

            XMLElement* PDO_name = RxPDO->FirstChildElement("Name");
            if (PDO_name)
            {
                PDO_info_xml_Rx[slave_count].PDO_name_index[count] = (char*)PDO_name->GetText();
            }
            int count_subIndex = 0;
            PDO_info_xml_Rx[slave_count].Size[count] = 0;
            for (XMLElement* entry = RxPDO->FirstChildElement("Entry");entry != NULL; entry = entry->NextSiblingElement("Entry"))
            {
                XMLElement* Index_ref = entry->FirstChildElement("Index");
                if (Index_ref)
                {
                    const char* index_c = Index_ref->GetText();
                    index_c = index_c + 2;
                    int index_value = strtol(index_c, NULL, 16);
                    PDO_info_xml_Rx[slave_count].PDO_subIndex[count].index_ref[count_subIndex] = index_value;
                }

                XMLElement* subIndex_name = entry->FirstChildElement("Name");
                if (subIndex_name)
                {
                    PDO_info_xml_Rx[slave_count].PDO_subIndex[count].subIndex_name[count_subIndex] = (char*)subIndex_name->GetText();
                }
                else
                {
                    PDO_info_xml_Rx[slave_count].PDO_subIndex[count].subIndex_name[count_subIndex] = NULL;
                }

                XMLElement* subIndex = entry->FirstChildElement("SubIndex");
                if (subIndex)
                {
                    PDO_info_xml_Rx[slave_count].PDO_subIndex[count].subindex[count_subIndex] = atoi(subIndex->GetText());
                }

                XMLElement* bitlen = entry->FirstChildElement("BitLen");
                if (bitlen)
                {
                    PDO_info_xml_Rx[slave_count].PDO_subIndex[count].bitlen[count_subIndex] = atoi(bitlen->GetText());
                    PDO_info_xml_Rx[slave_count].Size[count] += atoi(bitlen->GetText());
                }

                XMLElement* datatype = entry->FirstChildElement("DataType");
                if (datatype)
                {
                    PDO_info_xml_Rx[slave_count].PDO_subIndex[count].datatype[count_subIndex] = (char*)datatype->GetText();
                }
                count_subIndex++;
            }
            PDO_info_xml_Rx[slave_count].PDO_subIndex[count].count = count_subIndex;
            count++;
        }
        PDO_info_xml_Rx[slave_count].count = count;
        count = 0;

        XMLElement* PDO_output = PDO_info->FirstChildElement("Sm2");
        if (PDO_output)
        {
            XMLElement* Size = PDO_output->FirstChildElement("DefaultSize");
            if (Size)
            {
                PDO_mapping_xml_output[slave_count].size = atoi(Size->GetText());
            }
            for (XMLElement* PDO = PDO_output->FirstChildElement("Pdo"); PDO != NULL; PDO = PDO->NextSiblingElement("Pdo"))
            {
                PDO->QueryIntText(&PDO_mapping_xml_output[slave_count].PDO_Index[count]);
                count++;
            }
            PDO_mapping_xml_output[slave_count].count = count;
            count = 0;
        }

        XMLElement* PDO_input = PDO_info->FirstChildElement("Sm3");
        if (PDO_input)
        {
            XMLElement* Size = PDO_input->FirstChildElement("DefaultSize");
            if (Size)
            {
                PDO_mapping_xml_input[slave_count].size = atoi(Size->GetText());
            }
            for (XMLElement* PDO = PDO_input->FirstChildElement("Pdo"); PDO != NULL; PDO = PDO->NextSiblingElement("Pdo"))
            {
                PDO->QueryIntText(&PDO_mapping_xml_input[slave_count].PDO_Index[count]);
                PDO->QueryIntText(&PDO_info_xml_Tx[slave_count].Index[count]);
                count++;
            }
            PDO_mapping_xml_input[slave_count].count = count;
            count = 0;
        }

        XMLElement* Mailbox = slave->FirstChildElement("Mailbox");
        if (Mailbox)
        {
            XMLElement* Mailbox_Send = Mailbox->FirstChildElement("Send");
            if (Mailbox_Send)
            {
                XMLElement* Mailbox_Send_Size = Mailbox_Send->FirstChildElement("Length");
                if (Mailbox_Send_Size)
                {
                    mailbox_size_xml[slave_count].Mailbox_out = atoi(Mailbox_Send_Size->GetText());
                    QString Send_Size_Str = Mailbox_Send_Size->GetText();
                    Mailbox_Info_xml[slave_count].Mailbox_OutSize = Send_Size_Str.toUInt();
                }
                XMLElement* Mailbox_Send_Addr = Mailbox_Send->FirstChildElement("Start");
                if(Mailbox_Send_Addr)
                {
                    QString Send_Addr_Str = Mailbox_Send_Addr->GetText();
                    Mailbox_Info_xml[slave_count].Mailbox_OutAddr = Send_Addr_Str.toUInt();
                }
            }
            XMLElement* Mailbox_Receive = Mailbox->FirstChildElement("Recv");
            if (Mailbox_Receive)
            {
                XMLElement* Mailbox_Receive_Size = Mailbox_Receive->FirstChildElement("Length");
                if (Mailbox_Receive_Size)
                {
                    mailbox_size_xml[slave_count].Mailbox_in = atoi(Mailbox_Receive_Size->GetText());
                    QString Receive_Size_Str = Mailbox_Receive_Size->GetText();
                    Mailbox_Info_xml[slave_count].Mailbox_InSize = Receive_Size_Str.toUInt();
                }

                XMLElement* Mailbox_Receive_Addr = Mailbox_Receive->FirstChildElement("Start");
                if(Mailbox_Receive_Addr)
                {
                    QString Receive_Addr_Str = Mailbox_Receive_Addr->GetText();
                    Mailbox_Info_xml[slave_count].Mailbox_InAddr = Receive_Addr_Str.toUInt();
                }
            }

            XMLElement* CoE = Mailbox->FirstChildElement("CoE");
            XMLElement* InitCmds = CoE->FirstChildElement("InitCmds");
            for(XMLElement* InitCmd = InitCmds->FirstChildElement("InitCmd");InitCmd != NULL;InitCmd = InitCmd->NextSiblingElement("InitCmd"))
            {
                InitCmds_t cmd;
                InitCmd->QueryBoolAttribute("Fixed",&cmd.Fixed);
                InitCmd->QueryBoolAttribute("CompleteAccess",&cmd.CompleteAccess);
                cmd.Transition = InitCmd->FirstChildElement("Transition")->GetText();
                QString Index = QString(InitCmd->FirstChildElement("Index")->GetText());
                QString Sub_Index = QString(InitCmd->FirstChildElement("SubIndex")->GetText());
                cmd.Index = Index.toInt();
                cmd.Sub_Index = Sub_Index.toInt();
                cmd.Comment = InitCmd->FirstChildElement("Comment")->GetText();
                cmd.Data = InitCmd->FirstChildElement("Data")->GetText();

                QStringList Data;
                for(int j = 0; j < cmd.Data.size();j = j + 2)
                {
                    Data << cmd.Data.mid(j,2).toUpper();
                }
                cmd.Data = Data.join(" ");
                cmd.Protocol = "CoE";
                Ec_StartUp[slave_count].cmd.push_back(cmd);
            }
        }
    }

    int address = 0;
    int address_bit = 0;
    for(int i = 1; i<= slave_count;i++)
    {
        for(int j = 0; j < PDO_info_xml_Rx[i].count;j++)
        {
            for(int k = 0; k < PDO_info_xml_Rx[i].PDO_subIndex[j].count;k++)
            {
                if(PDO_info_xml_Rx[i].PDO_subIndex[j].bitlen[k] > 0)
                {
                    PDO_info_xml_Rx[i].PDO_subIndex[j].address[k] = address;
                    PDO_info_xml_Rx[i].PDO_subIndex[j].address_bit[k] = address_bit;
                    if(PDO_info_xml_Rx[i].PDO_subIndex[j].bitlen[k] %8 == 0)
                    {
                        address = address + PDO_info_xml_Rx[i].PDO_subIndex[j].bitlen[k]/8;
                        address_bit = 0;
                    }
                    address_bit = address_bit + PDO_info_xml_Rx[i].PDO_subIndex[j].bitlen[k]%8;
                    if(address_bit >=8)
                    {
                        address_bit = 0;
                        address++;
                    }
                }
            }
        }
    }

    address_bit = 0;
    for(int i = 1; i<= slave_count;i++)
    {
        for(int j = 0; j < PDO_info_xml_Tx[i].count;j++)
        {
            for(int k = 0; k < PDO_info_xml_Tx[i].PDO_subIndex[j].count;k++)
            {
                if(PDO_info_xml_Tx[i].PDO_subIndex[j].bitlen[k] > 0)
                {
                    PDO_info_xml_Tx[i].PDO_subIndex[j].address[k] = address;
                    PDO_info_xml_Tx[i].PDO_subIndex[j].address_bit[k] = address_bit;
                    if(PDO_info_xml_Tx[i].PDO_subIndex[j].bitlen[k] % 8 == 0)
                    {
                        address = address + PDO_info_xml_Tx[i].PDO_subIndex[j].bitlen[k]/8;
                        address_bit = 0;
                    }
                    address_bit = address_bit + PDO_info_xml_Tx[i].PDO_subIndex[j].bitlen[k]%8;
                    if(address_bit >=8)
                    {
                        address_bit = 0;
                        address++;
                    }
                }
            }
        }
    }

}
