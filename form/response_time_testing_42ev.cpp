#include "response_time_testing_42ev.h"
#include "ui_response_time_testing_42ev.h"
#include "ec_control.h"
#include "ec_info.h"
#include <QMessageBox>
#include <QRandomGenerator>
#include <QThread>
#include <QDateTime>
uint64_t temp_42EV = 0;
response_time_testing_42ev::response_time_testing_42ev(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::response_time_testing_42ev)
{
    ui->setupUi(this);
    Timer_Test = new QTimer(this);
    connect(Timer_Test,SIGNAL(timeout()),this,SLOT(RunIOTest()));
    Timer_RandomTest = new QTimer(this);
    connect(Timer_RandomTest,SIGNAL(timeout()),this,SLOT(RunRandomTest()));
    Timer_ManualTest = new QTimer(this);
    connect(Timer_ManualTest,SIGNAL(timeout()),this,SLOT(RunManualTest()));
    response_time_autoTest->Valve_Test = "";
}

response_time_testing_42ev::~response_time_testing_42ev()
{
    delete ui;
}

void response_time_testing_42ev::on_slave_comboBox_1_currentIndexChanged(int index)
{
    slave1_test = ui->slave_comboBox_1->currentIndex() + 1;
    for (int row = 0; row < ui->Valve_Control_TableWidget_1->rowCount(); row++)
    {
        for (int col = 0; col < ui->Valve_Control_TableWidget_1->columnCount(); col++)
        {
            ui->Valve_Control_TableWidget_1->setItem(row, col,new QTableWidgetItem(""));
            ui->Valve_Readback_TableWidget_1->setItem(row, col,new QTableWidgetItem(""));
            ui->Response_On_Time_TableWidget_1->setItem(row, col,new QTableWidgetItem(""));
            ui->Response_Off_Time_TableWidget_1->setItem(row, col,new QTableWidgetItem(""));
        }
    }

    for (int i = 0; i < PDO_mapping_output[slave1_test - 1].PDO_count;i++)
    {
        if(strcmp(PDO_mapping_output[slave1_test -1].name[i],"Valves 1 to 8") == 0 || strcmp( PDO_mapping_output[slave1_test -1].name[i],"Out 1 to 8") == 0 )
        {
            PDO_ValveControl_1 = i;
        }
    }

    for(int i = 0; i < PDO_info_xml_Rx[slave1_test].count;i++)
    {
        for(int j = 0; j < PDO_info_xml_Rx[slave1_test].PDO_subIndex[i].count; j++)
        {
            if(strcmp(PDO_info_xml_Rx[slave1_test].PDO_subIndex[i].subIndex_name[j],"Valves 1 to 8") == 0 || strcmp(PDO_info_xml_Rx[slave1_test].PDO_subIndex[i].subIndex_name[j],"Out 1 to 8") == 0)
            {
                PDO_ValveControl_Address_1 = PDO_info_xml_Rx[slave1_test].PDO_subIndex[i].address[j];
            }
        }
    }

    for(int i = 0; i < PDO_info_xml_Tx[slave1_test].count;i++)
    {
        for(int j = 0; j < PDO_info_xml_Tx[slave1_test].PDO_subIndex[i].count; j++)
        {
            if(strcmp(PDO_info_xml_Tx[slave1_test].PDO_subIndex[i].subIndex_name[j],"Valves 1 to 8") == 0 || strcmp(PDO_info_xml_Tx[slave1_test].PDO_subIndex[i].subIndex_name[j],"In 1 to 8") == 0)
            {
                PDO_ValveReadback_Address_1 = PDO_info_xml_Tx[slave1_test].PDO_subIndex[i].address[j];
            }
        }
    }

    for (int i = 0; i < PDO_mapping_input[slave1_test - 1].PDO_count;i++)
    {
        if(strcmp(PDO_mapping_input[slave1_test -1].name[i],"Valves 1 to 8") == 0 || strcmp( PDO_mapping_input[slave1_test -1].name[i],"In 1 to 8") == 0 )
        {
            PDO_ValveReadback_1 = i;
        }
    }
}


void response_time_testing_42ev::on_slave_comboBox_2_currentIndexChanged(int index)
{
    slave2_test = ui->slave_comboBox_2->currentIndex() + 1;
    for (int row = 0; row < ui->Valve_Control_TableWidget_2->rowCount(); row++)
    {
        for (int col = 0; col < ui->Valve_Control_TableWidget_2->columnCount(); col++)
        {
            ui->Valve_Control_TableWidget_2->setItem(row, col,new QTableWidgetItem(""));
            ui->Valve_Readback_TableWidget_2->setItem(row, col,new QTableWidgetItem(""));
            ui->Response_On_Time_TableWidget_2->setItem(row, col,new QTableWidgetItem(""));
            ui->Response_Off_Time_TableWidget_2->setItem(row, col,new QTableWidgetItem(""));
        }
    }

    for (int i = 0; i < PDO_mapping_output[slave2_test - 1].PDO_count;i++)
    {
        if(strcmp(PDO_mapping_output[slave2_test -1].name[i],"Valves 1 to 8") == 0 || strcmp( PDO_mapping_output[slave2_test -1].name[i],"Out 1 to 8") == 0 )
        {
            PDO_ValveControl_2 = i;
        }
    }
    for (int i = 0; i < PDO_mapping_input[slave2_test - 1].PDO_count;i++)
    {
        if(strcmp(PDO_mapping_input[slave2_test -1].name[i],"Valves 1 to 8") == 0 || strcmp( PDO_mapping_input[slave2_test -1].name[i],"In 1 to 8") == 0 )
        {
            PDO_ValveReadback_2 = i;
        }
    }

    for(int i = 0; i < PDO_info_xml_Rx[slave2_test].count;i++)
    {
        for(int j = 0; j < PDO_info_xml_Rx[slave2_test].PDO_subIndex[i].count; j++)
        {
            if(strcmp(PDO_info_xml_Rx[slave2_test].PDO_subIndex[i].subIndex_name[j],"Valves 1 to 8") == 0 || strcmp(PDO_info_xml_Rx[slave2_test].PDO_subIndex[i].subIndex_name[j],"Out 1 to 8") == 0)
            {
                PDO_ValveControl_Address_2 = PDO_info_xml_Rx[slave2_test].PDO_subIndex[i].address[j];
            }
        }
    }

    for(int i = 0; i < PDO_info_xml_Tx[slave2_test].count;i++)
    {
        for(int j = 0; j < PDO_info_xml_Tx[slave2_test].PDO_subIndex[i].count; j++)
        {
            if(strcmp(PDO_info_xml_Tx[slave2_test].PDO_subIndex[i].subIndex_name[j],"Valves 1 to 8") == 0 || strcmp(PDO_info_xml_Tx[slave2_test].PDO_subIndex[i].subIndex_name[j],"In 1 to 8") == 0)
            {
                PDO_ValveReadback_Address_2 = PDO_info_xml_Tx[slave2_test].PDO_subIndex[i].address[j];
            }
        }
    }
}


void response_time_testing_42ev::on_slave_comboBox_3_currentIndexChanged(int index)
{
    slave3_test = ui->slave_comboBox_3->currentIndex() + 1;
    for (int row = 0; row < ui->Valve_Control_TableWidget_3->rowCount(); row++)
    {
        for (int col = 0; col < ui->Valve_Control_TableWidget_3->columnCount(); col++)
        {
            ui->Valve_Control_TableWidget_3->setItem(row, col,new QTableWidgetItem(""));
            ui->Valve_Readback_TableWidget_3->setItem(row, col,new QTableWidgetItem(""));
            ui->Response_On_Time_TableWidget_3->setItem(row, col,new QTableWidgetItem(""));
            ui->Response_Off_Time_TableWidget_3->setItem(row, col,new QTableWidgetItem(""));
        }
    }

    for (int i = 0; i < PDO_mapping_output[slave3_test - 1].PDO_count;i++)
    {
        if(strcmp(PDO_mapping_output[slave3_test -1].name[i],"Valves 1 to 8") == 0 || strcmp( PDO_mapping_output[slave3_test -1].name[i],"Out 1 to 8") == 0 )
        {
            PDO_ValveControl_3 = i;
        }
    }
    for (int i = 0; i < PDO_mapping_input[slave3_test - 1].PDO_count;i++)
    {
        if(strcmp(PDO_mapping_input[slave3_test -1].name[i],"Valves 1 to 8") == 0 || strcmp( PDO_mapping_input[slave3_test -1].name[i],"In 1 to 8") == 0 )
        {
            PDO_ValveReadback_3 = i;
        }
    }

    for(int i = 0; i < PDO_info_xml_Rx[slave3_test].count;i++)
    {
        for(int j = 0; j < PDO_info_xml_Rx[slave3_test].PDO_subIndex[i].count; j++)
        {
            if(strcmp(PDO_info_xml_Rx[slave3_test].PDO_subIndex[i].subIndex_name[j],"Valves 1 to 8") == 0 || strcmp(PDO_info_xml_Rx[slave3_test].PDO_subIndex[i].subIndex_name[j],"Out 1 to 8") == 0)
            {
                PDO_ValveControl_Address_3 = PDO_info_xml_Rx[slave3_test].PDO_subIndex[i].address[j];
            }
        }
    }

    for(int i = 0; i < PDO_info_xml_Tx[slave3_test].count;i++)
    {
        for(int j = 0; j < PDO_info_xml_Tx[slave3_test].PDO_subIndex[i].count; j++)
        {
            if(strcmp(PDO_info_xml_Tx[slave3_test].PDO_subIndex[i].subIndex_name[j],"Valves 1 to 8") == 0 || strcmp(PDO_info_xml_Tx[slave3_test].PDO_subIndex[i].subIndex_name[j],"In 1 to 8") == 0)
            {
                PDO_ValveReadback_Address_3 = PDO_info_xml_Tx[slave3_test].PDO_subIndex[i].address[j];
            }
        }
    }
}


void response_time_testing_42ev::on_Start_Test_button_clicked()
{
    if (Is_Start_Test == true)
    {
        Is_Start_Test = false;
        ui->Start_Test_button->setText("Start Test");
        ui->Next_Step_textBox->setEnabled(true);
        ui->Max_Response_textBox->setEnabled(true);
        ui->Verify_Step_textBox->setEnabled(true);
        Timer_Test->stop();
        Timer_ManualTest->stop();
        Timer_RandomTest->stop();
        if(ui->Sequential_radioButton->isChecked() == true)
        {
            if(slave1_test <= slave_count)
            {
                logStatus(QString("Slave %1 - Max Response ON Time: %2ms, Min Response ON Time: %3ms").arg(slave1_test).arg(Max_Response_OnTime_1,0,'f',2).arg(Min_Response_OnTime_1,0,'f',2));
                logStatus(QString("Slave %1 - Max Response OFF Time: %2ms, Min Response OFF Time: %3ms").arg(slave1_test).arg(Max_Response_OffTime_1,0,'f',2).arg(Min_Response_OffTime_1,0,'f',2));
            }
            if(slave2_test <= slave_count)
            {
                logStatus(QString("Slave %1 - Max Response ON Time: %2ms, Min Response ON Time: %3ms").arg(slave2_test).arg(Max_Response_OnTime_2,0,'f',2).arg(Min_Response_OnTime_2,0,'f',2));
                logStatus(QString("Slave %1 - Max Response OFF Time: %2ms, Min Response OFF Time: %3ms").arg(slave2_test).arg(Max_Response_OffTime_2,0,'f',2).arg(Min_Response_OffTime_2,0,'f',2));
            }
            if(slave3_test <= slave_count)
            {
                logStatus(QString("Slave %1 - Max Response ON Time: %2ms, Min Response ON Time: %3ms").arg(slave3_test).arg(Max_Response_OnTime_3,0,'f',2).arg(Min_Response_OnTime_3,0,'f',2));
                logStatus(QString("Slave %1 - Max Response OFF Time: %2ms, Min Response OFF Time: %3ms").arg(slave3_test).arg(Max_Response_OffTime_3,0,'f',2).arg(Min_Response_OffTime_3,0,'f',2));
            }
        }
        logStatus("Stop Test");
    }
    else
    {
        logStatus("Start Test");
        Is_Start_Test = true;
        Is_Test_Fail = false;
        Valve_State = VALVE_ON;
        step = ui->Verify_Step_textBox->text().toInt();
        temp_42EV = 0;
        IOmap[PDO_mapping_output[slave1_test - 1].address[PDO_ValveControl_1]] = 0;
        IOmap[PDO_mapping_output[slave1_test - 1].address[PDO_ValveControl_1] + 1] = 0;
        IOmap[PDO_mapping_output[slave1_test - 1].address[PDO_ValveControl_1] + 2] = 0;
        IOmap[PDO_mapping_output[slave1_test - 1].address[PDO_ValveControl_1] + 3] = 0;
        IOmap[PDO_mapping_output[slave1_test - 1].address[PDO_ValveControl_1] + 4] = 0;
        IOmap[PDO_mapping_output[slave1_test - 1].address[PDO_ValveControl_1] + 5] = 0;

        IOmap[PDO_mapping_output[slave2_test - 1].address[PDO_ValveControl_2]] = 0;
        IOmap[PDO_mapping_output[slave2_test - 1].address[PDO_ValveControl_2] + 1] = 0;
        IOmap[PDO_mapping_output[slave2_test - 1].address[PDO_ValveControl_2] + 2] = 0;
        IOmap[PDO_mapping_output[slave2_test - 1].address[PDO_ValveControl_2] + 3] = 0;
        IOmap[PDO_mapping_output[slave2_test - 1].address[PDO_ValveControl_2] + 4] = 0;
        IOmap[PDO_mapping_output[slave2_test - 1].address[PDO_ValveControl_2] + 5] = 0;

        IOmap[PDO_mapping_output[slave3_test - 1].address[PDO_ValveControl_3]] = 0;
        IOmap[PDO_mapping_output[slave3_test - 1].address[PDO_ValveControl_3] + 1] = 0;
        IOmap[PDO_mapping_output[slave3_test - 1].address[PDO_ValveControl_3] + 2] = 0;
        IOmap[PDO_mapping_output[slave3_test - 1].address[PDO_ValveControl_3] + 3] = 0;
        IOmap[PDO_mapping_output[slave3_test - 1].address[PDO_ValveControl_3] + 4] = 0;
        IOmap[PDO_mapping_output[slave3_test - 1].address[PDO_ValveControl_3] + 5] = 0;
        Read_Valve();
        display_ValveControl();
        display_ValveReadback();
        Is_Start_Test = true;
        ui->Start_Test_button->setText("Stop Test");
        ui->Next_Step_textBox->setEnabled(false);
        ui->Max_Response_textBox->setEnabled(false);
        ui->Verify_Step_textBox->setEnabled(false);

        resetTableWidget();
        int next_step_time = ui->Next_Step_textBox->text().toInt();
        if (ui->Sequential_radioButton->isChecked() == true)
        {
            Timer_RandomTest->stop();
            Timer_ManualTest->stop();
            Timer_Test->start(next_step_time);
        }
        else if(ui->Random_radioButton->isChecked() == true)
        {
            Timer_Test->stop();
            Timer_ManualTest->stop();
            Timer_RandomTest->start(next_step_time);
        }
        else
        {
            if(slaveTest_valid_format == true)
            {
                Complete_Stop_Check_1 = true;
                Complete_Stop_Check_2 = true;
                Complete_Stop_Check_3 = true;
                Timer_ManualTest->start(next_step_time);
            }
            else
            {
                QMessageBox::critical(nullptr,"ERROR FORMAT","Slave Test is invalid format");
            }
            Timer_Test->stop();
            Timer_RandomTest->stop();
        }
    }
}


void response_time_testing_42ev::on_Next_Step_textBox_editingFinished()
{
    response_time_autoTest->next_Step_Time = ui->Next_Step_textBox->text().toInt();
}


void response_time_testing_42ev::on_Max_Response_textBox_editingFinished()
{
    max_response_time = ui->Max_Response_textBox->text().toInt();
    response_time_autoTest->max_Response_Time = max_response_time;
}


void response_time_testing_42ev::on_Verify_Step_textBox_editingFinished()
{
    response_time_autoTest->verify_Steps = ui->Verify_Step_textBox->text().toInt();
}


void response_time_testing_42ev::on_Valve_Test_textBox_editingFinished()
{
    response_time_autoTest->Valve_Test = ui->Valve_Test_textBox->text();
}


void response_time_testing_42ev::on_Valve_Test_textBox_textChanged(const QString &arg1)
{
    if(Check_ValveTest_String(ui->Valve_Test_textBox->text()))
    {
        ui->Valve_Test_textBox->setStyleSheet("color:black;");
        slaveTest_valid_format = true;
    }
    else
    {
        ui->Valve_Test_textBox->setStyleSheet("color:red;");
        slaveTest_valid_format = false;
    }
}


void response_time_testing_42ev::on_pushButton_clicked()
{
    // Response_Time_Chart* form = new Response_Time_Chart();
    // form->Signal = valve_manual_test;
    // form->LoadForm();
    // form->setAttribute(Qt::WA_DeleteOnClose);
    // form->show();
}

void response_time_testing_42ev::RunIOTest()
{
    int verify_step = ui->Verify_Step_textBox->text().toInt();
    int valve = temp_42EV/8;
    int valve_bit = temp_42EV%8;
    bit_change = valve_bit;


    if(Is_Start_Test == true && Is_Test_Fail == false && step > 0)
    {
        ui->Current_Step_textBox->setText(QString(" %1 /%2").arg(verify_step - step + 1).arg(verify_step));
        if(Valve_State == VALVE_ON)
        {
            processData_mutex.lock();
            if(slave1_test <= ctx.slavecount)
            {
                IOmap[PDO_mapping_output[slave1_test - 1].address[PDO_ValveControl_1] + valve] |= (1 <<valve_bit);
            }
            if(slave2_test <= ctx.slavecount)
            {
                IOmap[PDO_mapping_output[slave2_test - 1].address[PDO_ValveControl_2] + valve] |= (1 <<valve_bit);
            }
            if(slave3_test <= ctx.slavecount)
            {
                IOmap[PDO_mapping_output[slave3_test - 1].address[PDO_ValveControl_3] + valve] |= (1 <<valve_bit);
            }
            processData_mutex.unlock();
            check_time_response(valve,valve_bit);

        }
        else
        {
            processData_mutex.lock();
            if(slave1_test <= ctx.slavecount)
            {
                IOmap[PDO_mapping_output[slave1_test - 1].address[PDO_ValveControl_1] + valve] &= ~(1 <<valve_bit);
            }
            if(slave2_test <= ctx.slavecount)
            {
                IOmap[PDO_mapping_output[slave2_test - 1].address[PDO_ValveControl_2] + valve] &= ~(1 <<valve_bit);
            }
            if(slave3_test <= ctx.slavecount)
            {
                IOmap[PDO_mapping_output[slave3_test - 1].address[PDO_ValveControl_3] + valve] &= ~(1 <<valve_bit);
            }
            processData_mutex.unlock();
            check_time_response(valve,valve_bit);
        }
        temp_42EV++;
        if(temp_42EV == 42)
        {
            temp_42EV = 0;
            if(Valve_State == VALVE_OFF)
            {
                Valve_State = VALVE_ON;
                step--;
                bool Is_Test_ValveReadback = true;
                if(response_time_io_1->valve_readback_1to8 != 0 && Is_Test_ValveReadback == true && slave1_test <= ctx.slavecount)
                {
                    Is_Test_Fail = true;
                    Is_Test_ValveReadback = false;
                    logStatus(QString("Slave %1 - Valve 1 to 8: Readback Error").arg(slave1_test));
                }
                if(response_time_io_1->valve_readback_9to16 != 0 && Is_Test_ValveReadback == true && slave1_test <= ctx.slavecount)
                {
                    Is_Test_Fail = true;
                    Is_Test_ValveReadback = false;
                    logStatus(QString("Slave %1 - Valve 9 to 16: Readback Error").arg(slave1_test));
                }
                if(response_time_io_1->valve_readback_17to24 != 0 && Is_Test_ValveReadback == true && slave1_test <= ctx.slavecount)
                {
                    Is_Test_Fail = true;
                    Is_Test_ValveReadback = false;
                    logStatus(QString("Slave %1 - Valve 17 to 24: Readback Error").arg(slave1_test));
                }
                if(response_time_io_1->valve_readback_25to32 != 0 && Is_Test_ValveReadback == true && slave1_test <= ctx.slavecount)
                {
                    Is_Test_Fail = true;
                    Is_Test_ValveReadback = false;
                    logStatus(QString("Slave %1 - Valve 25 to 32: Readback Error").arg(slave1_test));
                }
                if(response_time_io_1->valve_readback_33to40 != 0 && Is_Test_ValveReadback == true && slave1_test <= ctx.slavecount)
                {
                    Is_Test_Fail = true;
                    Is_Test_ValveReadback = false;
                    logStatus(QString("Slave %1 - Valve 33 to 40: Readback Error").arg(slave1_test));
                }
                if(response_time_io_1->valve_readback_41to48 != 0 && Is_Test_ValveReadback == true && slave1_test <= ctx.slavecount)
                {
                    Is_Test_Fail = true;
                    Is_Test_ValveReadback = false;
                    logStatus(QString("Slave %1 - Valve 41 to 42: Readback Error").arg(slave1_test));
                }


                if(response_time_io_2->valve_readback_1to8 != 0 && Is_Test_ValveReadback == true && slave2_test <= ctx.slavecount)
                {
                    Is_Test_Fail = true;
                    Is_Test_ValveReadback = false;
                    logStatus(QString("Slave %1 - Valve 1 to 8: Readback Error").arg(slave2_test));
                }
                if(response_time_io_2->valve_readback_9to16 != 0 && Is_Test_ValveReadback == true && slave2_test <= ctx.slavecount)
                {
                    Is_Test_Fail = true;
                    Is_Test_ValveReadback = false;
                    logStatus(QString("Slave %1 - Valve 9 to 16: Readback Error").arg(slave2_test));
                }
                if(response_time_io_2->valve_readback_17to24 != 0 && Is_Test_ValveReadback == true && slave2_test <= ctx.slavecount)
                {
                    Is_Test_Fail = true;
                    Is_Test_ValveReadback = false;
                    logStatus(QString("Slave %1 - Valve 17 to 24: Readback Error").arg(slave2_test));
                }
                if(response_time_io_2->valve_readback_25to32 != 0 && Is_Test_ValveReadback == true && slave2_test <= ctx.slavecount)
                {
                    Is_Test_Fail = true;
                    Is_Test_ValveReadback = false;
                    logStatus(QString("Slave %1 - Valve 25 to 32: Readback Error").arg(slave2_test));
                }
                if(response_time_io_2->valve_readback_33to40 != 0 && Is_Test_ValveReadback == true && slave2_test <= ctx.slavecount)
                {
                    Is_Test_Fail = true;
                    Is_Test_ValveReadback = false;
                    logStatus(QString("Slave %1 - Valve 33 to 40: Readback Error").arg(slave2_test));
                }
                if(response_time_io_2->valve_readback_41to48 != 0 && Is_Test_ValveReadback == true && slave2_test <= ctx.slavecount)
                {
                    Is_Test_Fail = true;
                    Is_Test_ValveReadback = false;
                    logStatus(QString("Slave %1 - Valve 41 to 42: Readback Error").arg(slave2_test));
                }

                if(response_time_io_3->valve_readback_1to8 != 0 && Is_Test_ValveReadback == true && slave3_test <= ctx.slavecount)
                {
                    Is_Test_Fail = true;
                    Is_Test_ValveReadback = false;
                    logStatus(QString("Slave %1 - Valve 1 to 8: Readback Error").arg(slave3_test));
                }
                if(response_time_io_3->valve_readback_9to16 != 0 && Is_Test_ValveReadback == true && slave3_test <= ctx.slavecount)
                {
                    Is_Test_Fail = true;
                    Is_Test_ValveReadback = false;
                    logStatus(QString("Slave %1 - Valve 9 to 16: Readback Error").arg(slave3_test));
                }
                if(response_time_io_3->valve_readback_17to24 != 0 && Is_Test_ValveReadback == true && slave3_test <= ctx.slavecount)
                {
                    Is_Test_Fail = true;
                    Is_Test_ValveReadback = false;
                    logStatus(QString("Slave %1 - Valve 17 to 24: Readback Error").arg(slave3_test));
                }
                if(response_time_io_3->valve_readback_25to32 != 0 && Is_Test_ValveReadback == true && slave3_test <= ctx.slavecount)
                {
                    Is_Test_Fail = true;
                    Is_Test_ValveReadback = false;
                    logStatus(QString("Slave %1 - Valve 25 to 32: Readback Error").arg(slave3_test));
                }
                if(response_time_io_3->valve_readback_33to40 != 0 && Is_Test_ValveReadback == true && slave3_test <= ctx.slavecount)
                {
                    Is_Test_Fail = true;
                    Is_Test_ValveReadback = false;
                    logStatus(QString("Slave %1 - Valve 33 to 40: Readback Error").arg(slave3_test));
                }
                if(response_time_io_3->valve_readback_41to48 != 0 && Is_Test_ValveReadback == true && slave3_test <= ctx.slavecount)
                {
                    Is_Test_Fail = true;
                    Is_Test_ValveReadback = false;
                    logStatus(QString("Slave %1 - Valve 41 to 42: Readback Error").arg(slave3_test));
                }
            }
            else
            {
                Valve_State = VALVE_OFF;
                bool Is_Test_ValveReadback = true;
                if(response_time_io_1->valve_readback_1to8 !=255 && Is_Test_ValveReadback == true && slave1_test <= ctx.slavecount)
                {
                    Is_Test_Fail = true;
                    Is_Test_ValveReadback = false;
                    logStatus(QString("Slave %1 - Valve 1 to 8: Readback Error").arg(slave1_test));
                }
                if(response_time_io_1->valve_readback_9to16 !=255 && Is_Test_ValveReadback == true && slave1_test <= ctx.slavecount)
                {
                    Is_Test_Fail = true;
                    Is_Test_ValveReadback = false;
                    logStatus(QString("Slave %1 - Valve 9 to 16: Readback Error").arg(slave1_test));
                }
                if(response_time_io_1->valve_readback_17to24 !=255 && Is_Test_ValveReadback == true && slave1_test <= ctx.slavecount)
                {
                    Is_Test_Fail = true;
                    Is_Test_ValveReadback = false;
                    logStatus(QString("Slave %1 - Valve 17 to 24: Readback Error").arg(slave1_test));
                }
                if(response_time_io_1->valve_readback_25to32 !=255 && Is_Test_ValveReadback == true && slave1_test <= ctx.slavecount)
                {
                    Is_Test_Fail = true;
                    Is_Test_ValveReadback = false;
                    logStatus(QString("Slave %1 - Valve 25 to 32: Readback Error").arg(slave1_test));
                }
                if(response_time_io_1->valve_readback_33to40 !=255 && Is_Test_ValveReadback == true && slave1_test <= ctx.slavecount)
                {
                    Is_Test_Fail = true;
                    Is_Test_ValveReadback = false;
                    logStatus(QString("Slave %1 - Valve 33 to 40: Readback Error").arg(slave1_test));
                }
                if((response_time_io_1->valve_readback_41to48 & 0x03) != 0x03 && Is_Test_ValveReadback == true && slave1_test <= ctx.slavecount)
                {
                    Is_Test_Fail = true;
                    Is_Test_ValveReadback = false;
                    logStatus(QString("Slave %1 - Valve 41 to 42: Readback Error").arg(slave1_test));
                }


                if(response_time_io_2->valve_readback_1to8 !=255 && Is_Test_ValveReadback == true && slave2_test <= ctx.slavecount)
                {
                    Is_Test_Fail = true;
                    Is_Test_ValveReadback = false;
                    logStatus(QString("Slave %1 - Valve 1 to 8: Readback Error").arg(slave2_test));
                }
                if(response_time_io_2->valve_readback_9to16 !=255 && Is_Test_ValveReadback == true && slave2_test <= ctx.slavecount)
                {
                    Is_Test_Fail = true;
                    Is_Test_ValveReadback = false;
                    logStatus(QString("Slave %1 - Valve 9 to 16: Readback Error").arg(slave2_test));
                }
                if(response_time_io_2->valve_readback_17to24 !=255 && Is_Test_ValveReadback == true && slave2_test <= ctx.slavecount)
                {
                    Is_Test_Fail = true;
                    Is_Test_ValveReadback = false;
                    logStatus(QString("Slave %1 - Valve 17 to 24: Readback Error").arg(slave2_test));
                }
                if(response_time_io_2->valve_readback_25to32 !=255 && Is_Test_ValveReadback == true && slave2_test <= ctx.slavecount)
                {
                    Is_Test_Fail = true;
                    Is_Test_ValveReadback = false;
                    logStatus(QString("Slave %1 - Valve 25 to 32: Readback Error").arg(slave2_test));
                }
                if(response_time_io_2->valve_readback_33to40 !=255 && Is_Test_ValveReadback == true && slave2_test <= ctx.slavecount)
                {
                    Is_Test_Fail = true;
                    Is_Test_ValveReadback = false;
                    logStatus(QString("Slave %1 - Valve 33 to 40: Readback Error").arg(slave2_test));
                }
                if((response_time_io_2->valve_readback_41to48 & 0x03) != 0x03 && Is_Test_ValveReadback == true && slave2_test <= ctx.slavecount)
                {
                    Is_Test_Fail = true;
                    Is_Test_ValveReadback = false;
                    logStatus(QString("Slave %1 - Valve 41 to 42: Readback Error").arg(slave2_test));
                }

                if(response_time_io_3->valve_readback_1to8 !=255 && Is_Test_ValveReadback == true && slave3_test <= ctx.slavecount)
                {
                    Is_Test_Fail = true;
                    Is_Test_ValveReadback = false;
                    logStatus(QString("Slave %1 - Valve 1 to 8: Readback Error").arg(slave3_test));
                }
                if(response_time_io_3->valve_readback_9to16 !=255 && Is_Test_ValveReadback == true && slave3_test <= ctx.slavecount)
                {
                    Is_Test_Fail = true;
                    Is_Test_ValveReadback = false;
                    logStatus(QString("Slave %1 - Valve 9 to 16: Readback Error").arg(slave3_test));
                }
                if(response_time_io_3->valve_readback_17to24 !=255 && Is_Test_ValveReadback == true && slave3_test <= ctx.slavecount)
                {
                    Is_Test_Fail = true;
                    Is_Test_ValveReadback = false;
                    logStatus(QString("Slave %1 - Valve 17 to 24: Readback Error").arg(slave3_test));
                }
                if(response_time_io_3->valve_readback_25to32 !=255 && Is_Test_ValveReadback == true && slave3_test <= ctx.slavecount)
                {
                    Is_Test_Fail = true;
                    Is_Test_ValveReadback = false;
                    logStatus(QString("Slave %1 - Valve 25 to 32: Readback Error").arg(slave3_test));
                }
                if(response_time_io_3->valve_readback_33to40 !=255 && Is_Test_ValveReadback == true && slave3_test <= ctx.slavecount)
                {
                    Is_Test_Fail = true;
                    Is_Test_ValveReadback = false;
                    logStatus(QString("Slave %1 - Valve 33 to 40: Readback Error").arg(slave3_test));
                }
                if((response_time_io_3->valve_readback_41to48 & 0x03) != 0x03 && Is_Test_ValveReadback == true && slave3_test <= ctx.slavecount)
                {
                    Is_Test_Fail = true;
                    Is_Test_ValveReadback = false;
                    logStatus(QString("Slave %1 - Valve 41 to 42: Readback Error").arg(slave3_test));
                }
            }
        }

        if(Is_Test_Fail == true)
        {
            FinalTest_Log logs;
            logs.test_name = "Response Time 42EV";
            logs.data["status"] = "Fail";
            QStringList outlogs;
            outlogs << QString("Max ON Time: %1ms, Min ON Time: %2ms").arg(Max_Response_OnTime_1,0,'f',2).arg(Min_Response_OnTime_1,0,'f',2);
            outlogs << QString("Max OFF Time: %1ms, Min OFF Time: %2ms").arg(Max_Response_OffTime_1,0,'f',2).arg(Min_Response_OffTime_1,0,'f',2);
            logs.message = outlogs.join("\n");
            emit logTest(logs);
            logStatus("Sequential test failed");
            logStatus("Stop test");
            Is_Start_Test = false;
            ui->Start_Test_button->setText("Start Test");
            ui->Next_Step_textBox->setEnabled(true);
            ui->Max_Response_textBox->setEnabled(true);
            ui->Verify_Step_textBox->setEnabled(true);
        }
        if(step == 0)
        {
            FinalTest_Log logs;
            logs.test_name = "Response Time 42EV";
            logs.data["status"] = "Pass";
            QStringList outlogs;
            if(slave1_test <= slave_count)
            {
                logStatus(QString("Slave %1 - Max Response ON Time: %2ms, Min Response ON Time: %3ms").arg(slave1_test).arg(Max_Response_OnTime_1,0,'f',2).arg(Min_Response_OnTime_1,0,'f',2));
                logStatus(QString("Slave %1 - Max Response OFF Time: %2ms, Min Response OFF Time: %3ms").arg(slave1_test).arg(Max_Response_OffTime_1,0,'f',2).arg(Min_Response_OffTime_1,0,'f',2));

                outlogs << QString("Max ON Time: %1ms, Min ON Time: %2ms").arg(Max_Response_OnTime_1,0,'f',2).arg(Min_Response_OnTime_1,0,'f',2);
                outlogs << QString("Max OFF Time: %1ms, Min OFF Time: %2ms").arg(Max_Response_OffTime_1,0,'f',2).arg(Min_Response_OffTime_1,0,'f',2);
            }
            if(slave2_test <= slave_count)
            {
                logStatus(QString("Slave %1 - Max Response ON Time: %2ms, Min Response ON Time: %3ms").arg(slave2_test).arg(Max_Response_OnTime_2,0,'f',2).arg(Min_Response_OnTime_2,0,'f',2));
                logStatus(QString("Slave %1 - Max Response OFF Time: %2ms, Min Response OFF Time: %3ms").arg(slave2_test).arg(Max_Response_OffTime_2,0,'f',2).arg(Min_Response_OffTime_2,0,'f',2));
            }
            if(slave3_test <= slave_count)
            {
                logStatus(QString("Slave %1 - Max Response ON Time: %2ms, Min Response ON Time: %3ms").arg(slave3_test).arg(Max_Response_OnTime_3,0,'f',2).arg(Min_Response_OnTime_3,0,'f',2));
                logStatus(QString("Slave %1 - Max Response OFF Time: %2ms, Min Response OFF Time: %3ms").arg(slave3_test).arg(Max_Response_OffTime_3,0,'f',2).arg(Min_Response_OffTime_3,0,'f',2));
            }
            logs.message = outlogs.join("\n");
            emit logTest(logs);
            logStatus("Sequential test completed successfully");
            logStatus("Stop Test");
            ui->Start_Test_button->setText("Start Test");
            Is_Start_Test = false;
            ui->Next_Step_textBox->setEnabled(true);
            ui->Max_Response_textBox->setEnabled(true);
            ui->Verify_Step_textBox->setEnabled(true);
        }

        display_ValveControl();
        display_ValveReadback();
    }
}

void response_time_testing_42ev::RunRandomTest()
{
    Is_checking_ValveChange = false;
    int verify_step = ui->Verify_Step_textBox->text().toInt();
    static int numValveRandom;
    if(Is_Start_Test == true && Is_Test_Fail == false && step > 0)
    {
        int next_valve = 0;
        if(Valve_State == VALVE_ON)
        {
            ui->Current_Step_textBox->setText(QString(" %1 /%2").arg(verify_step - step + 1).arg(verify_step));
            for (int row = 0; row < 6; row++)
            {
                for (int col = 0; col < 8; col++)
                {
                    ui->Response_Off_Time_TableWidget_1->setItem(row, col,new QTableWidgetItem(""));
                    ui->Response_Off_Time_TableWidget_1->item(row,col)->setBackground(Qt::white);
                    ui->Response_Off_Time_TableWidget_2->setItem(row, col,new QTableWidgetItem(""));
                    ui->Response_Off_Time_TableWidget_3->setItem(row, col,new QTableWidgetItem(""));
                }
            }
            memset(valve_random,0,sizeof(valve_random));
            numValveRandom = QRandomGenerator::global()->bounded(1, 43);
            for(int i = 0; i < numValveRandom; i++)
            {
                valve_random[i] = QRandomGenerator::global()->bounded(0,42);
            }
            processData_mutex.lock();
            while(next_valve < numValveRandom && next_valve < 42)
            {
                int valve = valve_random[next_valve] /8;
                int valve_bit = valve_random[next_valve] % 8;
                ui->Valve_Control_TableWidget_1->item(valve,7 - valve_bit)->setBackground(QColor(213,238,255));
                if(slave1_test <= ctx.slavecount)
                {
                    IOmap[PDO_mapping_output[slave1_test - 1].address[PDO_ValveControl_1] + valve] |= (1 <<valve_bit);
                }
                if(slave2_test <= ctx.slavecount)
                {
                    IOmap[PDO_mapping_output[slave2_test - 1].address[PDO_ValveControl_2] + valve] |= (1 <<valve_bit);
                }
                if(slave3_test <= ctx.slavecount)
                {
                    IOmap[PDO_mapping_output[slave3_test - 1].address[PDO_ValveControl_3] + valve] |= (1 <<valve_bit);
                }
                next_valve++;
            }
            processData_mutex.unlock();
            check_time_response_random(numValveRandom);
        }
        else
        {
            for (int row = 0; row < 6; row++)
            {
                for (int col = 0; col < 8; col++)
                {
                    ui->Response_On_Time_TableWidget_1->setItem(row, col,new QTableWidgetItem(""));
                    ui->Response_On_Time_TableWidget_1->item(row,col)->setBackground(Qt::white);
                    ui->Response_On_Time_TableWidget_2->setItem(row, col,new QTableWidgetItem(""));
                    ui->Response_On_Time_TableWidget_3->setItem(row, col,new QTableWidgetItem(""));
                    ui->Valve_Control_TableWidget_1->item(row,col)->setBackground(Qt::white);
                }
            }
            next_valve = 0;
            processData_mutex.lock();
            while(next_valve < numValveRandom && next_valve < 42)
            {
                int valve = valve_random[next_valve] /8;
                int valve_bit = valve_random[next_valve] % 8;
                if(slave1_test <= ctx.slavecount)
                {
                    IOmap[PDO_mapping_output[slave1_test - 1].address[PDO_ValveControl_1] + valve] &= ~(1 <<valve_bit);
                }
                if(slave2_test <= ctx.slavecount)
                {
                    IOmap[PDO_mapping_output[slave2_test - 1].address[PDO_ValveControl_2] + valve] &= ~(1 <<valve_bit);
                }
                if(slave3_test <= ctx.slavecount)
                {
                    IOmap[PDO_mapping_output[slave3_test - 1].address[PDO_ValveControl_3] + valve] &= ~(1 <<valve_bit);
                }
                next_valve++;
            }
            processData_mutex.unlock();
            check_time_response_random(numValveRandom);
        }
        if(Valve_State == VALVE_ON)
        {
            Valve_State = VALVE_OFF;
        }
        else
        {
            Valve_State = VALVE_ON;
            step--;
        }
        if(Is_Test_Fail == true)
        {
            FinalTest_Log logs;
            logs.test_name = "Random 42EV";
            logs.data["status"] = "Pass";
            QStringList outlogs;
            outlogs << QString("Max ON Time: %1ms, Min ON Time: %2ms").arg(Max_Response_OnTime_1,0,'f',2).arg(Min_Response_OnTime_1,0,'f',2);
            outlogs << QString("Max OFF Time: %1ms, Min OFF Time: %2ms").arg(Max_Response_OffTime_1,0,'f',2).arg(Min_Response_OffTime_1,0,'f',2);
            logs.message = outlogs.join("\n");
            emit logTest(logs);

            logStatus("Random test failed");
            logStatus("Stop test");
            Is_Start_Test = false;
            ui->Start_Test_button->setText("Start Test");
            ui->Next_Step_textBox->setEnabled(true);
            ui->Max_Response_textBox->setEnabled(true);
            ui->Verify_Step_textBox->setEnabled(true);
        }
        if(step == 0)
        {
            FinalTest_Log logs;
            logs.test_name = "Random 42EV";
            logs.data["status"] = "Pass";
            emit logTest(logs);

            logStatus("Random test completed successfully");
            logStatus("Stop Test");
            ui->Start_Test_button->setText("Start Test");
            Is_Start_Test = false;
            ui->Next_Step_textBox->setEnabled(true);
            ui->Max_Response_textBox->setEnabled(true);
            ui->Verify_Step_textBox->setEnabled(true);
        }
    }

    display_ValveControl();
    display_ValveReadback();
}

void response_time_testing_42ev::RunManualTest()
{
    Is_checking_ValveChange = false;
    while(Complete_Stop_Check_1 == false ||Complete_Stop_Check_2 == false ||Complete_Stop_Check_3 == false );
    int verify_step = ui->Verify_Step_textBox->text().toInt();
    int numValve_Manual = valve_manual_test.count();
    if(Is_Start_Test == true && Is_Test_Fail == false && step > 0)
    {
        int next_valve = 0;
        ui->Current_Step_textBox->setText(QString(" %1 /%2").arg(verify_step - step + 1).arg(verify_step));
        if(step % 2 == 0)
        {
            for (int row = 0; row < 6; row++)
            {
                for (int col = 0; col < 8; col++)
                {
                    ui->Response_Off_Time_TableWidget_1->setItem(row, col,new QTableWidgetItem(""));
                    ui->Response_Off_Time_TableWidget_2->setItem(row, col,new QTableWidgetItem(""));
                    ui->Response_Off_Time_TableWidget_3->setItem(row, col,new QTableWidgetItem(""));
                }
            }

            for (int row = 0; row < 6; row++)
            {
                for (int col = 0; col < 8; col++)
                {
                    ui->Response_On_Time_TableWidget_1->setItem(row, col,new QTableWidgetItem(""));
                    ui->Response_On_Time_TableWidget_2->setItem(row, col,new QTableWidgetItem(""));
                    ui->Response_On_Time_TableWidget_3->setItem(row, col,new QTableWidgetItem(""));
                }
            }
            processData_mutex.lock();
            while(next_valve < numValve_Manual && next_valve < 42)
            {
                int valve = valve_manual_test[next_valve] /8;
                int valve_bit = valve_manual_test[next_valve] % 8;
                if(next_valve % 2 == 0)
                {
                    if(slave1_test <= ctx.slavecount)
                    {
                        IOmap[PDO_mapping_output[slave1_test - 1].address[PDO_ValveControl_1] + valve] |= (1 <<valve_bit);
                    }
                    if(slave2_test <= ctx.slavecount)
                    {
                        IOmap[PDO_mapping_output[slave2_test - 1].address[PDO_ValveControl_2] + valve] |= (1 <<valve_bit);
                    }
                    if(slave3_test <= ctx.slavecount)
                    {
                        IOmap[PDO_mapping_output[slave3_test - 1].address[PDO_ValveControl_3] + valve] |= (1 <<valve_bit);
                    }
                }
                else
                {
                    if(slave1_test <= ctx.slavecount)
                    {
                        IOmap[PDO_mapping_output[slave1_test - 1].address[PDO_ValveControl_1] + valve] &= ~(1 <<valve_bit);
                    }
                    if(slave2_test <= ctx.slavecount)
                    {
                        IOmap[PDO_mapping_output[slave2_test - 1].address[PDO_ValveControl_2] + valve] &= ~(1 <<valve_bit);
                    }
                    if(slave3_test <= ctx.slavecount)
                    {
                        IOmap[PDO_mapping_output[slave3_test - 1].address[PDO_ValveControl_3] + valve] &= ~(1 <<valve_bit);
                    }
                }
                next_valve++;
            }
            processData_mutex.unlock();
            check_time_response_manual(numValve_Manual);
        }
        else
        {

            next_valve = 0;
            processData_mutex.lock();
            while(next_valve < numValve_Manual && next_valve < 42)
            {
                int valve = valve_manual_test[next_valve] /8;
                int valve_bit = valve_manual_test[next_valve] % 8;
                if(next_valve % 2 == 0)
                {
                    if(slave1_test <= ctx.slavecount)
                    {
                        IOmap[PDO_mapping_output[slave1_test - 1].address[PDO_ValveControl_1] + valve] &= ~(1 <<valve_bit);
                    }
                    if(slave2_test <= ctx.slavecount)
                    {
                        IOmap[PDO_mapping_output[slave2_test - 1].address[PDO_ValveControl_2] + valve] &= ~(1 <<valve_bit);
                    }
                    if(slave3_test <= ctx.slavecount)
                    {
                        IOmap[PDO_mapping_output[slave3_test - 1].address[PDO_ValveControl_3] + valve] &= ~(1 <<valve_bit);
                    }
                }
                else
                {
                    if(slave1_test <= ctx.slavecount)
                    {
                        IOmap[PDO_mapping_output[slave1_test - 1].address[PDO_ValveControl_1] + valve] |= (1 <<valve_bit);
                    }
                    if(slave2_test <= ctx.slavecount)
                    {
                        IOmap[PDO_mapping_output[slave2_test - 1].address[PDO_ValveControl_2] + valve] |= (1 <<valve_bit);
                    }
                    if(slave3_test <= ctx.slavecount)
                    {
                        IOmap[PDO_mapping_output[slave3_test - 1].address[PDO_ValveControl_3] + valve] |= (1 <<valve_bit);
                    }
                }
                next_valve++;
            }
            processData_mutex.unlock();
            check_time_response_manual(numValve_Manual);
        }

        step--;

        if(Is_Test_Fail == true)
        {
            FinalTest_Log logs;
            logs.test_name = "Manual 42EV";
            logs.data["status"] = "Fail";
            emit logTest(logs);

            logStatus("Manual test failed");
            logStatus("Stop test");
            Is_Start_Test = false;
            ui->Start_Test_button->setText("Start Test");
            ui->Next_Step_textBox->setEnabled(true);
            ui->Max_Response_textBox->setEnabled(true);
            ui->Verify_Step_textBox->setEnabled(true);
        }
        if(step == 0)
        {
            FinalTest_Log logs;
            logs.test_name = "Manual 42EV";
            logs.data["status"] = "Pass";
            emit logTest(logs);

            logStatus("Manual test completed successfully");
            logStatus("Stop Test");
            ui->Start_Test_button->setText("Start Test");
            Is_Start_Test = false;
            ui->Next_Step_textBox->setEnabled(true);
            ui->Max_Response_textBox->setEnabled(true);
            ui->Verify_Step_textBox->setEnabled(true);
        }
    }

    display_ValveControl();
    display_ValveReadback();
}

void response_time_testing_42ev::Read_Valve()
{
    processData_mutex.lock();
    if(slave1_test <= ctx.slavecount)
    {
        response_time_io_1->valve_control_1to8 = IOmap[PDO_mapping_output[slave1_test - 1].address[PDO_ValveControl_1]];
        response_time_io_1->valve_control_9to16 = IOmap[PDO_mapping_output[slave1_test - 1].address[PDO_ValveControl_1] + 1];
        response_time_io_1->valve_control_17to24 = IOmap[PDO_mapping_output[slave1_test - 1].address[PDO_ValveControl_1] + 2];
        response_time_io_1->valve_control_25to32 = IOmap[PDO_mapping_output[slave1_test - 1].address[PDO_ValveControl_1] + 3];
        response_time_io_1->valve_control_33to40 = IOmap[PDO_mapping_output[slave1_test - 1].address[PDO_ValveControl_1] + 4];
        response_time_io_1->valve_control_41to48 = IOmap[PDO_mapping_output[slave1_test - 1].address[PDO_ValveControl_1] + 5];

        response_time_io_1->valve_readback_1to8 = IOmap[PDO_mapping_input[slave1_test - 1].address[PDO_ValveReadback_1]];
        response_time_io_1->valve_readback_9to16 = IOmap[PDO_mapping_input[slave1_test - 1].address[PDO_ValveReadback_1] + 1];
        response_time_io_1->valve_readback_17to24 = IOmap[PDO_mapping_input[slave1_test - 1].address[PDO_ValveReadback_1] + 2];
        response_time_io_1->valve_readback_25to32 = IOmap[PDO_mapping_input[slave1_test - 1].address[PDO_ValveReadback_1] + 3];
        response_time_io_1->valve_readback_33to40 = IOmap[PDO_mapping_input[slave1_test - 1].address[PDO_ValveReadback_1] + 4];
        response_time_io_1->valve_readback_41to48 = IOmap[PDO_mapping_input[slave1_test - 1].address[PDO_ValveReadback_1] + 5];
    }
    else
    {
        response_time_io_1->valve_control_1to8 = 0;
        response_time_io_1->valve_control_9to16 = 0;
        response_time_io_1->valve_control_17to24 = 0;
        response_time_io_1->valve_control_25to32 = 0;
        response_time_io_1->valve_control_33to40 = 0;
        response_time_io_1->valve_control_41to48 = 0;

        response_time_io_1->valve_readback_1to8 = 0;
        response_time_io_1->valve_readback_9to16 = 0;
        response_time_io_1->valve_readback_17to24 = 0;
        response_time_io_1->valve_readback_25to32 = 0;
        response_time_io_1->valve_readback_33to40 = 0;
        response_time_io_1->valve_readback_41to48 = 0;
    }
    if(slave2_test <=ctx.slavecount)
    {
        response_time_io_2->valve_control_1to8 = IOmap[PDO_mapping_output[slave2_test - 1].address[PDO_ValveControl_2]];
        response_time_io_2->valve_control_9to16 = IOmap[PDO_mapping_output[slave2_test - 1].address[PDO_ValveControl_2] + 1];
        response_time_io_2->valve_control_17to24 = IOmap[PDO_mapping_output[slave2_test - 1].address[PDO_ValveControl_2] + 2];
        response_time_io_2->valve_control_25to32 = IOmap[PDO_mapping_output[slave2_test - 1].address[PDO_ValveControl_2] + 3];
        response_time_io_2->valve_control_33to40 = IOmap[PDO_mapping_output[slave2_test - 1].address[PDO_ValveControl_2] + 4];
        response_time_io_2->valve_control_41to48 = IOmap[PDO_mapping_output[slave2_test - 1].address[PDO_ValveControl_2] + 5];

        response_time_io_2->valve_readback_1to8 = IOmap[PDO_mapping_input[slave2_test - 1].address[PDO_ValveReadback_2]];
        response_time_io_2->valve_readback_9to16 = IOmap[PDO_mapping_input[slave2_test - 1].address[PDO_ValveReadback_2] + 1];
        response_time_io_2->valve_readback_17to24 = IOmap[PDO_mapping_input[slave2_test - 1].address[PDO_ValveReadback_2] + 2];
        response_time_io_2->valve_readback_25to32 = IOmap[PDO_mapping_input[slave2_test - 1].address[PDO_ValveReadback_2] + 3];
        response_time_io_2->valve_readback_33to40 = IOmap[PDO_mapping_input[slave2_test - 1].address[PDO_ValveReadback_2] + 4];
        response_time_io_2->valve_readback_41to48 = IOmap[PDO_mapping_input[slave2_test - 1].address[PDO_ValveReadback_2] + 5];
    }
    else
    {
        response_time_io_2->valve_control_1to8 = 0;
        response_time_io_2->valve_control_9to16 = 0;
        response_time_io_2->valve_control_17to24 = 0;
        response_time_io_2->valve_control_25to32 = 0;
        response_time_io_2->valve_control_33to40 =0;
        response_time_io_2->valve_control_41to48 = 0;

        response_time_io_2->valve_readback_1to8 = 0;
        response_time_io_2->valve_readback_9to16 =0;
        response_time_io_2->valve_readback_17to24 = 0;
        response_time_io_2->valve_readback_25to32 = 0;
        response_time_io_2->valve_readback_33to40 = 0;
        response_time_io_2->valve_readback_41to48 = 0;
    }

    if(slave3_test <= ctx.slavecount)
    {
        response_time_io_3->valve_control_1to8 = IOmap[PDO_mapping_output[slave3_test - 1].address[PDO_ValveControl_3]];
        response_time_io_3->valve_control_9to16 = IOmap[PDO_mapping_output[slave3_test - 1].address[PDO_ValveControl_3] + 1];
        response_time_io_3->valve_control_17to24 = IOmap[PDO_mapping_output[slave3_test - 1].address[PDO_ValveControl_3] + 2];
        response_time_io_3->valve_control_25to32 = IOmap[PDO_mapping_output[slave3_test - 1].address[PDO_ValveControl_3] + 3];
        response_time_io_3->valve_control_33to40 = IOmap[PDO_mapping_output[slave3_test - 1].address[PDO_ValveControl_3] + 4];
        response_time_io_3->valve_control_41to48 = IOmap[PDO_mapping_output[slave3_test - 1].address[PDO_ValveControl_3] + 5];

        response_time_io_3->valve_readback_1to8 = IOmap[PDO_mapping_input[slave3_test - 1].address[PDO_ValveReadback_3]];
        response_time_io_3->valve_readback_9to16 = IOmap[PDO_mapping_input[slave3_test - 1].address[PDO_ValveReadback_3] + 1];
        response_time_io_3->valve_readback_17to24 = IOmap[PDO_mapping_input[slave3_test - 1].address[PDO_ValveReadback_3] + 2];
        response_time_io_3->valve_readback_25to32 = IOmap[PDO_mapping_input[slave3_test - 1].address[PDO_ValveReadback_3] + 3];
        response_time_io_3->valve_readback_33to40 = IOmap[PDO_mapping_input[slave3_test - 1].address[PDO_ValveReadback_3] + 4];
        response_time_io_3->valve_readback_41to48 = IOmap[PDO_mapping_input[slave3_test - 1].address[PDO_ValveReadback_3] + 5];
    }
    else
    {
        response_time_io_3->valve_control_1to8 = 0;
        response_time_io_3->valve_control_9to16 = 0;
        response_time_io_3->valve_control_17to24 = 0;
        response_time_io_3->valve_control_25to32 = 0;
        response_time_io_3->valve_control_33to40 =0;
        response_time_io_3->valve_control_41to48 = 0;

        response_time_io_3->valve_readback_1to8 = 0;
        response_time_io_3->valve_readback_9to16 = 0;
        response_time_io_3->valve_readback_17to24 = 0;
        response_time_io_3->valve_readback_25to32 = 0;
        response_time_io_3->valve_readback_33to40 = 0;
        response_time_io_3->valve_readback_41to48 = 0;
    }
    processData_mutex.unlock();
}

void response_time_testing_42ev::display_ValveReadback()
{
    for (int i = 0; i < 8; i++)
    {
        ui->Valve_Readback_TableWidget_1->item(0,i)->setText(QString ("%1").arg((response_time_io_1->valve_readback_1to8 >> (7 - i)) & 0x01));
        ui->Valve_Readback_TableWidget_2->item(0,i)->setText(QString ("%1").arg((response_time_io_2->valve_readback_1to8 >> (7 - i)) & 0x01));
        ui->Valve_Readback_TableWidget_3->item(0,i)->setText(QString ("%1").arg((response_time_io_3->valve_readback_1to8 >> (7 - i)) & 0x01));
    }
    for (int i = 0; i < 8; i++)
    {
        ui->Valve_Readback_TableWidget_1->item(1,i)->setText(QString ("%1").arg((response_time_io_1->valve_readback_9to16 >> (7 - i)) & 0x01));
        ui->Valve_Readback_TableWidget_2->item(1,i)->setText(QString ("%1").arg((response_time_io_2->valve_readback_9to16 >> (7 - i)) & 0x01));
        ui->Valve_Readback_TableWidget_3->item(1,i)->setText(QString ("%1").arg((response_time_io_3->valve_readback_9to16 >> (7 - i)) & 0x01));
    }
    for (int i = 0; i < 8; i++)
    {
        ui->Valve_Readback_TableWidget_1->item(2,i)->setText(QString ("%1").arg((response_time_io_1->valve_readback_17to24 >> (7 - i)) & 0x01));
        ui->Valve_Readback_TableWidget_2->item(2,i)->setText(QString ("%1").arg((response_time_io_2->valve_readback_17to24 >> (7 - i)) & 0x01));
        ui->Valve_Readback_TableWidget_3->item(2,i)->setText(QString ("%1").arg((response_time_io_3->valve_readback_17to24 >> (7 - i)) & 0x01));
    }
    for (int i = 0; i < 8; i++)
    {
        ui->Valve_Readback_TableWidget_1->item(3,i)->setText(QString ("%1").arg((response_time_io_1->valve_readback_25to32 >> (7 - i)) & 0x01));
        ui->Valve_Readback_TableWidget_2->item(3,i)->setText(QString ("%1").arg((response_time_io_2->valve_readback_25to32 >> (7 - i)) & 0x01));
        ui->Valve_Readback_TableWidget_3->item(3,i)->setText(QString ("%1").arg((response_time_io_3->valve_readback_25to32 >> (7 - i)) & 0x01));
    }
    for (int i = 0; i < 8; i++)
    {
        ui->Valve_Readback_TableWidget_1->item(4,i)->setText(QString ("%1").arg((response_time_io_1->valve_readback_33to40 >> (7 - i)) & 0x01));
        ui->Valve_Readback_TableWidget_2->item(4,i)->setText(QString ("%1").arg((response_time_io_2->valve_readback_33to40 >> (7 - i)) & 0x01));
        ui->Valve_Readback_TableWidget_3->item(4,i)->setText(QString ("%1").arg((response_time_io_3->valve_readback_33to40 >> (7 - i)) & 0x01));
    }
    for (int i = 0; i < 8; i++)
    {
        ui->Valve_Readback_TableWidget_1->item(5,i)->setText(QString ("%1").arg((response_time_io_1->valve_readback_41to48 >> (7 - i)) & 0x01));
        ui->Valve_Readback_TableWidget_2->item(5,i)->setText(QString ("%1").arg((response_time_io_2->valve_readback_41to48 >> (7 - i)) & 0x01));
        ui->Valve_Readback_TableWidget_3->item(5,i)->setText(QString ("%1").arg((response_time_io_3->valve_readback_41to48 >> (7 - i)) & 0x01));
    }
}

void response_time_testing_42ev::display_ValveControl()
{
    for (int i = 0; i < 8; i++)
    {
        ui->Valve_Control_TableWidget_1->item(0,i)->setText(QString ("%1").arg((response_time_io_1->valve_control_1to8 >> (7 - i)) & 0x01));
        ui->Valve_Control_TableWidget_2->item(0,i)->setText(QString ("%1").arg((response_time_io_2->valve_control_1to8 >> (7 - i)) & 0x01));
        ui->Valve_Control_TableWidget_3->item(0,i)->setText(QString ("%1").arg((response_time_io_3->valve_control_1to8 >> (7 - i)) & 0x01));
    }
    for (int i = 0; i < 8; i++)
    {
        ui->Valve_Control_TableWidget_1->item(1,i)->setText(QString ("%1").arg((response_time_io_1->valve_control_9to16 >> (7 - i)) & 0x01));
        ui->Valve_Control_TableWidget_2->item(1,i)->setText(QString ("%1").arg((response_time_io_2->valve_control_9to16 >> (7 - i)) & 0x01));
        ui->Valve_Control_TableWidget_3->item(1,i)->setText(QString ("%1").arg((response_time_io_3->valve_control_9to16 >> (7 - i)) & 0x01));
    }
    for (int i = 0; i < 8; i++)
    {
        ui->Valve_Control_TableWidget_1->item(2,i)->setText(QString ("%1").arg((response_time_io_1->valve_control_17to24 >> (7 - i)) & 0x01));
        ui->Valve_Control_TableWidget_2->item(2,i)->setText(QString ("%1").arg((response_time_io_2->valve_control_17to24 >> (7 - i)) & 0x01));
        ui->Valve_Control_TableWidget_3->item(2,i)->setText(QString ("%1").arg((response_time_io_3->valve_control_17to24 >> (7 - i)) & 0x01));
    }
    for (int i = 0; i < 8; i++)
    {
        ui->Valve_Control_TableWidget_1->item(3,i)->setText(QString ("%1").arg((response_time_io_1->valve_control_25to32 >> (7 - i)) & 0x01));
        ui->Valve_Control_TableWidget_2->item(3,i)->setText(QString ("%1").arg((response_time_io_2->valve_control_25to32 >> (7 - i)) & 0x01));
        ui->Valve_Control_TableWidget_3->item(3,i)->setText(QString ("%1").arg((response_time_io_3->valve_control_25to32 >> (7 - i)) & 0x01));
    }
    for (int i = 0; i < 8; i++)
    {
        ui->Valve_Control_TableWidget_1->item(4,i)->setText(QString ("%1").arg((response_time_io_1->valve_control_33to40 >> (7 - i)) & 0x01));
        ui->Valve_Control_TableWidget_2->item(4,i)->setText(QString ("%1").arg((response_time_io_2->valve_control_33to40 >> (7 - i)) & 0x01));
        ui->Valve_Control_TableWidget_3->item(4,i)->setText(QString ("%1").arg((response_time_io_3->valve_control_33to40 >> (7 - i)) & 0x01));
    }
    for (int i = 0; i < 8; i++)
    {
        ui->Valve_Control_TableWidget_1->item(5,i)->setText(QString ("%1").arg((response_time_io_1->valve_control_41to48 >> (7 - i)) & 0x01));
        ui->Valve_Control_TableWidget_2->item(5,i)->setText(QString ("%1").arg((response_time_io_2->valve_control_41to48 >> (7 - i)) & 0x01));
        ui->Valve_Control_TableWidget_3->item(5,i)->setText(QString ("%1").arg((response_time_io_3->valve_control_41to48 >> (7 - i)) & 0x01));
    }
}

void response_time_testing_42ev::logStatus(QString msg)
{
    if (QThread::currentThread() != this->thread()) {
        QMetaObject::invokeMethod(this, [=]() {
            logStatus(msg);
        }, Qt::QueuedConnection);
        return;
    }
    QString timestamp = QDateTime::currentDateTime().toString("MM/dd HH:mm:ss|");
    QString message = QString("%1 %2").arg(timestamp,msg);
    ui->Status_textBox->appendPlainText(message);
    QCoreApplication::processEvents();
    logBuffer.append(message);
}

void response_time_testing_42ev::updateStartTestButton(QString msg)
{
    ui->Start_Test_button->setText(msg);
    ui->Next_Step_textBox->setEnabled(true);
    ui->Max_Response_textBox->setEnabled(true);
    ui->Verify_Step_textBox->setEnabled(true);
}

void response_time_testing_42ev::resetTableWidget()
{
    for (int row = 0; row < 6; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            ui->Valve_Control_TableWidget_1->setItem(row, col,new QTableWidgetItem(""));
            ui->Valve_Readback_TableWidget_1->setItem(row, col,new QTableWidgetItem(""));
            ui->Response_On_Time_TableWidget_1->setItem(row, col,new QTableWidgetItem(""));
            ui->Response_Off_Time_TableWidget_1->setItem(row, col,new QTableWidgetItem(""));

            ui->Valve_Control_TableWidget_2->setItem(row, col,new QTableWidgetItem(""));
            ui->Valve_Readback_TableWidget_2->setItem(row, col,new QTableWidgetItem(""));
            ui->Response_On_Time_TableWidget_2->setItem(row, col,new QTableWidgetItem(""));
            ui->Response_Off_Time_TableWidget_2->setItem(row, col,new QTableWidgetItem(""));

            ui->Valve_Control_TableWidget_3->setItem(row, col,new QTableWidgetItem(""));
            ui->Valve_Readback_TableWidget_3->setItem(row, col,new QTableWidgetItem(""));
            ui->Response_On_Time_TableWidget_3->setItem(row, col,new QTableWidgetItem(""));
            ui->Response_Off_Time_TableWidget_3->setItem(row, col,new QTableWidgetItem(""));
        }
    }
}

void response_time_testing_42ev::check_time_response(int valve, int valve_bit)
{
    QElapsedTimer Timer_count;
    qint64 time_elapsed;
    int invalve_1, outvalve_1;
    int invalve_2, outvalve_2;
    int invalve_3, outvalve_3;
    int inbit_1, outbit_1;
    int inbit_2, outbit_2;
    int inbit_3, outbit_3;
    bool complete_test_1 = false;
    bool complete_test_2 = false;
    bool complete_test_3 = false;
    Timer_count.start();
    if(slave1_test > slave_count)
    {
        complete_test_1 = true;
    }
    if(slave2_test > slave_count)
    {
        complete_test_2 = true;
    }
    if(slave3_test > slave_count)
    {
        complete_test_3 = true;
    }
    while(Is_Start_Test)
    {
        Read_Valve();
        time_elapsed = Timer_count.nsecsElapsed();
        double timeElapsed = time_elapsed/1e6;
        if(timeElapsed > max_response_time)
        {
            Is_Test_Fail = true;
            if(complete_test_1 == false)
            {
                if(Valve_State == VALVE_ON)
                {
                    ui->Response_On_Time_TableWidget_1->item(valve,7 - valve_bit)->setText("ERR");
                }
                else
                {
                    ui->Response_Off_Time_TableWidget_1->item(valve,7 - valve_bit)->setText("ERR");
                }
                logStatus(QString("Slave %1 - Valve %2: Timeout error.").arg(slave1_test).arg(valve * 8 + valve_bit + 1));
            }

            if(complete_test_2 == false)
            {
                if(Valve_State == VALVE_ON)
                {
                    ui->Response_On_Time_TableWidget_2->item(valve,7 - valve_bit)->setText("ERR");
                }
                else
                {
                    ui->Response_Off_Time_TableWidget_2->item(valve,7 - valve_bit)->setText("ERR");
                }
                logStatus(QString("Slave %1 - Valve %2: Timeout error.").arg(slave2_test).arg(valve * 8 + valve_bit + 1));
            }
            if(complete_test_3 == false)
            {
                if(Valve_State == VALVE_ON)
                {
                    ui->Response_On_Time_TableWidget_3->item(valve,7 - valve_bit)->setText("ERR");
                }
                else
                {
                    ui->Response_Off_Time_TableWidget_3->item(valve,7 - valve_bit)->setText("ERR");
                }
                logStatus(QString("Slave %1 - Valve %2: Timeout error.").arg(slave3_test).arg(valve * 8 + valve_bit + 1));
            }
            while(1)
            {
                Read_Valve();
                time_elapsed = Timer_count.nsecsElapsed();
                timeElapsed = time_elapsed/1e6;
                qDebug() << QString::number(timeElapsed,'f',2);
                switch (valve)
                {
                case 0:
                    invalve_1 = response_time_io_1->valve_readback_1to8;
                    outvalve_1 = response_time_io_1->valve_control_1to8;
                    inbit_1 = invalve_1 & (1 << bit_change);
                    outbit_1 = outvalve_1 & (1 << bit_change);

                    invalve_2 = response_time_io_2->valve_readback_1to8;
                    outvalve_2 = response_time_io_2->valve_control_1to8;
                    inbit_2 = invalve_2 & (1 << bit_change);
                    outbit_2 = outvalve_2 & (1 << bit_change);

                    invalve_3 = response_time_io_3->valve_readback_1to8;
                    outvalve_3 = response_time_io_3->valve_control_1to8;
                    inbit_3 = invalve_3 & (1 << bit_change);
                    outbit_3 = outvalve_3 & (1 << bit_change);
                    break;

                case 1:
                    invalve_1 = response_time_io_1->valve_readback_9to16;
                    outvalve_1 = response_time_io_1->valve_control_9to16;
                    inbit_1 = invalve_1 & (1 << bit_change);
                    outbit_1 = outvalve_1 & (1 << bit_change);

                    invalve_2 = response_time_io_2->valve_readback_9to16;
                    outvalve_2 = response_time_io_2->valve_control_9to16;
                    inbit_2 = invalve_2 & (1 << bit_change);
                    outbit_2 = outvalve_2 & (1 << bit_change);

                    invalve_3 = response_time_io_3->valve_readback_9to16;
                    outvalve_3 = response_time_io_3->valve_control_9to16;
                    inbit_3 = invalve_3 & (1 << bit_change);
                    outbit_3 = outvalve_3 & (1 << bit_change);
                    break;

                case 2:
                    invalve_1 = response_time_io_1->valve_readback_17to24;
                    outvalve_1 = response_time_io_1->valve_control_17to24;
                    inbit_1 = invalve_1 & (1 << bit_change);
                    outbit_1 = outvalve_1 & (1 << bit_change);

                    invalve_2 = response_time_io_2->valve_readback_17to24;
                    outvalve_2 = response_time_io_2->valve_control_17to24;
                    inbit_2 = invalve_2 & (1 << bit_change);
                    outbit_2 = outvalve_2 & (1 << bit_change);

                    invalve_3 = response_time_io_3->valve_readback_17to24;
                    outvalve_3 = response_time_io_3->valve_control_17to24;
                    inbit_3 = invalve_3 & (1 << bit_change);
                    outbit_3 = outvalve_3 & (1 << bit_change);
                    break;

                case 3:
                    invalve_1 = response_time_io_1->valve_readback_25to32;
                    outvalve_1 = response_time_io_1->valve_control_25to32;
                    inbit_1 = invalve_1 & (1 << bit_change);
                    outbit_1 = outvalve_1 & (1 << bit_change);

                    invalve_2 = response_time_io_2->valve_readback_25to32;
                    outvalve_2 = response_time_io_2->valve_control_25to32;
                    inbit_2 = invalve_2 & (1 << bit_change);
                    outbit_2 = outvalve_2 & (1 << bit_change);

                    invalve_3 = response_time_io_3->valve_readback_25to32;
                    outvalve_3 = response_time_io_3->valve_control_25to32;
                    inbit_3 = invalve_3 & (1 << bit_change);
                    outbit_3 = outvalve_3 & (1 << bit_change);
                    break;
                case 4:
                    invalve_1 = response_time_io_1->valve_readback_33to40;
                    outvalve_1 = response_time_io_1->valve_control_33to40;
                    inbit_1 = invalve_1 & (1 << bit_change);
                    outbit_1 = outvalve_1 & (1 << bit_change);

                    invalve_2 = response_time_io_2->valve_readback_33to40;
                    outvalve_2 = response_time_io_2->valve_control_33to40;
                    inbit_2 = invalve_2 & (1 << bit_change);
                    outbit_2 = outvalve_2 & (1 << bit_change);

                    invalve_3 = response_time_io_3->valve_readback_33to40;
                    outvalve_3 = response_time_io_3->valve_control_33to40;
                    inbit_3 = invalve_3 & (1 << bit_change);
                    outbit_3 = outvalve_3 & (1 << bit_change);
                    break;
                case 5:
                    invalve_1 = response_time_io_1->valve_readback_41to48;
                    outvalve_1 = response_time_io_1->valve_control_41to48;
                    inbit_1 = invalve_1 & (1 << bit_change);
                    outbit_1 = outvalve_1 & (1 << bit_change);

                    invalve_2 = response_time_io_2->valve_readback_41to48;
                    outvalve_2 = response_time_io_2->valve_control_41to48;
                    inbit_2 = invalve_2 & (1 << bit_change);
                    outbit_2 = outvalve_2 & (1 << bit_change);

                    invalve_3 = response_time_io_3->valve_readback_41to48;
                    outvalve_3 = response_time_io_3->valve_control_41to48;
                    inbit_3 = invalve_3 & (1 << bit_change);
                    outbit_3 = outvalve_3 & (1 << bit_change);
                    break;
                }

                if(inbit_1 == outbit_1 && complete_test_1 == false)
                {
                    complete_test_1 = true;
                    if(Valve_State == VALVE_ON)
                    {
                        logStatus(QString("Slave %1 - Valve %2: Response ON time %3ms").arg(slave1_test).arg(valve * 8 + valve_bit + 1).arg(QString::number(timeElapsed,'f',2)));
                    }
                    else
                    {
                        logStatus(QString("Slave %1 - Valve %2: Response OFF time %3ms").arg(slave1_test).arg(valve * 8 + valve_bit + 1).arg(QString::number(timeElapsed,'f',2)));
                    }
                }
                if(inbit_3 == outbit_3 && complete_test_3 == false)
                {
                    complete_test_3 = true;
                    if(Valve_State == VALVE_ON)
                    {
                        logStatus(QString("Slave %1 - Valve %2: Response ON time %3ms").arg(slave3_test).arg(valve * 8 + valve_bit + 1).arg(QString::number(timeElapsed,'f',2)));
                    }
                    else
                    {
                        logStatus(QString("Slave %1 - Valve %2: Response OFF time %3ms").arg(slave3_test).arg(valve * 8 + valve_bit + 1).arg(QString::number(timeElapsed,'f',2)));
                    }
                }
                if(inbit_2 == outbit_2 && complete_test_2 == false)
                {
                    complete_test_2 = true;
                    if(Valve_State == VALVE_ON)
                    {
                        logStatus(QString("Slave %1 - Valve %2: Response ON time %3ms").arg(slave2_test).arg(valve * 8 + valve_bit + 1).arg(QString::number(timeElapsed,'f',2)));
                    }
                    else
                    {
                        logStatus(QString("Slave %1 - Valve %2: Response OFF time %3ms").arg(slave2_test).arg(valve * 8 + valve_bit + 1).arg(QString::number(timeElapsed,'f',2)));
                    }
                }
                if(complete_test_1 == true && complete_test_2 == true && complete_test_3 == true || timeElapsed > 2000)
                {
                    return;
                }
            }
        }
        switch (valve)
        {
        case 0:
            invalve_1 = response_time_io_1->valve_readback_1to8;
            outvalve_1 = response_time_io_1->valve_control_1to8;
            inbit_1 = invalve_1 & (1 << bit_change);
            outbit_1 = outvalve_1 & (1 << bit_change);

            invalve_2 = response_time_io_2->valve_readback_1to8;
            outvalve_2 = response_time_io_2->valve_control_1to8;
            inbit_2 = invalve_2 & (1 << bit_change);
            outbit_2 = outvalve_2 & (1 << bit_change);

            invalve_3 = response_time_io_3->valve_readback_1to8;
            outvalve_3 = response_time_io_3->valve_control_1to8;
            inbit_3 = invalve_3 & (1 << bit_change);
            outbit_3 = outvalve_3 & (1 << bit_change);
            break;

        case 1:
            invalve_1 = response_time_io_1->valve_readback_9to16;
            outvalve_1 = response_time_io_1->valve_control_9to16;
            inbit_1 = invalve_1 & (1 << bit_change);
            outbit_1 = outvalve_1 & (1 << bit_change);

            invalve_2 = response_time_io_2->valve_readback_9to16;
            outvalve_2 = response_time_io_2->valve_control_9to16;
            inbit_2 = invalve_2 & (1 << bit_change);
            outbit_2 = outvalve_2 & (1 << bit_change);

            invalve_3 = response_time_io_3->valve_readback_9to16;
            outvalve_3 = response_time_io_3->valve_control_9to16;
            inbit_3 = invalve_3 & (1 << bit_change);
            outbit_3 = outvalve_3 & (1 << bit_change);
            break;

        case 2:
            invalve_1 = response_time_io_1->valve_readback_17to24;
            outvalve_1 = response_time_io_1->valve_control_17to24;
            inbit_1 = invalve_1 & (1 << bit_change);
            outbit_1 = outvalve_1 & (1 << bit_change);

            invalve_2 = response_time_io_2->valve_readback_17to24;
            outvalve_2 = response_time_io_2->valve_control_17to24;
            inbit_2 = invalve_2 & (1 << bit_change);
            outbit_2 = outvalve_2 & (1 << bit_change);

            invalve_3 = response_time_io_3->valve_readback_17to24;
            outvalve_3 = response_time_io_3->valve_control_17to24;
            inbit_3 = invalve_3 & (1 << bit_change);
            outbit_3 = outvalve_3 & (1 << bit_change);
            break;

        case 3:
            invalve_1 = response_time_io_1->valve_readback_25to32;
            outvalve_1 = response_time_io_1->valve_control_25to32;
            inbit_1 = invalve_1 & (1 << bit_change);
            outbit_1 = outvalve_1 & (1 << bit_change);

            invalve_2 = response_time_io_2->valve_readback_25to32;
            outvalve_2 = response_time_io_2->valve_control_25to32;
            inbit_2 = invalve_2 & (1 << bit_change);
            outbit_2 = outvalve_2 & (1 << bit_change);

            invalve_3 = response_time_io_3->valve_readback_25to32;
            outvalve_3 = response_time_io_3->valve_control_25to32;
            inbit_3 = invalve_3 & (1 << bit_change);
            outbit_3 = outvalve_3 & (1 << bit_change);
            break;
        case 4:
            invalve_1 = response_time_io_1->valve_readback_33to40;
            outvalve_1 = response_time_io_1->valve_control_33to40;
            inbit_1 = invalve_1 & (1 << bit_change);
            outbit_1 = outvalve_1 & (1 << bit_change);

            invalve_2 = response_time_io_2->valve_readback_33to40;
            outvalve_2 = response_time_io_2->valve_control_33to40;
            inbit_2 = invalve_2 & (1 << bit_change);
            outbit_2 = outvalve_2 & (1 << bit_change);

            invalve_3 = response_time_io_3->valve_readback_33to40;
            outvalve_3 = response_time_io_3->valve_control_33to40;
            inbit_3 = invalve_3 & (1 << bit_change);
            outbit_3 = outvalve_3 & (1 << bit_change);
            break;
        case 5:
            invalve_1 = response_time_io_1->valve_readback_41to48;
            outvalve_1 = response_time_io_1->valve_control_41to48;
            inbit_1 = invalve_1 & (1 << bit_change);
            outbit_1 = outvalve_1 & (1 << bit_change);

            invalve_2 = response_time_io_2->valve_readback_41to48;
            outvalve_2 = response_time_io_2->valve_control_41to48;
            inbit_2 = invalve_2 & (1 << bit_change);
            outbit_2 = outvalve_2 & (1 << bit_change);

            invalve_3 = response_time_io_3->valve_readback_41to48;
            outvalve_3 = response_time_io_3->valve_control_41to48;
            inbit_3 = invalve_3 & (1 << bit_change);
            outbit_3 = outvalve_3 & (1 << bit_change);
            break;
        }

        if(inbit_1 == outbit_1 && complete_test_1 == false)
        {
            complete_test_1 = true;
            if(Valve_State == VALVE_ON)
            {
                ui->Response_On_Time_TableWidget_1->item(valve, 7 - valve_bit)->setText(QString::number(timeElapsed,'f',1));
                if(Min_Response_OnTime_1 == 0)
                {
                    Min_Response_OnTime_1 = timeElapsed;
                    Max_Response_OnTime_1 = timeElapsed;
                }
                else
                {
                    if(Min_Response_OnTime_1 > timeElapsed)
                    {
                        Min_Response_OnTime_1 = timeElapsed;
                    }
                    if(Max_Response_OnTime_1 < timeElapsed)
                    {
                        Max_Response_OnTime_1 = timeElapsed;
                    }
                }
            }
            else
            {
                ui->Response_Off_Time_TableWidget_1->item(valve, 7 - valve_bit)->setText(QString::number(timeElapsed,'f',1));
                if(Min_Response_OffTime_1 == 0)
                {
                    Min_Response_OffTime_1 = timeElapsed;
                    Max_Response_OffTime_1 = timeElapsed;
                }
                else
                {
                    if(Min_Response_OffTime_1 > timeElapsed)
                    {
                        Min_Response_OffTime_1 = timeElapsed;
                    }
                    if(Max_Response_OffTime_1 < timeElapsed)
                    {
                        Max_Response_OffTime_1 = timeElapsed;
                    }
                }
            }
        }
        if(inbit_3 == outbit_3 && complete_test_3 == false)
        {
            qDebug()<< QString("In_Bit_3: %1, Out_Bit_3: %2").arg(inbit_3).arg(outbit_3);
            complete_test_3 = true;
            if(Valve_State == VALVE_ON)
            {
                ui->Response_On_Time_TableWidget_3->item(valve, 7 - valve_bit)->setText(QString::number(timeElapsed,'f',1));
                if(Min_Response_OnTime_3 == 0)
                {
                    Min_Response_OnTime_3 = timeElapsed;
                    Max_Response_OnTime_3 = timeElapsed;
                }
                else
                {
                    if(Min_Response_OnTime_3 > timeElapsed)
                    {
                        Min_Response_OnTime_3 = timeElapsed;
                    }
                    if(Max_Response_OnTime_3 < timeElapsed)
                    {
                        Max_Response_OnTime_3 = timeElapsed;
                    }
                }
            }
            else
            {
                ui->Response_Off_Time_TableWidget_3->item(valve, 7 - valve_bit)->setText(QString::number(timeElapsed,'f',1));
                if(Min_Response_OffTime_3 == 0)
                {
                    Min_Response_OffTime_3 = timeElapsed;
                    Max_Response_OffTime_3 = timeElapsed;
                }
                else
                {
                    if(Min_Response_OffTime_3 > timeElapsed)
                    {
                        Min_Response_OffTime_3 = timeElapsed;
                    }
                    if(Max_Response_OffTime_3 < timeElapsed)
                    {
                        Max_Response_OffTime_3 = timeElapsed;
                    }
                }
            }
        }
        if(inbit_2 == outbit_2 && complete_test_2 == false)
        {
            qDebug()<< QString("In_Bit_2: %1, Out_Bit_2: %2").arg(inbit_2).arg(outbit_2);
            complete_test_2 = true;
            if(Valve_State == VALVE_ON)
            {
                ui->Response_On_Time_TableWidget_2->item(valve, 7 - valve_bit)->setText(QString::number(timeElapsed,'f',1));
                if(Min_Response_OnTime_2 == 0)
                {
                    Min_Response_OnTime_2 = timeElapsed;
                    Max_Response_OnTime_2 = timeElapsed;
                }
                else
                {
                    if(Min_Response_OnTime_2 > timeElapsed)
                    {
                        Min_Response_OnTime_2 = timeElapsed;
                    }
                    if(Max_Response_OnTime_2 < timeElapsed)
                    {
                        Max_Response_OnTime_2 = timeElapsed;
                    }
                }
            }
            else
            {
                ui->Response_Off_Time_TableWidget_2->item(valve, 7 - valve_bit)->setText(QString::number(timeElapsed,'f',1));
                if(Min_Response_OffTime_2 == 0)
                {
                    Min_Response_OffTime_2 = timeElapsed;
                    Max_Response_OffTime_2 = timeElapsed;
                }
                else
                {
                    if(Min_Response_OffTime_2 > timeElapsed)
                    {
                        Min_Response_OffTime_2 = timeElapsed;
                    }
                    if(Max_Response_OffTime_2 < timeElapsed)
                    {
                        Max_Response_OffTime_2 = timeElapsed;
                    }
                }
            }
        }
        if(complete_test_1 == true && complete_test_2 == true && complete_test_3 == true)
        {
            return;
        }
    }
}

void response_time_testing_42ev::check_time_response_random(int numValveRandom)
{
    QElapsedTimer Timer_count;
    qint64 time_elapsed;
    int invalve_1[42] = {0}, outvalve_1[42]= {0};
    int invalve_2[42]= {0}, outvalve_2[42]= {0};
    int invalve_3[42]= {0}, outvalve_3[42]= {0};
    int inbit_1[42]= {0}, outbit_1[42]= {0};
    int inbit_2[42]= {0}, outbit_2[42]= {0};
    int inbit_3[42]= {0}, outbit_3[42]= {0};
    bool complete_test_bit_1[42] = {false};
    bool complete_test_bit_2[42] = {false};
    bool complete_test_bit_3[42] = {false};
    bool complete_test_1 = false;
    bool complete_test_2 = false;
    bool complete_test_3 = false;
    bool check_data_change_1 = false;
    bool check_data_change_2 = false;
    bool check_data_change_3 = false;
    if(slave1_test > slave_count)
    {
        complete_test_1 = true;
        memset(complete_test_bit_1,true,sizeof(complete_test_bit_1));
    }
    if(slave2_test > slave_count)
    {
        complete_test_2 = true;
        memset(complete_test_bit_2,true,sizeof(complete_test_bit_2));

    }
    if(slave3_test > slave_count)
    {
        complete_test_3 = true;
        memset(complete_test_bit_3,true,sizeof(complete_test_bit_3));
    }
    Timer_count.start();
    while(Is_Start_Test == true)
    {
        Read_Valve();
        display_ValveReadback();
        display_ValveControl();
        time_elapsed = Timer_count.nsecsElapsed();
        double timeElapsed = time_elapsed/1e6;
        if(timeElapsed > max_response_time)
        {
            Is_Test_Fail = true;
            display_ValveReadback();
            display_ValveControl();
            if(complete_test_1 == false)
            {
                for(int i = 0; i < numValveRandom; i++)
                {
                    int valve = valve_random[i]/8;
                    int valve_bit = valve_random[i]%8;
                    if(complete_test_bit_1[i] == false)
                    {
                        if(Valve_State == VALVE_ON)
                        {
                            ui->Response_On_Time_TableWidget_1->item(valve,7 - valve_bit)->setText("ERR");
                        }
                        else
                        {
                            ui->Response_Off_Time_TableWidget_1->item(valve,7 - valve_bit)->setText("ERR");
                        }
                        logStatus(QString("Slave %1 - Valve %2: Timeout error.").arg(slave1_test).arg(valve * 8 + valve_bit + 1));
                    }
                }
            }
            if(complete_test_2 == false)
            {
                for(int i = 0; i < numValveRandom; i++)
                {
                    int valve = valve_random[i]/8;
                    int valve_bit = valve_random[i]%8;
                    if(complete_test_bit_2[i] == false)
                    {
                        if(Valve_State == VALVE_ON)
                        {
                            ui->Response_On_Time_TableWidget_2->item(valve,7 - valve_bit)->setText("ERR");
                        }
                        else
                        {
                            ui->Response_Off_Time_TableWidget_2->item(valve,7 - valve_bit)->setText("ERR");
                        }
                        logStatus(QString("Slave %1 - Valve %2: Timeout error.").arg(slave2_test).arg(valve * 8 + valve_bit + 1));
                    }
                }
            }
            if(complete_test_3 == false)
            {
                for(int i = 0; i < numValveRandom; i++)
                {
                    int valve = valve_random[i]/8;
                    int valve_bit = valve_random[i]%8;
                    if(complete_test_bit_3[i] == false)
                    {
                        if(Valve_State == VALVE_ON)
                        {
                            ui->Response_On_Time_TableWidget_3->item(valve,7 - valve_bit)->setText("ERR");
                        }
                        else
                        {
                            ui->Response_Off_Time_TableWidget_3->item(valve,7 - valve_bit)->setText("ERR");
                        }
                        logStatus(QString("Slave %1 - Valve %2: Timeout error.").arg(slave3_test).arg(valve * 8 + valve_bit + 1));
                    }
                }
            }
            while(1)
            {
                Read_Valve();
                time_elapsed = Timer_count.nsecsElapsed();
                timeElapsed = time_elapsed/1e6;
                for(int i = 0; i < numValveRandom; i++)
                {
                    int valve = valve_random[i] / 8;
                    int valve_bit = valve_random[i] %8;
                    switch(valve)
                    {
                    case 0:
                        invalve_1[i] = response_time_io_1->valve_readback_1to8;
                        outvalve_1[i] = response_time_io_1->valve_control_1to8;
                        inbit_1[i] = invalve_1[i] & (1 << valve_bit);
                        outbit_1[i] = outvalve_1[i] & (1 << valve_bit);

                        invalve_2[i] = response_time_io_2->valve_readback_1to8;
                        outvalve_2[i] = response_time_io_2->valve_control_1to8;
                        inbit_2[i] = invalve_2[i] & (1 << valve_bit);
                        outbit_2[i] = outvalve_2[i] & (1 << valve_bit);

                        invalve_3[i] = response_time_io_3->valve_readback_1to8;
                        outvalve_3[i] = response_time_io_3->valve_control_1to8;
                        inbit_3[i] = invalve_3[i] & (1 << valve_bit);
                        outbit_3[i] = outvalve_3[i] & (1 << valve_bit);
                        break;
                    case 1:
                        invalve_1[i] = response_time_io_1->valve_readback_9to16;
                        outvalve_1[i] = response_time_io_1->valve_control_9to16;
                        inbit_1[i] = invalve_1[i] & (1 << valve_bit);
                        outbit_1[i] = outvalve_1[i] & (1 << valve_bit);

                        invalve_2[i] = response_time_io_2->valve_readback_9to16;
                        outvalve_2[i] = response_time_io_2->valve_control_9to16;
                        inbit_2[i] = invalve_2[i] & (1 << valve_bit);
                        outbit_2[i] = outvalve_2[i] & (1 << valve_bit);

                        invalve_3[i] = response_time_io_3->valve_readback_9to16;
                        outvalve_3[i] = response_time_io_3->valve_control_9to16;
                        inbit_3[i] = invalve_3[i] & (1 << valve_bit);
                        outbit_3[i] = outvalve_3[i] & (1 << valve_bit);
                        break;

                    case 2:
                        invalve_1[i] = response_time_io_1->valve_readback_17to24;
                        outvalve_1[i] = response_time_io_1->valve_control_17to24;
                        inbit_1[i] = invalve_1[i] & (1 << valve_bit);
                        outbit_1[i] = outvalve_1[i] & (1 << valve_bit);

                        invalve_2[i] = response_time_io_2->valve_readback_17to24;
                        outvalve_2[i] = response_time_io_2->valve_control_17to24;
                        inbit_2[i] = invalve_2[i] & (1 << valve_bit);
                        outbit_2[i] = outvalve_2[i] & (1 << valve_bit);

                        invalve_3[i] = response_time_io_3->valve_readback_17to24;
                        outvalve_3[i] = response_time_io_3->valve_control_17to24;
                        inbit_3[i] = invalve_3[i] & (1 << valve_bit);
                        outbit_3[i] = outvalve_3[i] & (1 << valve_bit);
                        break;

                    case 3:
                        invalve_1[i] = response_time_io_1->valve_readback_25to32;
                        outvalve_1[i] = response_time_io_1->valve_control_25to32;
                        inbit_1[i] = invalve_1[i] & (1 << valve_bit);
                        outbit_1[i] = outvalve_1[i] & (1 << valve_bit);

                        invalve_2[i] = response_time_io_2->valve_readback_25to32;
                        outvalve_2[i] = response_time_io_2->valve_control_25to32;
                        inbit_2[i] = invalve_2[i] & (1 << valve_bit);
                        outbit_2[i] = outvalve_2[i] & (1 << valve_bit);

                        invalve_3[i] = response_time_io_3->valve_readback_25to32;
                        outvalve_3[i] = response_time_io_3->valve_control_25to32;
                        inbit_3[i] = invalve_3[i] & (1 << valve_bit);
                        outbit_3[i] = outvalve_3[i] & (1 << valve_bit);
                        break;

                    case 4:
                        invalve_1[i] = response_time_io_1->valve_readback_33to40;
                        outvalve_1[i] = response_time_io_1->valve_control_33to40;
                        inbit_1[i] = invalve_1[i] & (1 << valve_bit);
                        outbit_1[i] = outvalve_1[i] & (1 << valve_bit);

                        invalve_2[i] = response_time_io_2->valve_readback_33to40;
                        outvalve_2[i] = response_time_io_2->valve_control_33to40;
                        inbit_2[i] = invalve_2[i] & (1 << valve_bit);
                        outbit_2[i] = outvalve_2[i] & (1 << valve_bit);

                        invalve_3[i] = response_time_io_3->valve_readback_33to40;
                        outvalve_3[i] = response_time_io_3->valve_control_33to40;
                        inbit_3[i] = invalve_3[i] & (1 << valve_bit);
                        outbit_3[i] = outvalve_3[i] & (1 << valve_bit);
                        break;
                    case 5:
                        invalve_1[i] = response_time_io_1->valve_readback_41to48;
                        outvalve_1[i] = response_time_io_1->valve_control_41to48;
                        inbit_1[i] = invalve_1[i] & (1 << valve_bit);
                        outbit_1[i] = outvalve_1[i] & (1 << valve_bit);

                        invalve_2[i] = response_time_io_2->valve_readback_41to48;
                        outvalve_2[i] = response_time_io_2->valve_control_41to48;
                        inbit_2[i] = invalve_2[i] & (1 << valve_bit);
                        outbit_2[i] = outvalve_2[i] & (1 << valve_bit);

                        invalve_3[i] = response_time_io_3->valve_readback_41to48;
                        outvalve_3[i] = response_time_io_3->valve_control_41to48;
                        inbit_3[i] = invalve_3[i] & (1 << valve_bit);
                        outbit_3[i] = outvalve_3[i] & (1 << valve_bit);
                        break;
                    }
                }
                for(int i = 0; i < numValveRandom; i++)
                {
                    int valve = valve_random[i] / 8;
                    int valve_bit = valve_random[i] %8;
                    if(inbit_1[i] == outbit_1[i] && complete_test_bit_1[i] == false)
                    {
                        time_elapsed = Timer_count.nsecsElapsed();
                        timeElapsed = time_elapsed/1e6;
                        complete_test_bit_1[i] = true;
                        if(Valve_State == VALVE_ON)
                        {
                            logStatus(QString("Slave %1 - Valve %2: Response ON time: %3ms").arg(slave1_test).arg(valve * 8 + valve_bit +1).arg(QString::number(timeElapsed,'f',1)));
                        }
                        else
                        {
                            logStatus(QString("Slave %1 - Valve %2: Response OFF time: %3ms").arg(slave1_test).arg(valve * 8 + valve_bit +1).arg(QString::number(timeElapsed,'f',1)));
                        }
                    }
                    if(inbit_2[i] == outbit_2[i] && complete_test_bit_2[i] == false)
                    {
                        time_elapsed = Timer_count.nsecsElapsed();
                        timeElapsed = time_elapsed/1e6;
                        complete_test_bit_2[i] = true;
                        if(Valve_State == VALVE_ON)
                        {
                            logStatus(QString("Slave %1 - Valve %2: Response ON time: %3ms").arg(slave2_test).arg(valve * 8 + valve_bit +1).arg(QString::number(timeElapsed,'f',1)));
                        }
                        else
                        {
                            logStatus(QString("Slave %1 - Valve %2: Response OFF time: %3ms").arg(slave2_test).arg(valve * 8 + valve_bit +1).arg(QString::number(timeElapsed,'f',1)));
                        }
                    }
                    if(inbit_3[i] == outbit_3[i] && complete_test_bit_3[i] == false)
                    {
                        time_elapsed = Timer_count.nsecsElapsed();
                        timeElapsed = time_elapsed/1e6;
                        complete_test_bit_3[i] = true;
                        if(Valve_State == VALVE_ON)
                        {
                            logStatus(QString("Slave %1 - Valve %2: Response ON time: %3ms").arg(slave3_test).arg(valve * 8 + valve_bit +1).arg(QString::number(timeElapsed,'f',1)));
                        }
                        else
                        {
                            logStatus(QString("Slave %1 - Valve %2: Response OFF time: %3ms").arg(slave3_test).arg(valve * 8 + valve_bit +1).arg(QString::number(timeElapsed,'f',1)));
                        }
                    }
                }

                complete_test_1 = true;
                complete_test_2 = true;
                complete_test_3 = true;
                for(int i = 0; i < numValveRandom; i++)
                {
                    if(complete_test_bit_1[i] == false)
                    {
                        complete_test_1 = false;
                    }
                    if(complete_test_bit_2[i] == false)
                    {
                        complete_test_2 = false;
                    }
                    if(complete_test_bit_3[i] == false)
                    {
                        complete_test_3 = false;
                    }
                }

                if(complete_test_1 == true && complete_test_2 == true && complete_test_3 == true)
                {
                    return;
                }
            }
        }

        for(int i = 0; i < numValveRandom; i++)
        {
            int valve = valve_random[i] / 8;
            int valve_bit = valve_random[i] %8;
            switch(valve)
            {
            case 0:
                invalve_1[i] = response_time_io_1->valve_readback_1to8;
                outvalve_1[i] = response_time_io_1->valve_control_1to8;
                inbit_1[i] = invalve_1[i] & (1 << valve_bit);
                outbit_1[i] = outvalve_1[i] & (1 << valve_bit);

                invalve_2[i] = response_time_io_2->valve_readback_1to8;
                outvalve_2[i] = response_time_io_2->valve_control_1to8;
                inbit_2[i] = invalve_2[i] & (1 << valve_bit);
                outbit_2[i] = outvalve_2[i] & (1 << valve_bit);

                invalve_3[i] = response_time_io_3->valve_readback_1to8;
                outvalve_3[i] = response_time_io_3->valve_control_1to8;
                inbit_3[i] = invalve_3[i] & (1 << valve_bit);
                outbit_3[i] = outvalve_3[i] & (1 << valve_bit);
                break;
            case 1:
                invalve_1[i] = response_time_io_1->valve_readback_9to16;
                outvalve_1[i] = response_time_io_1->valve_control_9to16;
                inbit_1[i] = invalve_1[i] & (1 << valve_bit);
                outbit_1[i] = outvalve_1[i] & (1 << valve_bit);

                invalve_2[i] = response_time_io_2->valve_readback_9to16;
                outvalve_2[i] = response_time_io_2->valve_control_9to16;
                inbit_2[i] = invalve_2[i] & (1 << valve_bit);
                outbit_2[i] = outvalve_2[i] & (1 << valve_bit);

                invalve_3[i] = response_time_io_3->valve_readback_9to16;
                outvalve_3[i] = response_time_io_3->valve_control_9to16;
                inbit_3[i] = invalve_3[i] & (1 << valve_bit);
                outbit_3[i] = outvalve_3[i] & (1 << valve_bit);
                break;

            case 2:
                invalve_1[i] = response_time_io_1->valve_readback_17to24;
                outvalve_1[i] = response_time_io_1->valve_control_17to24;
                inbit_1[i] = invalve_1[i] & (1 << valve_bit);
                outbit_1[i] = outvalve_1[i] & (1 << valve_bit);

                invalve_2[i] = response_time_io_2->valve_readback_17to24;
                outvalve_2[i] = response_time_io_2->valve_control_17to24;
                inbit_2[i] = invalve_2[i] & (1 << valve_bit);
                outbit_2[i] = outvalve_2[i] & (1 << valve_bit);

                invalve_3[i] = response_time_io_3->valve_readback_17to24;
                outvalve_3[i] = response_time_io_3->valve_control_17to24;
                inbit_3[i] = invalve_3[i] & (1 << valve_bit);
                outbit_3[i] = outvalve_3[i] & (1 << valve_bit);
                break;

            case 3:
                invalve_1[i] = response_time_io_1->valve_readback_25to32;
                outvalve_1[i] = response_time_io_1->valve_control_25to32;
                inbit_1[i] = invalve_1[i] & (1 << valve_bit);
                outbit_1[i] = outvalve_1[i] & (1 << valve_bit);

                invalve_2[i] = response_time_io_2->valve_readback_25to32;
                outvalve_2[i] = response_time_io_2->valve_control_25to32;
                inbit_2[i] = invalve_2[i] & (1 << valve_bit);
                outbit_2[i] = outvalve_2[i] & (1 << valve_bit);

                invalve_3[i] = response_time_io_3->valve_readback_25to32;
                outvalve_3[i] = response_time_io_3->valve_control_25to32;
                inbit_3[i] = invalve_3[i] & (1 << valve_bit);
                outbit_3[i] = outvalve_3[i] & (1 << valve_bit);
                break;
            case 4:
                invalve_1[i] = response_time_io_1->valve_readback_33to40;
                outvalve_1[i] = response_time_io_1->valve_control_33to40;
                inbit_1[i] = invalve_1[i] & (1 << valve_bit);
                outbit_1[i] = outvalve_1[i] & (1 << valve_bit);

                invalve_2[i] = response_time_io_2->valve_readback_33to40;
                outvalve_2[i] = response_time_io_2->valve_control_33to40;
                inbit_2[i] = invalve_2[i] & (1 << valve_bit);
                outbit_2[i] = outvalve_2[i] & (1 << valve_bit);

                invalve_3[i] = response_time_io_3->valve_readback_33to40;
                outvalve_3[i] = response_time_io_3->valve_control_33to40;
                inbit_3[i] = invalve_3[i] & (1 << valve_bit);
                outbit_3[i] = outvalve_3[i] & (1 << valve_bit);
                break;
            case 5:
                invalve_1[i] = response_time_io_1->valve_readback_41to48;
                outvalve_1[i] = response_time_io_1->valve_control_41to48;
                inbit_1[i] = invalve_1[i] & (1 << valve_bit);
                outbit_1[i] = outvalve_1[i] & (1 << valve_bit);

                invalve_2[i] = response_time_io_2->valve_readback_41to48;
                outvalve_2[i] = response_time_io_2->valve_control_41to48;
                inbit_2[i] = invalve_2[i] & (1 << valve_bit);
                outbit_2[i] = outvalve_2[i] & (1 << valve_bit);

                invalve_3[i] = response_time_io_3->valve_readback_41to48;
                outvalve_3[i] = response_time_io_3->valve_control_41to48;
                inbit_3[i] = invalve_3[i] & (1 << valve_bit);
                outbit_3[i] = outvalve_3[i] & (1 << valve_bit);
                break;
            }
        }
        for(int i = 0; i < numValveRandom; i++)
        {
            int valve = valve_random[i] / 8;
            int valve_bit = valve_random[i] %8;
            if(inbit_1[i] == outbit_1[i] && complete_test_bit_1[i] == false)
            {
                time_elapsed = Timer_count.nsecsElapsed();
                timeElapsed = time_elapsed/1e6;
                complete_test_bit_1[i] = true;
                if(Valve_State == VALVE_ON)
                {
                    ui->Response_On_Time_TableWidget_1->item(valve, 7 - valve_bit)->setText(QString::number(timeElapsed,'f',1));
                    ui->Response_On_Time_TableWidget_1->item(valve, 7 - valve_bit)->setBackground(QColor(213,238,255));
                }
                else
                {
                    ui->Response_Off_Time_TableWidget_1->item(valve, 7 - valve_bit)->setText(QString::number(timeElapsed,'f',1));
                    ui->Response_Off_Time_TableWidget_1->item(valve, 7 - valve_bit)->setBackground(QColor(213,238,255));
                }
            }
            if(inbit_2[i] == outbit_2[i] && complete_test_bit_2[i] == false)
            {
                time_elapsed = Timer_count.nsecsElapsed();
                timeElapsed = time_elapsed/1e6;
                complete_test_bit_2[i] = true;
                if(Valve_State == VALVE_ON)
                {
                    ui->Response_On_Time_TableWidget_2->item(valve, 7 - valve_bit)->setText(QString::number(timeElapsed,'f',1));
                }
                else
                {
                    ui->Response_Off_Time_TableWidget_2->item(valve, 7 - valve_bit)->setText(QString::number(timeElapsed,'f',1));
                }
            }
            if(inbit_3[i] == outbit_3[i] && complete_test_bit_3[i] == false)
            {
                time_elapsed = Timer_count.nsecsElapsed();
                timeElapsed = time_elapsed/1e6;
                complete_test_bit_3[i] = true;
                if(Valve_State == VALVE_ON)
                {
                    ui->Response_On_Time_TableWidget_3->item(valve, 7 - valve_bit)->setText(QString::number(timeElapsed,'f',1));
                }
                else
                {
                    ui->Response_Off_Time_TableWidget_3->item(valve, 7 - valve_bit)->setText(QString::number(timeElapsed,'f',1));
                }
            }
        }

        complete_test_1 = true;
        complete_test_2 = true;
        complete_test_3 = true;
        for(int i = 0; i < numValveRandom; i++)
        {
            if(complete_test_bit_1[i] == false)
            {
                complete_test_1 = false;
            }
            if(complete_test_bit_2[i] == false)
            {
                complete_test_2 = false;
            }
            if(complete_test_bit_3[i] == false)
            {
                complete_test_3 = false;
            }
        }

        if(complete_test_1 == true && slave1_test <= slave_count)
        {
            if(check_data_change_1 == false)
            {
                check_data_change_1 = true;
                Is_checking_ValveChange = true;
                std::thread check_ValveChange([&,numValveRandom]()
                                              {
                                                  Complete_Stop_Check_1 = false;
                                                  int Valve_count_change[42] = {0};
                                                  int invalve[42] = {0}, outvalve[42]= {0};
                                                  int inbit[42]= {0}, outbit[42]= {0};
                                                  bool Is_change[42] = {false};
                                                  auto next = std::chrono::steady_clock::now();
                                                  while(Is_checking_ValveChange && Is_Start_Test)
                                                  {
                                                      next+= std::chrono::microseconds(100);

                                                      processData_mutex.lock();


                                                      for(int i = 0; i <numValveRandom;i++)
                                                      {
                                                          int valve = valve_random[i] / 8;
                                                          int valve_bit = valve_random[i] % 8;

                                                          invalve[i] = IOmap[PDO_ValveReadback_Address_1 + valve];
                                                          outvalve[i] = IOmap[PDO_ValveControl_Address_1 + valve];
                                                          inbit[i] = invalve[i] & (1 << valve_bit);
                                                          outbit[i] = outvalve[i] & (1 << valve_bit);

                                                          if(inbit[i] != outbit[i])
                                                          {
                                                              if(Is_change[i] == false)
                                                              {
                                                                  Valve_count_change[i]++;
                                                              }
                                                              Is_change[i] = true;
                                                          }
                                                          else
                                                          {
                                                              Is_change[i] = false;
                                                          }
                                                      }
                                                      processData_mutex.unlock();
                                                      std::this_thread::sleep_until(next);
                                                  }

                                                  for(int i = 0; i < numValveRandom;i++)
                                                  {
                                                      int valve = valve_random[i] / 8;
                                                      int valve_bit = valve_random[i] % 8;
                                                      if(Valve_count_change[i] > 0 && Is_Start_Test)
                                                      {
                                                          logStatus(QString("Slave %1 - Valve %2: After Response Time, Detected %3 state transitions of the valve").arg(slave1_test).arg(valve * 8 + valve_bit +1).arg(Valve_count_change[i]));
                                                      }
                                                  }
                                                  Complete_Stop_Check_1 = true;
                                              });
                check_ValveChange.detach();
            }
        }

        if(complete_test_2 == true && slave2_test <= slave_count)
        {
            if(check_data_change_2 == false)
            {
                Is_checking_ValveChange = true;
                check_data_change_2 = true;
                std::thread check_ValveChange([&,numValveRandom]()
                                              {
                                                  Complete_Stop_Check_2 = false;
                                                  int Valve_count_change[42] = {0};
                                                  int invalve[42] = {0}, outvalve[42]= {0};
                                                  int inbit[42]= {0}, outbit[42]= {0};
                                                  bool Is_change[42] = {false};
                                                  auto next = std::chrono::steady_clock::now();
                                                  while(Is_checking_ValveChange && Is_Start_Test)
                                                  {
                                                      next+= std::chrono::microseconds(100);
                                                      processData_mutex.lock();
                                                      for(int i = 0; i <numValveRandom;i++)
                                                      {
                                                          int valve = valve_random[i] / 8;
                                                          int valve_bit = valve_random[i] % 8;

                                                          invalve[i] = IOmap[PDO_ValveReadback_Address_2 + valve];
                                                          outvalve[i] = IOmap[PDO_ValveControl_Address_2 + valve];
                                                          inbit[i] = invalve[i] & (1 << valve_bit);
                                                          outbit[i] = outvalve[i] & (1 << valve_bit);

                                                          if(inbit[i] != outbit[i])
                                                          {
                                                              if(Is_change[i] == false)
                                                              {
                                                                  Valve_count_change[i]++;

                                                              }
                                                              Is_change[i] = true;
                                                          }
                                                          else
                                                          {
                                                              Is_change[i] = false;
                                                          }

                                                      }
                                                      processData_mutex.unlock();
                                                      std::this_thread::sleep_until(next);
                                                  }
                                                  for(int i = 0; i < numValveRandom;i++)
                                                  {
                                                      int valve = valve_random[i] / 8;
                                                      int valve_bit = valve_random[i] % 8;
                                                      if(Valve_count_change[i] > 0 && Is_Start_Test)
                                                      {
                                                          logStatus(QString("Slave %1 - Valve %2: After Response Time, Detected %3 state transitions of the valve").arg(slave2_test).arg(valve * 8 + valve_bit +1).arg(Valve_count_change[i]));
                                                      }
                                                  }
                                                  Complete_Stop_Check_2 = true;
                                              });
                check_ValveChange.detach();
            }
        }

        if(complete_test_3 == true && slave3_test <= slave_count)
        {
            if(check_data_change_3 == false)
            {
                check_data_change_3 = true;
                Is_checking_ValveChange = true;
                std::thread check_ValveChange([&,numValveRandom]()
                                              {
                                                  Complete_Stop_Check_3 = false;
                                                  int Valve_count_change[42] = {0};
                                                  int invalve[42] = {0}, outvalve[42]= {0};
                                                  int inbit[42]= {0}, outbit[42]= {0};
                                                  bool Is_change[42] = {false};
                                                  auto next = std::chrono::steady_clock::now();
                                                  while(Is_checking_ValveChange && Is_Start_Test)
                                                  {
                                                      next+= std::chrono::microseconds(100);
                                                      processData_mutex.lock();

                                                      for(int i = 0; i <numValveRandom;i++)
                                                      {
                                                          int valve = valve_random[i] / 8;
                                                          int valve_bit = valve_random[i] % 8;

                                                          invalve[i] = IOmap[PDO_ValveReadback_Address_3 + valve];
                                                          outvalve[i] = IOmap[PDO_ValveControl_Address_3 + valve];
                                                          inbit[i] = invalve[i] & (1 << valve_bit);
                                                          outbit[i] = outvalve[i] & (1 << valve_bit);

                                                          if(inbit[i] != outbit[i])
                                                          {
                                                              if(Is_change[i] == false)
                                                              {
                                                                  Valve_count_change[i]++;
                                                                  qDebug() << QString("In_bit: %1, Out_bit: %2").arg(inbit[i]).arg(outbit[i]);
                                                              }
                                                              Is_change[i] = true;
                                                          }
                                                          else
                                                          {
                                                              Is_change[i] = false;
                                                          }
                                                      }
                                                      processData_mutex.unlock();

                                                      std::this_thread::sleep_until(next);
                                                  }
                                                  for(int i = 0; i < numValveRandom;i++)
                                                  {
                                                      int valve = valve_random[i] / 8;
                                                      int valve_bit = valve_random[i] % 8;

                                                      if(Valve_count_change[i] > 0 && Is_Start_Test)
                                                      {
                                                          logStatus(QString("Slave %1 - Valve %2: After Response Time, Detected %3 state transitions of the valve").arg(slave3_test).arg(valve * 8 + valve_bit +1).arg(Valve_count_change[i]));
                                                      }
                                                  }
                                                  Complete_Stop_Check_3 = true;
                                              });
                check_ValveChange.detach();
            }
        }

        if(complete_test_1 == true && complete_test_2 == true && complete_test_3 == true)
        {
            return;
        }
    }
}

void response_time_testing_42ev::check_time_response_manual(int numValve_Manual)
{
    QElapsedTimer Timer_count;
    qint64 time_elapsed;
    int invalve_1[42] = {0}, outvalve_1[42]= {0};
    int invalve_2[42]= {0}, outvalve_2[42]= {0};
    int invalve_3[42]= {0}, outvalve_3[42]= {0};
    int inbit_1[42]= {0}, outbit_1[42]= {0};
    int inbit_2[42]= {0}, outbit_2[42]= {0};
    int inbit_3[42]= {0}, outbit_3[42]= {0};
    bool complete_test_bit_1[42] = {false};
    bool complete_test_bit_2[42] = {false};
    bool complete_test_bit_3[42] = {false};
    bool complete_test_1 = false;
    bool complete_test_2 = false;
    bool complete_test_3 = false;
    bool check_data_change_1 = false;
    bool check_data_change_2 = false;
    bool check_data_change_3 = false;
    if(slave1_test > slave_count)
    {
        complete_test_1 = true;
        memset(complete_test_bit_1,true,sizeof(complete_test_bit_1));
    }
    if(slave2_test > slave_count)
    {
        complete_test_2 = true;
        memset(complete_test_bit_2,true,sizeof(complete_test_bit_2));

    }
    if(slave3_test > slave_count)
    {
        complete_test_3 = true;
        memset(complete_test_bit_3,true,sizeof(complete_test_bit_3));
    }
    Timer_count.start();
    while(Is_Start_Test == true)
    {
        Read_Valve();
        display_ValveReadback();
        display_ValveControl();
        time_elapsed = Timer_count.nsecsElapsed();
        double timeElapsed = time_elapsed/1e6;
        if(timeElapsed > max_response_time)
        {
            Is_Test_Fail = true;
            display_ValveReadback();
            display_ValveControl();
            if(complete_test_1 == false)
            {
                for(int i = 0; i < numValve_Manual; i++)
                {
                    int valve = valve_random[i]/8;
                    int valve_bit = valve_random[i]%8;
                    if(complete_test_bit_1[i] == false)
                    {
                        if(step % 2 == 0)
                        {
                            if(i % 2 == 0)
                            {
                                ui->Response_On_Time_TableWidget_1->item(valve,7 - valve_bit)->setText("ERR");
                            }
                            else
                            {
                                ui->Response_Off_Time_TableWidget_1->item(valve,7 - valve_bit)->setText("ERR");
                            }
                        }
                        else
                        {
                            if(i % 2 == 0)
                            {
                                ui->Response_Off_Time_TableWidget_1->item(valve,7 - valve_bit)->setText("ERR");
                            }
                            else
                            {
                                ui->Response_On_Time_TableWidget_1->item(valve,7 - valve_bit)->setText("ERR");
                            }
                        }
                        logStatus(QString("Slave %1 - Valve %2: Timeout error.").arg(slave1_test).arg(valve * 8 + valve_bit + 1));
                    }
                }
            }
            if(complete_test_2 == false)
            {
                for(int i = 0; i < numValve_Manual; i++)
                {
                    int valve = valve_random[i]/8;
                    int valve_bit = valve_random[i]%8;
                    if(complete_test_bit_2[i] == false)
                    {
                        if(step % 2 == 0)
                        {
                            if(i % 2 == 0)
                            {
                                ui->Response_On_Time_TableWidget_2->item(valve,7 - valve_bit)->setText("ERR");
                            }
                            else
                            {
                                ui->Response_Off_Time_TableWidget_2->item(valve,7 - valve_bit)->setText("ERR");
                            }
                        }
                        else
                        {
                            if(i % 2 == 0)
                            {
                                ui->Response_Off_Time_TableWidget_2->item(valve,7 - valve_bit)->setText("ERR");
                            }
                            else
                            {
                                ui->Response_On_Time_TableWidget_2->item(valve,7 - valve_bit)->setText("ERR");
                            }
                        }
                        logStatus(QString("Slave %1 - Valve %2: Timeout error.").arg(slave2_test).arg(valve * 8 + valve_bit + 1));
                    }
                }
            }
            if(complete_test_3 == false)
            {
                for(int i = 0; i < numValve_Manual; i++)
                {
                    int valve = valve_random[i]/8;
                    int valve_bit = valve_random[i]%8;
                    if(complete_test_bit_3[i] == false)
                    {
                        if(step % 2 == 0)
                        {
                            if(i % 2 == 0)
                            {
                                ui->Response_On_Time_TableWidget_3->item(valve,7 - valve_bit)->setText("ERR");
                            }
                            else
                            {
                                ui->Response_Off_Time_TableWidget_3->item(valve,7 - valve_bit)->setText("ERR");
                            }
                        }
                        else
                        {
                            if(i % 2 == 0)
                            {
                                ui->Response_Off_Time_TableWidget_3->item(valve,7 - valve_bit)->setText("ERR");
                            }
                            else
                            {
                                ui->Response_On_Time_TableWidget_3->item(valve,7 - valve_bit)->setText("ERR");
                            }
                        }
                        logStatus(QString("Slave %1 - Valve %2: Timeout error.").arg(slave3_test).arg(valve * 8 + valve_bit + 1));
                    }
                }
            }
            while(1)
            {
                Read_Valve();
                time_elapsed = Timer_count.nsecsElapsed();
                timeElapsed = time_elapsed/1e6;
                for(int i = 0; i < numValve_Manual; i++)
                {
                    int valve = valve_random[i] / 8;
                    int valve_bit = valve_random[i] %8;
                    switch(valve)
                    {
                    case 0:
                        invalve_1[i] = response_time_io_1->valve_readback_1to8;
                        outvalve_1[i] = response_time_io_1->valve_control_1to8;
                        inbit_1[i] = invalve_1[i] & (1 << valve_bit);
                        outbit_1[i] = outvalve_1[i] & (1 << valve_bit);

                        invalve_2[i] = response_time_io_2->valve_readback_1to8;
                        outvalve_2[i] = response_time_io_2->valve_control_1to8;
                        inbit_2[i] = invalve_2[i] & (1 << valve_bit);
                        outbit_2[i] = outvalve_2[i] & (1 << valve_bit);

                        invalve_3[i] = response_time_io_3->valve_readback_1to8;
                        outvalve_3[i] = response_time_io_3->valve_control_1to8;
                        inbit_3[i] = invalve_3[i] & (1 << valve_bit);
                        outbit_3[i] = outvalve_3[i] & (1 << valve_bit);
                        break;
                    case 1:
                        invalve_1[i] = response_time_io_1->valve_readback_9to16;
                        outvalve_1[i] = response_time_io_1->valve_control_9to16;
                        inbit_1[i] = invalve_1[i] & (1 << valve_bit);
                        outbit_1[i] = outvalve_1[i] & (1 << valve_bit);

                        invalve_2[i] = response_time_io_2->valve_readback_9to16;
                        outvalve_2[i] = response_time_io_2->valve_control_9to16;
                        inbit_2[i] = invalve_2[i] & (1 << valve_bit);
                        outbit_2[i] = outvalve_2[i] & (1 << valve_bit);

                        invalve_3[i] = response_time_io_3->valve_readback_9to16;
                        outvalve_3[i] = response_time_io_3->valve_control_9to16;
                        inbit_3[i] = invalve_3[i] & (1 << valve_bit);
                        outbit_3[i] = outvalve_3[i] & (1 << valve_bit);
                        break;

                    case 2:
                        invalve_1[i] = response_time_io_1->valve_readback_17to24;
                        outvalve_1[i] = response_time_io_1->valve_control_17to24;
                        inbit_1[i] = invalve_1[i] & (1 << valve_bit);
                        outbit_1[i] = outvalve_1[i] & (1 << valve_bit);

                        invalve_2[i] = response_time_io_2->valve_readback_17to24;
                        outvalve_2[i] = response_time_io_2->valve_control_17to24;
                        inbit_2[i] = invalve_2[i] & (1 << valve_bit);
                        outbit_2[i] = outvalve_2[i] & (1 << valve_bit);

                        invalve_3[i] = response_time_io_3->valve_readback_17to24;
                        outvalve_3[i] = response_time_io_3->valve_control_17to24;
                        inbit_3[i] = invalve_3[i] & (1 << valve_bit);
                        outbit_3[i] = outvalve_3[i] & (1 << valve_bit);
                        break;

                    case 3:
                        invalve_1[i] = response_time_io_1->valve_readback_25to32;
                        outvalve_1[i] = response_time_io_1->valve_control_25to32;
                        inbit_1[i] = invalve_1[i] & (1 << valve_bit);
                        outbit_1[i] = outvalve_1[i] & (1 << valve_bit);

                        invalve_2[i] = response_time_io_2->valve_readback_25to32;
                        outvalve_2[i] = response_time_io_2->valve_control_25to32;
                        inbit_2[i] = invalve_2[i] & (1 << valve_bit);
                        outbit_2[i] = outvalve_2[i] & (1 << valve_bit);

                        invalve_3[i] = response_time_io_3->valve_readback_25to32;
                        outvalve_3[i] = response_time_io_3->valve_control_25to32;
                        inbit_3[i] = invalve_3[i] & (1 << valve_bit);
                        outbit_3[i] = outvalve_3[i] & (1 << valve_bit);
                        break;
                    case 4:
                        invalve_1[i] = response_time_io_1->valve_readback_33to40;
                        outvalve_1[i] = response_time_io_1->valve_control_33to40;
                        inbit_1[i] = invalve_1[i] & (1 << valve_bit);
                        outbit_1[i] = outvalve_1[i] & (1 << valve_bit);

                        invalve_2[i] = response_time_io_2->valve_readback_33to40;
                        outvalve_2[i] = response_time_io_2->valve_control_33to40;
                        inbit_2[i] = invalve_2[i] & (1 << valve_bit);
                        outbit_2[i] = outvalve_2[i] & (1 << valve_bit);

                        invalve_3[i] = response_time_io_3->valve_readback_33to40;
                        outvalve_3[i] = response_time_io_3->valve_control_33to40;
                        inbit_3[i] = invalve_3[i] & (1 << valve_bit);
                        outbit_3[i] = outvalve_3[i] & (1 << valve_bit);
                        break;
                    case 5:
                        invalve_1[i] = response_time_io_1->valve_readback_41to48;
                        outvalve_1[i] = response_time_io_1->valve_control_41to48;
                        inbit_1[i] = invalve_1[i] & (1 << valve_bit);
                        outbit_1[i] = outvalve_1[i] & (1 << valve_bit);

                        invalve_2[i] = response_time_io_2->valve_readback_41to48;
                        outvalve_2[i] = response_time_io_2->valve_control_41to48;
                        inbit_2[i] = invalve_2[i] & (1 << valve_bit);
                        outbit_2[i] = outvalve_2[i] & (1 << valve_bit);

                        invalve_3[i] = response_time_io_3->valve_readback_41to48;
                        outvalve_3[i] = response_time_io_3->valve_control_41to48;
                        inbit_3[i] = invalve_3[i] & (1 << valve_bit);
                        outbit_3[i] = outvalve_3[i] & (1 << valve_bit);
                    }
                }
                for(int i = 0; i < numValve_Manual; i++)
                {
                    int valve = valve_random[i] / 8;
                    int valve_bit = valve_random[i] %8;
                    if(inbit_1[i] == outbit_1[i] && complete_test_bit_1[i] == false)
                    {
                        time_elapsed = Timer_count.nsecsElapsed();
                        timeElapsed = time_elapsed/1e6;
                        complete_test_bit_1[i] = true;

                        if(step % 2 == 0)
                        {
                            if(i % 2 == 0)
                            {
                                logStatus(QString("Slave %1 - Valve %2: Response ON time: %3ms").arg(slave1_test).arg(valve * 8 + valve_bit +1).arg(QString::number(timeElapsed,'f',1)));
                            }
                            else
                            {
                                logStatus(QString("Slave %1 - Valve %2: Response OFF time: %3ms").arg(slave1_test).arg(valve * 8 + valve_bit +1).arg(QString::number(timeElapsed,'f',1)));
                            }
                        }
                        else
                        {
                            if(i % 2 == 0)
                            {
                                logStatus(QString("Slave %1 - Valve %2: Response OFF time: %3ms").arg(slave1_test).arg(valve * 8 + valve_bit +1).arg(QString::number(timeElapsed,'f',1)));
                            }
                            else
                            {
                                logStatus(QString("Slave %1 - Valve %2: Response ON time: %3ms").arg(slave1_test).arg(valve * 8 + valve_bit +1).arg(QString::number(timeElapsed,'f',1)));
                            }
                        }
                    }
                    if(inbit_2[i] == outbit_2[i] && complete_test_bit_2[i] == false)
                    {
                        time_elapsed = Timer_count.nsecsElapsed();
                        timeElapsed = time_elapsed/1e6;
                        complete_test_bit_2[i] = true;
                        if(step % 2 == 0)
                        {
                            if(i % 2 == 0)
                            {
                                logStatus(QString("Slave %1 - Valve %2: Response ON time: %3ms").arg(slave2_test).arg(valve * 8 + valve_bit +1).arg(QString::number(timeElapsed,'f',1)));
                            }
                            else
                            {
                                logStatus(QString("Slave %1 - Valve %2: Response OFF time: %3ms").arg(slave2_test).arg(valve * 8 + valve_bit +1).arg(QString::number(timeElapsed,'f',1)));
                            }
                        }
                        else
                        {
                            if(i % 2 == 0)
                            {
                                logStatus(QString("Slave %1 - Valve %2: Response OFF time: %3ms").arg(slave2_test).arg(valve * 8 + valve_bit +1).arg(QString::number(timeElapsed,'f',1)));
                            }
                            else
                            {
                                logStatus(QString("Slave %1 - Valve %2: Response ON time: %3ms").arg(slave2_test).arg(valve * 8 + valve_bit +1).arg(QString::number(timeElapsed,'f',1)));
                            }
                        }
                    }
                    if(inbit_3[i] == outbit_3[i] && complete_test_bit_3[i] == false)
                    {
                        time_elapsed = Timer_count.nsecsElapsed();
                        timeElapsed = time_elapsed/1e6;
                        complete_test_bit_3[i] = true;
                        if(step % 2 == 0)
                        {
                            if(i % 2 == 0)
                            {
                                logStatus(QString("Slave %1 - Valve %2: Response ON time: %3ms").arg(slave3_test).arg(valve * 8 + valve_bit +1).arg(QString::number(timeElapsed,'f',1)));
                            }
                            else
                            {
                                logStatus(QString("Slave %1 - Valve %2: Response OFF time: %3ms").arg(slave3_test).arg(valve * 8 + valve_bit +1).arg(QString::number(timeElapsed,'f',1)));
                            }
                        }
                        else
                        {
                            if(i % 2 == 0)
                            {
                                logStatus(QString("Slave %1 - Valve %2: Response OFF time: %3ms").arg(slave3_test).arg(valve * 8 + valve_bit +1).arg(QString::number(timeElapsed,'f',1)));
                            }
                            else
                            {
                                logStatus(QString("Slave %1 - Valve %2: Response ON time: %3ms").arg(slave3_test).arg(valve * 8 + valve_bit +1).arg(QString::number(timeElapsed,'f',1)));
                            }
                        }
                    }
                }

                complete_test_1 = true;
                complete_test_2 = true;
                complete_test_3 = true;
                for(int i = 0; i < numValve_Manual; i++)
                {
                    if(complete_test_bit_1[i] == false)
                    {
                        complete_test_1 = false;
                    }
                    if(complete_test_bit_2[i] == false)
                    {
                        complete_test_2 = false;
                    }
                    if(complete_test_bit_3[i] == false)
                    {
                        complete_test_3 = false;
                    }
                }

                if(complete_test_1 == true && complete_test_2 == true && complete_test_3 == true)
                {
                    return;
                }
            }
        }

        for(int i = 0; i < numValve_Manual; i++)
        {
            int valve = valve_manual_test[i] / 8;
            int valve_bit = valve_manual_test[i] % 8;
            switch(valve)
            {
            case 0:
                invalve_1[i] = response_time_io_1->valve_readback_1to8;
                outvalve_1[i] = response_time_io_1->valve_control_1to8;
                inbit_1[i] = invalve_1[i] & (1 << valve_bit);
                outbit_1[i] = outvalve_1[i] & (1 << valve_bit);

                invalve_2[i] = response_time_io_2->valve_readback_1to8;
                outvalve_2[i] = response_time_io_2->valve_control_1to8;
                inbit_2[i] = invalve_2[i] & (1 << valve_bit);
                outbit_2[i] = outvalve_2[i] & (1 << valve_bit);

                invalve_3[i] = response_time_io_3->valve_readback_1to8;
                outvalve_3[i] = response_time_io_3->valve_control_1to8;
                inbit_3[i] = invalve_3[i] & (1 << valve_bit);
                outbit_3[i] = outvalve_3[i] & (1 << valve_bit);
                break;
            case 1:
                invalve_1[i] = response_time_io_1->valve_readback_9to16;
                outvalve_1[i] = response_time_io_1->valve_control_9to16;
                inbit_1[i] = invalve_1[i] & (1 << valve_bit);
                outbit_1[i] = outvalve_1[i] & (1 << valve_bit);

                invalve_2[i] = response_time_io_2->valve_readback_9to16;
                outvalve_2[i] = response_time_io_2->valve_control_9to16;
                inbit_2[i] = invalve_2[i] & (1 << valve_bit);
                outbit_2[i] = outvalve_2[i] & (1 << valve_bit);

                invalve_3[i] = response_time_io_3->valve_readback_9to16;
                outvalve_3[i] = response_time_io_3->valve_control_9to16;
                inbit_3[i] = invalve_3[i] & (1 << valve_bit);
                outbit_3[i] = outvalve_3[i] & (1 << valve_bit);
                break;

            case 2:
                invalve_1[i] = response_time_io_1->valve_readback_17to24;
                outvalve_1[i] = response_time_io_1->valve_control_17to24;
                inbit_1[i] = invalve_1[i] & (1 << valve_bit);
                outbit_1[i] = outvalve_1[i] & (1 << valve_bit);

                invalve_2[i] = response_time_io_2->valve_readback_17to24;
                outvalve_2[i] = response_time_io_2->valve_control_17to24;
                inbit_2[i] = invalve_2[i] & (1 << valve_bit);
                outbit_2[i] = outvalve_2[i] & (1 << valve_bit);

                invalve_3[i] = response_time_io_3->valve_readback_17to24;
                outvalve_3[i] = response_time_io_3->valve_control_17to24;
                inbit_3[i] = invalve_3[i] & (1 << valve_bit);
                outbit_3[i] = outvalve_3[i] & (1 << valve_bit);
                break;

            case 3:
                invalve_1[i] = response_time_io_1->valve_readback_25to32;
                outvalve_1[i] = response_time_io_1->valve_control_25to32;
                inbit_1[i] = invalve_1[i] & (1 << valve_bit);
                outbit_1[i] = outvalve_1[i] & (1 << valve_bit);

                invalve_2[i] = response_time_io_2->valve_readback_25to32;
                outvalve_2[i] = response_time_io_2->valve_control_25to32;
                inbit_2[i] = invalve_2[i] & (1 << valve_bit);
                outbit_2[i] = outvalve_2[i] & (1 << valve_bit);

                invalve_3[i] = response_time_io_3->valve_readback_25to32;
                outvalve_3[i] = response_time_io_3->valve_control_25to32;
                inbit_3[i] = invalve_3[i] & (1 << valve_bit);
                outbit_3[i] = outvalve_3[i] & (1 << valve_bit);
                break;
            case 4:
                invalve_1[i] = response_time_io_1->valve_readback_33to40;
                outvalve_1[i] = response_time_io_1->valve_control_33to40;
                inbit_1[i] = invalve_1[i] & (1 << valve_bit);
                outbit_1[i] = outvalve_1[i] & (1 << valve_bit);

                invalve_2[i] = response_time_io_2->valve_readback_33to40;
                outvalve_2[i] = response_time_io_2->valve_control_33to40;
                inbit_2[i] = invalve_2[i] & (1 << valve_bit);
                outbit_2[i] = outvalve_2[i] & (1 << valve_bit);

                invalve_3[i] = response_time_io_3->valve_readback_33to40;
                outvalve_3[i] = response_time_io_3->valve_control_33to40;
                inbit_3[i] = invalve_3[i] & (1 << valve_bit);
                outbit_3[i] = outvalve_3[i] & (1 << valve_bit);
                break;
            case 5:
                invalve_1[i] = response_time_io_1->valve_readback_41to48;
                outvalve_1[i] = response_time_io_1->valve_control_41to48;
                inbit_1[i] = invalve_1[i] & (1 << valve_bit);
                outbit_1[i] = outvalve_1[i] & (1 << valve_bit);

                invalve_2[i] = response_time_io_2->valve_readback_41to48;
                outvalve_2[i] = response_time_io_2->valve_control_41to48;
                inbit_2[i] = invalve_2[i] & (1 << valve_bit);
                outbit_2[i] = outvalve_2[i] & (1 << valve_bit);

                invalve_3[i] = response_time_io_3->valve_readback_41to48;
                outvalve_3[i] = response_time_io_3->valve_control_41to48;
                inbit_3[i] = invalve_3[i] & (1 << valve_bit);
                outbit_3[i] = outvalve_3[i] & (1 << valve_bit);
            }
        }
        for(int i = 0; i < numValve_Manual; i++)
        {
            int valve = valve_manual_test[i] / 8;
            int valve_bit = valve_manual_test[i] %8;
            if(inbit_1[i] == outbit_1[i] && complete_test_bit_1[i] == false)
            {
                time_elapsed = Timer_count.nsecsElapsed();
                timeElapsed = time_elapsed/1e6;
                complete_test_bit_1[i] = true;
                if(step % 2 == 0)
                {
                    if(i % 2 == 0)
                    {
                        ui->Response_On_Time_TableWidget_1->item(valve, 7 - valve_bit)->setText(QString::number(timeElapsed,'f',1));
                    }
                    else
                    {
                        ui->Response_Off_Time_TableWidget_1->item(valve, 7 - valve_bit)->setText(QString::number(timeElapsed,'f',1));
                    }
                }
                else
                {
                    if(i % 2 == 0)
                    {
                        ui->Response_Off_Time_TableWidget_1->item(valve, 7 - valve_bit)->setText(QString::number(timeElapsed,'f',1));
                    }
                    else
                    {
                        ui->Response_On_Time_TableWidget_1->item(valve, 7 - valve_bit)->setText(QString::number(timeElapsed,'f',1));
                    }
                }
            }
            if(inbit_2[i] == outbit_2[i] && complete_test_bit_2[i] == false)
            {
                time_elapsed = Timer_count.nsecsElapsed();
                timeElapsed = time_elapsed/1e6;
                complete_test_bit_2[i] = true;
                if(step % 2 == 0)
                {
                    if(i % 2 == 0)
                    {
                        ui->Response_On_Time_TableWidget_2->item(valve, 7 - valve_bit)->setText(QString::number(timeElapsed,'f',1));
                    }
                    else
                    {
                        ui->Response_Off_Time_TableWidget_2->item(valve, 7 - valve_bit)->setText(QString::number(timeElapsed,'f',1));
                    }
                }
                else
                {
                    if(i % 2 == 0)
                    {
                        ui->Response_Off_Time_TableWidget_2->item(valve, 7 - valve_bit)->setText(QString::number(timeElapsed,'f',1));
                    }
                    else
                    {
                        ui->Response_On_Time_TableWidget_2->item(valve, 7 - valve_bit)->setText(QString::number(timeElapsed,'f',1));
                    }
                }
            }
            if(inbit_3[i] == outbit_3[i] && complete_test_bit_3[i] == false)
            {
                time_elapsed = Timer_count.nsecsElapsed();
                timeElapsed = time_elapsed/1e6;
                complete_test_bit_3[i] = true;
                if(step % 2 == 0)
                {
                    if(i % 2 == 0)
                    {
                        ui->Response_On_Time_TableWidget_3->item(valve, 7 - valve_bit)->setText(QString::number(timeElapsed,'f',1));
                    }
                    else
                    {
                        ui->Response_Off_Time_TableWidget_3->item(valve, 7 - valve_bit)->setText(QString::number(timeElapsed,'f',1));
                    }
                }
                else
                {
                    if(i % 2 == 0)
                    {
                        ui->Response_Off_Time_TableWidget_3->item(valve, 7 - valve_bit)->setText(QString::number(timeElapsed,'f',1));
                    }
                    else
                    {
                        ui->Response_On_Time_TableWidget_3->item(valve, 7 - valve_bit)->setText(QString::number(timeElapsed,'f',1));
                    }
                }
            }
        }

        complete_test_1 = true;
        complete_test_2 = true;
        complete_test_3 = true;
        for(int i = 0; i < numValve_Manual; i++)
        {
            if(complete_test_bit_1[i] == false)
            {
                complete_test_1 = false;
            }
            if(complete_test_bit_2[i] == false)
            {
                complete_test_2 = false;
            }
            if(complete_test_bit_3[i] == false)
            {
                complete_test_3 = false;
            }
        }

        if(complete_test_1 == true && slave1_test <= slave_count)
        {
            if(check_data_change_1 == false)
            {
                check_data_change_1 = true;
                Is_checking_ValveChange = true;
                std::thread check_ValveChange([&]()
                                              {
                                                  Complete_Stop_Check_1 = false;
                                                  int Valve_count_change[42] = {0};
                                                  int invalve[42] = {0}, outvalve[42]= {0};
                                                  int inbit[42]= {0}, outbit[42]= {0};
                                                  bool Is_change[42] = {false};
                                                  auto next = std::chrono::steady_clock::now();
                                                  while(Is_checking_ValveChange && Is_Start_Test)
                                                  {
                                                      next+= std::chrono::microseconds(100);

                                                      processData_mutex.lock();


                                                      for(int i = 0; i <valve_manual_test.count();i++)
                                                      {
                                                          int valve = valve_manual_test[i] / 8;
                                                          int valve_bit = valve_manual_test[i] % 8;

                                                          invalve[i] = IOmap[PDO_ValveReadback_Address_1 + valve];
                                                          outvalve[i] = IOmap[PDO_ValveControl_Address_1 + valve];
                                                          inbit[i] = invalve[i] & (1 << valve_bit);
                                                          outbit[i] = outvalve[i] & (1 << valve_bit);

                                                          if(inbit[i] != outbit[i])
                                                          {
                                                              if(Is_change[i] == false)
                                                              {
                                                                  Valve_count_change[i]++;
                                                                  qDebug() << QString("In_bit: %1, Out_bit: %2").arg(inbit[i]).arg(outbit[i]);
                                                              }
                                                              Is_change[i] = true;
                                                          }
                                                          else
                                                          {
                                                              Is_change[i] = false;
                                                          }
                                                      }
                                                      processData_mutex.unlock();

                                                      std::this_thread::sleep_until(next);
                                                  }

                                                  for(int i = 0; i < valve_manual_test.count();i++)
                                                  {
                                                      int valve = valve_manual_test[i] / 8;
                                                      int valve_bit = valve_manual_test[i] % 8;
                                                      if(Valve_count_change[i] > 0 && Is_Start_Test)
                                                      {
                                                          logStatus(QString("Slave %1 - Valve %2: After Response Time, Detected %3 state transitions of the valve").arg(slave1_test).arg(valve * 8 + valve_bit +1).arg(Valve_count_change[i]));
                                                      }
                                                  }
                                                  Complete_Stop_Check_1 = true;
                                              });
                check_ValveChange.detach();
            }
        }

        if(complete_test_2 == true && slave2_test <= slave_count)
        {
            if(check_data_change_2 == false)
            {
                Is_checking_ValveChange = true;
                check_data_change_2 = true;
                std::thread check_ValveChange([&]()
                                              {
                                                  Complete_Stop_Check_2 = false;
                                                  int Valve_count_change[42] = {0};
                                                  int invalve[42] = {0}, outvalve[42]= {0};
                                                  int inbit[42]= {0}, outbit[42]= {0};
                                                  bool Is_change[42] = {false};
                                                  auto next = std::chrono::steady_clock::now();
                                                  while(Is_checking_ValveChange && Is_Start_Test)
                                                  {
                                                      next+= std::chrono::microseconds(100);

                                                      processData_mutex.lock();

                                                      for(int i = 0; i <valve_manual_test.count();i++)
                                                      {
                                                          int valve = valve_manual_test[i] / 8;
                                                          int valve_bit = valve_manual_test[i] % 8;

                                                          invalve[i] = IOmap[PDO_ValveReadback_Address_2 + valve];
                                                          outvalve[i] = IOmap[PDO_ValveControl_Address_2 + valve];
                                                          inbit[i] = invalve[i] & (1 << valve_bit);
                                                          outbit[i] = outvalve[i] & (1 << valve_bit);

                                                          if(inbit[i] != outbit[i])
                                                          {
                                                              if(Is_change[i] == false)
                                                              {
                                                                  Valve_count_change[i]++;
                                                                  qDebug() << QString("In_bit: %1, Out_bit: %2").arg(invalve[i]).arg(outvalve[i]);
                                                              }
                                                              Is_change[i] = true;
                                                          }
                                                          else
                                                          {
                                                              Is_change[i] = false;
                                                          }

                                                      }
                                                      processData_mutex.unlock();

                                                      std::this_thread::sleep_until(next);
                                                  }
                                                  for(int i = 0; i < valve_manual_test.count();i++)
                                                  {
                                                      int valve = valve_manual_test[i] / 8;
                                                      int valve_bit = valve_manual_test[i] % 8;

                                                      if(Valve_count_change[i] > 0 && Is_Start_Test)
                                                      {
                                                          logStatus(QString("Slave %1 - Valve %2: After Response Time, Detected %3 state transitions of the valve").arg(slave2_test).arg(valve * 8 + valve_bit +1).arg(Valve_count_change[i]));
                                                      }
                                                  }
                                                  Complete_Stop_Check_2 = true;
                                              });
                check_ValveChange.detach();
            }
        }

        if(complete_test_3 == true && slave3_test <= slave_count)
        {
            if(check_data_change_3 == false)
            {
                check_data_change_3 = true;
                Is_checking_ValveChange = true;
                std::thread check_ValveChange([&]()
                                              {
                                                  Complete_Stop_Check_3 = false;
                                                  int Valve_count_change[42] = {0};
                                                  int invalve[42] = {0}, outvalve[42]= {0};
                                                  int inbit[42]= {0}, outbit[42]= {0};
                                                  bool Is_change[42] = {false};
                                                  auto next = std::chrono::steady_clock::now();
                                                  while(Is_checking_ValveChange && Is_Start_Test)
                                                  {
                                                      next+= std::chrono::microseconds(100);

                                                      processData_mutex.lock();


                                                      for(int i = 0; i <valve_manual_test.count();i++)
                                                      {
                                                          int valve = valve_manual_test[i] / 8;
                                                          int valve_bit = valve_manual_test[i] % 8;

                                                          invalve[i] = IOmap[PDO_ValveReadback_Address_3 + valve];
                                                          outvalve[i] = IOmap[PDO_ValveControl_Address_3 + valve];
                                                          inbit[i] = invalve[i] & (1 << valve_bit);
                                                          outbit[i] = outvalve[i] & (1 << valve_bit);

                                                          if(inbit[i] != outbit[i])
                                                          {
                                                              if(Is_change[i] == false)
                                                              {
                                                                  Valve_count_change[i]++;
                                                                  qDebug() << QString("In_bit: %1, Out_bit: %2").arg(inbit[i]).arg(outbit[i]);
                                                              }
                                                              Is_change[i] = true;
                                                          }
                                                          else
                                                          {
                                                              Is_change[i] = false;
                                                          }
                                                      }
                                                      processData_mutex.unlock();

                                                      std::this_thread::sleep_until(next);
                                                  }
                                                  for(int i = 0; i < valve_manual_test.count();i++)
                                                  {
                                                      int valve = valve_manual_test[i] / 8;
                                                      int valve_bit = valve_manual_test[i] % 8;

                                                      if(Valve_count_change[i] > 0 && Is_Start_Test)
                                                      {
                                                          logStatus(QString("Slave %1 - Valve %2: After Response Time, Detected %3 state transitions of the valve").arg(slave3_test).arg(valve * 8 + valve_bit +1).arg(Valve_count_change[i]));
                                                      }
                                                  }
                                                  Complete_Stop_Check_3 = true;
                                              });
                check_ValveChange.detach();
            }
        }

        if(complete_test_1 == true && complete_test_2 == true && complete_test_3 == true)
        {
            return;
        }
    }
}

bool response_time_testing_42ev::Check_ValveTest_String(QString str)
{
    QRegularExpression re("^\\d+(,\\d+)*$");
    bool Is_Valid = re.match(str).hasMatch();
    if(Is_Valid)
    {
        QStringList lists = str.split(",");
        valve_manual_test.clear();
        for(const QString& list: lists)
        {
            bool ok = false;
            int numValve = list.toInt(&ok);
            if(ok)
            {
                valve_manual_test.append(numValve - 1);
            }
        }
    }
    return Is_Valid;
}

void response_time_testing_42ev::LoadParamConfig()
{
    // XMLDocument doc;
    // if (doc.LoadFile(Project_ConfigPath.toStdString().c_str()) != XML_SUCCESS)
    // {
    //     return;
    // }
    // XMLElement* root = doc.RootElement();
    // if (!root)
    // {
    //     return;
    // }

    // XMLElement* ResponseTimeTest = root->FirstChildElement("ResponseTimeTest_42EV");
    // ResponseTimeTest->FirstChildElement("NextStepTime")->QueryIntText(&response_time_autoTest->next_Step_Time);
    // ResponseTimeTest->FirstChildElement("MaxResponseTime")->QueryIntText(&response_time_autoTest->max_Response_Time);
    // ResponseTimeTest->FirstChildElement("VerifySteps")->QueryIntText(&response_time_autoTest->verify_Steps);
    // response_time_autoTest->Valve_Test = ResponseTimeTest->FirstChildElement("ValveTest")->GetText();
    response_time_autoTest->next_Step_Time = 400;
    response_time_autoTest->max_Response_Time = 20;
    response_time_autoTest->verify_Steps = 2;
    response_time_autoTest->Valve_Test = "1";
}

void response_time_testing_42ev::SaveParamConfig()
{
    // XMLDocument doc;
    // if (doc.LoadFile(Project_ConfigPath.toStdString().c_str()) != XML_SUCCESS)
    // {
    //     return;
    // }
    // XMLElement* root = doc.RootElement();
    // if (!root)
    // {
    //     return;
    // }

    // XMLElement* ResponseTimeTest = root->FirstChildElement("ResponseTimeTest_42EV");
    // ResponseTimeTest->FirstChildElement("NextStepTime")->SetText(response_time_autoTest->next_Step_Time);
    // ResponseTimeTest->FirstChildElement("MaxResponseTime")->SetText(response_time_autoTest->max_Response_Time);
    // ResponseTimeTest->FirstChildElement("VerifySteps")->SetText(response_time_autoTest->verify_Steps);
    // ResponseTimeTest->FirstChildElement("ValveTest")->SetText(response_time_autoTest->Valve_Test.toStdString().c_str());

    // if (doc.SaveFile(Project_ConfigPath.toStdString().c_str()) != XML_SUCCESS) {
    //     std::cerr << "Không thể lưu file config.txt\n";
    // }
}

void response_time_testing_42ev::LoadForm()
{
    LoadParamConfig();
    ui->Next_Step_textBox->setText(QString::number(response_time_autoTest->next_Step_Time));
    ui->Max_Response_textBox->setText(QString::number(response_time_autoTest->max_Response_Time));
    ui->Verify_Step_textBox->setText(QString::number(response_time_autoTest->verify_Steps));
    ui->Valve_Test_textBox->setText(response_time_autoTest->Valve_Test);
    ui->Valve_Test_label->setVisible(false);
    ui->Valve_Test_textBox->setVisible(false);

    auto setStretch = [](QTableWidget* table) {
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        table->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    };

    setStretch(ui->Valve_Control_TableWidget_1);
    setStretch(ui->Valve_Readback_TableWidget_1);
    setStretch(ui->Response_On_Time_TableWidget_1);
    setStretch(ui->Response_Off_Time_TableWidget_1);

    setStretch(ui->Valve_Control_TableWidget_2);
    setStretch(ui->Valve_Readback_TableWidget_2);
    setStretch(ui->Response_On_Time_TableWidget_2);
    setStretch(ui->Response_Off_Time_TableWidget_2);

    setStretch(ui->Valve_Control_TableWidget_3);
    setStretch(ui->Valve_Readback_TableWidget_3);
    setStretch(ui->Response_On_Time_TableWidget_3);
    setStretch(ui->Response_Off_Time_TableWidget_3);


    for(int i = 0; i < slave_count;i++)
    {
        QString name_Slave = QString::fromUtf8(slave_info_xml[i].slave_name);
        ui->slave_comboBox_1->addItem(name_Slave);
        ui->slave_comboBox_2->addItem(name_Slave);
        ui->slave_comboBox_3->addItem(name_Slave);
    }

    ui->slave_comboBox_1->addItem("None");
    ui->slave_comboBox_2->addItem("None");
    ui->slave_comboBox_3->addItem("None");
    ui->slave_comboBox_1->setCurrentIndex(0);

    // switch(slave_count)
    // {
    // case 0:
    //     ui->slave_comboBox_1->setCurrentIndex(0);
    //     slave1_test = ui->slave_comboBox_1->currentIndex() + 1;
    //     ui->slave_comboBox_2->setCurrentIndex(0);
    //     slave2_test = ui->slave_comboBox_2->currentIndex() + 1;
    //     ui->slave_comboBox_3->setCurrentIndex(0);
    //     slave3_test = ui->slave_comboBox_3->currentIndex() + 1;
    //     break;
    // case 1:
    //     ui->slave_comboBox_1->setCurrentIndex(0);
    //     slave1_test = ui->slave_comboBox_1->currentIndex() + 1;
    //     ui->slave_comboBox_2->setCurrentIndex(1);
    //     slave2_test = ui->slave_comboBox_2->currentIndex() + 1;
    //     ui->slave_comboBox_3->setCurrentIndex(1);
    //     slave3_test = ui->slave_comboBox_3->currentIndex() + 1;
    //     break;
    // default:
    //     ui->slave_comboBox_1->setCurrentIndex(0);
    //     slave1_test = ui->slave_comboBox_1->currentIndex() + 1;
    //     ui->slave_comboBox_2->setCurrentIndex(1);
    //     slave2_test = ui->slave_comboBox_2->currentIndex() + 1;
    //     ui->slave_comboBox_3->setCurrentIndex(2);
    //     slave3_test = ui->slave_comboBox_3->currentIndex() + 1;
    //     break;
    // }

    QIntValidator * validator = new QIntValidator(0,INT_MAX,this);
    ui->Next_Step_textBox->setValidator(validator);
    ui->Verify_Step_textBox->setValidator(validator);
    ui->Max_Response_textBox->setValidator(validator);
    ui->Valve_Control_TableWidget_1->setRowCount(6);
    ui->Valve_Control_TableWidget_1->setColumnCount(8);
    ui->Valve_Control_TableWidget_2->setRowCount(6);
    ui->Valve_Control_TableWidget_2->setColumnCount(8);
    ui->Valve_Control_TableWidget_3->setRowCount(6);
    ui->Valve_Control_TableWidget_3->setColumnCount(8);

    ui->Valve_Readback_TableWidget_1->setRowCount(6);
    ui->Valve_Readback_TableWidget_1->setColumnCount(8);
    ui->Valve_Readback_TableWidget_2->setRowCount(6);
    ui->Valve_Readback_TableWidget_2->setColumnCount(8);
    ui->Valve_Readback_TableWidget_3->setRowCount(6);
    ui->Valve_Readback_TableWidget_3->setColumnCount(8);

    ui->Response_On_Time_TableWidget_1->setRowCount(6);
    ui->Response_On_Time_TableWidget_1->setColumnCount(8);
    ui->Response_On_Time_TableWidget_2->setRowCount(6);
    ui->Response_On_Time_TableWidget_2->setColumnCount(8);
    ui->Response_On_Time_TableWidget_3->setRowCount(6);
    ui->Response_On_Time_TableWidget_3->setColumnCount(8);

    ui->Response_Off_Time_TableWidget_1->setRowCount(6);
    ui->Response_Off_Time_TableWidget_1->setColumnCount(8);
    ui->Response_Off_Time_TableWidget_2->setRowCount(6);
    ui->Response_Off_Time_TableWidget_2->setColumnCount(8);
    ui->Response_Off_Time_TableWidget_3->setRowCount(6);
    ui->Response_Off_Time_TableWidget_3->setColumnCount(8);
    resetTableWidget();
    max_response_time = ui->Max_Response_textBox->text().toInt();
}

void response_time_testing_42ev::RunAllTest_API()
{
    ui->Verify_Step_textBox->setText("10");
    ui->Max_Response_textBox->setText("20");
    ui->Sequential_radioButton->setChecked(true);
    this->on_Start_Test_button_clicked();
    ui->Random_radioButton->setChecked(true);
    this->on_Start_Test_button_clicked();
    ui->Manual_radioButton->setChecked(true);
    this->on_Start_Test_button_clicked();
}

void response_time_testing_42ev::RunNormalTest_API()
{
    ui->Verify_Step_textBox->setText(QString::number(response_time_autoTest->verify_Steps));
    ui->Max_Response_textBox->setText(QString::number(response_time_autoTest->max_Response_Time));
    ui->Next_Step_textBox->setText(QString::number(response_time_autoTest->next_Step_Time));
    ui->Sequential_radioButton->setChecked(true);
    max_response_time = response_time_autoTest->max_Response_Time;
    ui->slave_comboBox_1->setCurrentIndex(0);
    ui->slave_comboBox_2->setCurrentIndex(slave_count);
    ui->slave_comboBox_3->setCurrentIndex(slave_count);
    this->on_Start_Test_button_clicked();
}

void response_time_testing_42ev::RunManualTest_API()
{
    ui->Verify_Step_textBox->setText("10");
    ui->Manual_radioButton->setText("20");
    ui->Manual_radioButton->setChecked(true);
    this->on_Start_Test_button_clicked();
}

void response_time_testing_42ev::RunRandomTest_API()
{
    ui->Verify_Step_textBox->setText("10");
    ui->Max_Response_textBox->setText("20");
    ui->Random_radioButton->setChecked(true);
    this->on_Start_Test_button_clicked();
}

void response_time_testing_42ev::closeEvent(QCloseEvent *event)
{
    // if(logBuffer.size() > 0)
    // {
    //     QString LogsFolder_Path = Project_FolderPath + "/logs/Response_Time_Reports/";
    //     QString LogsFile_Path = LogsFolder_Path +"Normal_Test "+ QDate::currentDate().toString("MM-dd ") + QTime::currentTime().toString("HH.mm") +".txt";
    //     if(ui->Sequential_radioButton->isChecked())
    //     {
    //         LogsFile_Path = LogsFolder_Path +"Normal_Test "+ QDate::currentDate().toString("MM-dd ") + QTime::currentTime().toString("HH.mm") +".txt";
    //     }
    //     else if(ui->Random_radioButton->isChecked())
    //     {
    //         LogsFile_Path = LogsFolder_Path +"Random_Test "+ QDate::currentDate().toString("MM-dd ") + QTime::currentTime().toString("HH.mm") +".txt";
    //     }
    //     else if(ui->Manual_radioButton->isChecked())
    //     {
    //         LogsFile_Path = LogsFolder_Path +"Manual_Test "+ QDate::currentDate().toString("MM-dd ") + QTime::currentTime().toString("HH.mm") +".txt";
    //     }
    //     QDir dir;
    //     if(!dir.exists(LogsFolder_Path))
    //     {
    //         dir.mkpath(LogsFolder_Path);
    //     }

    //     QFile file(LogsFile_Path);
    //     if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    //     {
    //         QTextStream stream(&file);
    //         for(const QString& line: logBuffer)
    //         {
    //             stream << line<<"\n";
    //         }
    //         file.close();
    //     }
    // }
    // SaveParamConfig();
    event->accept();
}


void response_time_testing_42ev::on_Manual_radioButton_toggled(bool checked)
{
    if(checked)
    {
        ui->Valve_Test_label->setVisible(true);
        ui->Valve_Test_textBox->setVisible(true);
    }
    else
    {
        ui->Valve_Test_label->setVisible(false);
        ui->Valve_Test_textBox->setVisible(false);
    }
}

