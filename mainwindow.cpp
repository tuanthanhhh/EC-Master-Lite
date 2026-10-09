#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "ec_info.h"
#include "ec_control.h"
#include <QStandardPaths>
#include <QMessageBox>
#include <QTimer>
#include "Global_Variable.h"
#include "PDOMapping.h"
#include <QDateTime>
#include <QtConcurrent/QtConcurrent>
#include <QIntValidator>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Timer_HotConnect = new QTimer(this);
    connect(Timer_HotConnect,SIGNAL(timeout()),this,SLOT(Hot_Connect()));

    Timer_Check_SlaveState = new QTimer(this);
    connect(Timer_Check_SlaveState,SIGNAL(timeout()),this,SLOT(Check_SlaveState()));

    QRegularExpression regex("^(0[xX])?[0-9A-Fa-f]{1,3}$");
    ui->Check_Address_textBox->setValidator(new QRegularExpressionValidator(regex, ui->Check_Address_textBox));
    ui->Write_SerialNumber_textBox->setValidator(new QIntValidator(0, INT_MAX, ui->Write_SerialNumber_textBox));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Start_Test_button_clicked()
{
    SerialNumber_str = ui->Write_SerialNumber_textBox->text();
    QString address_sw = ui->Check_Address_textBox->text().trimmed();
    bool error_flag = false;

    QStringList list_err;
    QString err;
    if(SerialNumber_str.length() != 9)
    {
        err = "- Invalid serial number format. Expected decimal format: WW YY XXXXX";
        list_err << err;
        error_flag = true;
    }

    if(address_sw.startsWith("0x",Qt::CaseInsensitive))
    {
        address_sw = address_sw.mid(2);
    }
    bool ok;
    uint16_t address_sw_value = address_sw.toUShort(&ok,16);
    if(ok == false || address_sw_value > 0xFFF)
    {
        err = "- Invalid Address format. Expected decimal format: 0xAAA or AAA";
        list_err << err;
        error_flag = true;
    }

    if(error_flag == true)
    {
        QMessageBox::critical(this,"Validation Error",list_err.join("\n"));
        return;
    }
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    ui->Start_Test_Time_textBox->setText(timestamp);
    ui->Result_Test_textBox->setText("RUNNING");
    ui->Test_Status_Icon->clear();
    ui->Check_Address_Icon->clear();
    ui->Ecat_Out_Icon->clear();
    ui->On_Off_Valve_Icon->clear();
    ui->Write_Serial_Number_Icon->clear();
    Is_FinalTest_Pass = true;

    QStringList list_test;
    list_test << "1";
    if(ui->Ecat_Out_checkBox->isChecked())
    {
        list_test << "2";
    }
    if(ui->Check_Address_checkBox->isChecked())
    {
        list_test << "3";
    }
    if(ui->Write_Serial_Number_checkBox->isChecked())
    {
        list_test << "4";
    }
    if(ui->On_Off_Valve_checkBox->isChecked())
    {
        list_test << "5";
    }
    QString Local_Path = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    QString ENI_Path = Local_Path + "/ENI/42EV_2Box.xml";
    readXMLFile(ENI_Path.toStdString());
    for(int i = 0; i < Adapter_available.count(); i++)
    {
        Change_NetworkCard(i);
        if(scan_ec_slave(ifbuf) != 0)
        {
            qDebug() << QString(ifbuf);
            qDebug() << QString("Card: %1").arg(i);
            break;
        }
    }
    if(Is_EC_connected == true)
    {
        Is_stop_thread = false;
        std::thread soemThread(start_ec_master);
        soemThread.detach();
        ui->Start_Test_button->setEnabled(false);
        ui->Stop_Test_button->setEnabled(true);
        QTimer::singleShot(1000,this,[=](){
            currentTest = 0;
            logStatusMessage("===============================");
            logStatusMessage("Start Test (selected items: " + list_test.join(", ")+")");
            Run_Next_Test();
        });
    }
    else
    {
        QMessageBox::critical(this,"ERROR","No slave found!");
    }
}

void MainWindow::FormInit()
{
    QPixmap Company_logo(":/img/img/logo.png");
    int height = ui->App_Name->height();
    ui->Company_Logo->setPixmap(Company_logo.scaled(150,80, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    Scan_NetworkCards();
    Timer_HotConnect->start(1000);
    Timer_Check_SlaveState->start(200);
    responseTime42EVTest = new response_time_testing_42ev();
    responseTime42EVTest->setAttribute(Qt::WA_DeleteOnClose);
    responseTime42EVTest->LoadForm();
    // responseTime42EVTest->show();


    // ui->Check_Address_PassIcon->setPixmap(QPixmap(":/img/img/check.png").scaled(16,16,Qt::KeepAspectRatio, Qt::SmoothTransformation));
    // ui->Write_Serial_Number_PassIcon->setPixmap(QPixmap(":/img/img/check.png").scaled(16,16,Qt::KeepAspectRatio, Qt::SmoothTransformation));
    // ui->On_Off_Valve_PassIcon->setPixmap(QPixmap(":/img/img/Error.png").scaled(16,16,Qt::KeepAspectRatio, Qt::SmoothTransformation));
    // ui->Ecat_Out_PassIcon->setPixmap(QPixmap(":/img/img/check.png").scaled(16,16,Qt::KeepAspectRatio, Qt::SmoothTransformation));
    // ui->Test_Status_Icon->setPixmap(QPixmap(":/img/img/check.png").scaled(44,44,Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void MainWindow::Change_NetworkCard(int index)
{
    QString adapter_name = Adapter_available[index];
    QByteArray ba = adapter_name.toUtf8();
    strncpy(ifbuf, ba.constData(), sizeof(ifbuf));
    ifbuf[sizeof(ifbuf) - 1] = '\0';
}

void MainWindow::Scan_NetworkCards()
{
    int num_if = 0;
    ec_adaptert * adapter = NULL;

    adapter = ec_find_adapters();
    while (adapter != NULL)
    {
        QString adapterName = QString(adapter->name);
        QString adapterDesc = QString(adapter->desc);
        QString display = QString("%1 (%2)").arg(adapterDesc, adapterName);
        QString descLower = adapterDesc.toLower();
        QString nameLower = adapterName.toLower();

        if(descLower.contains("vpn") || descLower.contains("wan"))
        {
            adapter = adapter->next;
            continue;
        }
        else
        {
            Adapter_available.append(adapterName);
            printf("Name: %s, Description: %s\n", adapter->name, adapter->desc);
            adapter = adapter->next;
        }
    }
}

void MainWindow::Run_Next_Test()
{
    switch (currentTest)
    {
    case ECAT_IN_TEST:
        if(Run_ECAT_IN_Test() == 0)
        {
            return;
        }
        logStatusMessage("[1. ECAT IN] Starting ECAT IN test ...");
        logStatusMessage("[1. ECAT IN] Test passed.");
        QTimer::singleShot(200,this,[=](){
            currentTest++;
            Run_Next_Test();
        });
        break;

    case ECAT_OUT_TEST:
        Run_ECAT_OUT_Test();
        QTimer::singleShot(200,this,[=](){
            currentTest++;
            Run_Next_Test();
        });
        break;

    case CHECK_ADDRESS_TEST:
        Check_Address_Test();
        QTimer::singleShot(200,this,[=](){
            currentTest++;
            Run_Next_Test();
        });
        break;
    case WRITE_SERIAL_NUMBER_TEST:
        if(ui->Write_Serial_Number_checkBox->isChecked())
        {
            logStatusMessage("[4. Serial Number] Starting Serial Number Test ...");
            disconnect(this,nullptr,this,nullptr);
            connect(this,&MainWindow::finish_WriteSerialNumber,this,[this](FinalTest_Log logs){
                Result_WriteSerialNumber_Test(logs.data["actual"].toString(),logs.data["expect"].toString(),logs.data["is_writed"].toBool());
                currentTest++;
                Run_Next_Test();
            });
            Run_SerialNumber_Test();
        }
        else
        {
            QTimer::singleShot(200,this,[=](){
                currentTest++;
                Run_Next_Test();
            });
        }
        break;
    case ON_OFF_VALVE_TEST:
        if(ui->On_Off_Valve_checkBox->isChecked())
        {
            disconnect(responseTime42EVTest,nullptr,this,nullptr);
            logStatusMessage("[5. On/OFF Valve] Starting On/OFF Valve Test ...");
            responseTime42EVTest->response_time_autoTest->verify_Steps = ui->Cycle_Test_Spin->value();
            connect(responseTime42EVTest,&response_time_testing_42ev::logTest,this,[this](FinalTest_Log logs){
                Result_ResponseTimeTesting(logs.data["status"].toString());
                QTimer::singleShot(1000,this,[this](){
                    currentTest++;
                    Run_Next_Test();
                });
            });
            responseTime42EVTest->RunNormalTest_API();
        }
        else
        {
            QTimer::singleShot(200,this,[=](){
                currentTest++;
                Run_Next_Test();
            });
        }
        break;
    default:
        QString format = "yyyy-MM-dd HH:mm:ss";
        QDateTime endTime = QDateTime::currentDateTime();
        QDateTime startTime = QDateTime::fromString(ui->Start_Test_Time_textBox->text(),format);
        QString timestamp = endTime.toString("yyyy-MM-dd HH:mm:ss");
        ui->End_Test_Time_textBox->setText(timestamp);

        if(startTime.isValid() && endTime.isValid())
        {
            uint64_t totalSeconds = startTime.secsTo(endTime);
            uint64_t hours = totalSeconds / 3600;
            uint64_t minutes = (totalSeconds % 3600) / 60;
            uint64_t seconds = totalSeconds % 60;

            QString elapsTime = QString("%1:%2:%3").arg(hours, 2, 10, QChar('0')).arg(minutes, 2, 10, QChar('0')).arg(seconds,2,10,QChar('0'));
            ui->Test_Duration_textBox->setText(elapsTime);
        }
        if(Is_FinalTest_Pass == true)
        {
            ui->Result_Test_textBox->setText("PASS");
            ui->Test_Status_Icon->setPixmap(QPixmap(":/img/img/check.png").scaled(44,44,Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
        else
        {
            ui->Result_Test_textBox->setText("FAIL");
            ui->Test_Status_Icon->setPixmap(QPixmap(":/img/img/Error.png").scaled(44,44,Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
        break;
    }
}

bool MainWindow::Run_ECAT_IN_Test()
{
    if(Is_ECAT_Init == false)
    {
        QTimer::singleShot(1000,this,[=](){
            currentTest = 0;
            Run_Next_Test();
        });
        return 0;
    }
    ui->Product_Code_1_textBox->setText(QString::fromUtf8(slave_info_xml[0].product_code));
    ui->Vendor_ID_1_textBox->setText(QString::fromUtf8(slave_info_xml[0].vendor_id));
    ui->Serial_Number_1_textBox->setText(QString::fromUtf8(slave_info_xml[0].serial_number));
    ui->Rev_Number_1_textBox->setText(QString::fromUtf8(slave_info[0].revision_number));
    ui->Software_Ver_1_textBox->setText(QString::fromUtf8(slave_info[0].software_ver));
    ui->Hardware_Ver_1_textBox->setText(QString::fromUtf8(slave_info[0].hardware_ver));

    ecx_readstate(&ctx);
    switch (ctx.slavelist[1].state)
    {
    case EC_STATE_INIT:
        ui->State_Slave1_textBox->setText("INIT");
        break;
    case EC_STATE_BOOT:
        ui->State_Slave1_textBox->setText("BOOT");
        break;
    case EC_STATE_PRE_OP:
        ui->State_Slave1_textBox->setText("PRE_OP");
        break;
    case EC_STATE_SAFE_OP:
        ui->State_Slave1_textBox->setText("SAFE_OP");
        break;
    case EC_STATE_OPERATIONAL:
        ui->State_Slave1_textBox->setText("OP");
        break;
    default:
        break;
    }
    if(Is_EC_connected == true)
    {
        ui->Link_State_Slave1_textBox->setText("Good");
    }
    else
    {
        ui->Link_State_Slave1_textBox->setText("Disconnect");
    }

    uint16_t value_register;
    uint8_t Rx_err_0, Rx_err_1;
    uint8_t Forw_err_0,Forw_err_1;
    uint8_t link_lost_0, link_lost_1;

    value_register = ecx_FPRDw(&ctx.port,ctx.slavelist[1].configadr, ECT_REG_RXERR, EC_TIMEOUTRET3);
    Rx_err_0 = (value_register >> 8) & 0xFF;

    value_register = ecx_FPRDw(&ctx.port,ctx.slavelist[1].configadr, ECT_REG_RXERR + 2, EC_TIMEOUTRET3);
    Rx_err_1 = (value_register >> 8) & 0xFF;

    value_register = ecx_FPRDw(&ctx.port,ctx.slavelist[1].configadr, ECT_REG_FRXERR, EC_TIMEOUTRET3);
    Forw_err_0 = value_register & 0xFF;
    Forw_err_1 = (value_register >> 8) & 0xFF;


    value_register = ecx_FPRDw(&ctx.port,ctx.slavelist[1].configadr, ECT_REG_LLCNT, EC_TIMEOUTRET3);
    link_lost_0 = value_register & 0xFF;
    link_lost_1 = (value_register >> 8) & 0xFF;

    ui->Rx_Error_Slave1_textBox->setText(QString::number(Rx_err_0));
    ui->Rx_Error_Slave2_textBox->setText(QString::number(Rx_err_1));


    ui->Forw_Error_Slave1_textBox->setText(QString::number(Forw_err_0));
    ui->Forw_Error_Slave2_textBox->setText(QString::number(Forw_err_1));


    ui->Link_Lost_Slave1_textBox->setText(QString::number(link_lost_0));
    ui->Link_Lost_Slave2_textBox->setText(QString::number(link_lost_1));
    return 1;
}

void MainWindow::Run_ECAT_OUT_Test()
{
    if(ui->Ecat_Out_checkBox->isChecked())
    {
        logStatusMessage("[2. ECAT OUT] Starting ECAT OUT test ...");
        if(ctx.slavecount > 1)
        {
            ui->Product_Code_2_textBox->setText(QString::fromUtf8(slave_info_xml[1].product_code));
            ui->Vendor_ID_2_textBox->setText(QString::fromUtf8(slave_info_xml[1].vendor_id));
            ui->Serial_Number_2_textBox->setText(QString::fromUtf8(slave_info_xml[1].serial_number));
            ui->Rev_Number_2_textBox->setText(QString::fromUtf8(slave_info[1].revision_number));
            ui->Software_Ver_2_textBox->setText(QString::fromUtf8(slave_info[1].software_ver));
            ui->Hardware_Ver_2_textBox->setText(QString::fromUtf8(slave_info[1].hardware_ver));
            ui->Ecat_Out_Icon->setPixmap(QPixmap(":/img/img/check.png").scaled(16,16,Qt::KeepAspectRatio, Qt::SmoothTransformation));
            logStatusMessage("[2. ECAT OUT] Device detected at Slave OUT port.");
            logStatusMessage("[2. ECAT OUT] Test passed.");
        }
        else
        {
            ui->Product_Code_2_textBox->setText("-");
            ui->Vendor_ID_2_textBox->setText("-");
            ui->Serial_Number_2_textBox->setText("-");
            ui->Rev_Number_2_textBox->setText("-");
            ui->Software_Ver_2_textBox->setText("-");
            ui->Hardware_Ver_2_textBox->setText("-");
            ui->Ecat_Out_Icon->setPixmap(QPixmap(":/img/img/Error.png").scaled(16,16,Qt::KeepAspectRatio, Qt::SmoothTransformation));
            logStatusMessage("[2. ECAT OUT] No device detected at Slave OUT port.");
            logStatusMessage("[2. ECAT OUT] Test failed.");
            Is_FinalTest_Pass = false;
        }
    }
}

void MainWindow::Check_Address_Test()
{
    if(ui->Check_Address_checkBox->isChecked())
    {
        logStatusMessage("[3. Check Address] Starting Check Address test ...");
        uint16_t address_actual = ecx_FPRDw(&ctx.port,ctx.slavelist[1].configadr, 0x12, EC_TIMEOUTRET3);
        bool ok;
        uint16_t address_expect = ui->Check_Address_textBox->text().toUInt(&ok, 16);
        logStatusMessage(("[3. Check Address]Switch value expect: 0x" + QString::number(address_expect,16).toUpper() +". Switch value actual: 0x"+ QString::number(address_actual,16).toUpper()));
        if(address_actual != address_expect)
        {
            ui->Check_Address_Icon->setPixmap(QPixmap(":/img/img/Error.png").scaled(16,16,Qt::KeepAspectRatio, Qt::SmoothTransformation));
            logStatusMessage("[3. Check Address] Test failed.");
            Is_FinalTest_Pass = false;
        }
        else
        {
            ui->Check_Address_Icon->setPixmap(QPixmap(":/img/img/check.png").scaled(16,16,Qt::KeepAspectRatio, Qt::SmoothTransformation));
            logStatusMessage("[3. Check Address] Test passed.");
        }
    }
}

void MainWindow::Run_SerialNumber_Test()
{
    SerialNumber_str = ui->Write_SerialNumber_textBox->text();
    if(SerialNumber_str.isEmpty())
    {
        return;
    }
    else
    {
        uint32_t serial_number = SerialNumber_str.toUInt();
        SerialNumber_str = QString("%1").arg(serial_number,8,16,QChar('0'));
    }
    QtConcurrent::run([=](){
        uint8_t data[5];
        data[0] = 0xF9;
        uint8_t buffer[10];
        bool ok_actual, ok_expect;
        int size = sizeof(buffer) - 1;
        processData_mutex.lock();
        int wkc = ecx_SDOread(&ctx,1, 0xF9F0, 00, FALSE, &size,buffer, EC_TIMEOUTRXM);
        processData_mutex.unlock();
        uint32_t Serial_actual = QString::fromLatin1((char*)buffer,9).trimmed().toUInt(&ok_actual,10);
        FinalTest_Log logs;
        if(Serial_actual != 123456789)
        {
            logs.test_name = "Serial Number";
            logs.message = QString("Serial Number is already written and cannot be modified.Serial Number actual: %1").arg(Serial_actual);
            logs.data["is_writed"] = true;
            emit finish_WriteSerialNumber(logs);
            return;
        }
        for(int i = 0; i < SerialNumber_str.length()/2;i++)
        {
            bool ok;
            uint8_t value = SerialNumber_str.mid(i*2,2).toUInt(&ok,16);
            if(!ok)
            {
                return;
            }
            else
            {
                data[i+1] = value;
            }
        }
        processData_mutex.lock();
        ecx_SDOwrite(&ctx,1, 0xFBF0, 01, FALSE,sizeof(data),data,EC_TIMEOUTRXM);
        processData_mutex.unlock();
        bool slave_reconnect = false;
        while(slave_reconnect == false)
        {
            processData_mutex.lock();
            ecx_readstate(&ctx);
            processData_mutex.unlock();
            if(ctx.slavelist[1].state == EC_STATE_OPERATIONAL)
            {
                int size = sizeof(buffer) - 1;
                processData_mutex.lock();
                int wkc = ecx_SDOread(&ctx,1, 0xF9F0, 00, FALSE, &size,buffer, EC_TIMEOUTRXM);
                processData_mutex.unlock();
                if(wkc > 0)
                {
                    slave_reconnect = true;
                }
                QThread::msleep(10);
            }
        }


        Serial_actual = QString::fromLatin1((char*)buffer,9).trimmed().toUInt(&ok_actual,10);
        uint32_t Serial_expect = SerialNumber_str.toUInt(&ok_expect,16);

        logs.test_name = "Serial Number";
        logs.message = QString("Serial Number expect: %1, Serial Number actual: %2").arg(Serial_expect).arg(Serial_actual);
        logs.data["actual"] = Serial_actual;
        logs.data["expect"] = Serial_expect;
        emit finish_WriteSerialNumber(logs);
    });
}

void MainWindow::Run_On_Off_Valve_Test()
{

}

void MainWindow::Result_WriteSerialNumber_Test(QString actual, QString expect, bool Is_writed)
{
    if(Is_writed == true)
    {
        ui->Write_Serial_Number_Icon->setPixmap(QPixmap(":/img/img/Error.png").scaled(16,16,Qt::KeepAspectRatio, Qt::SmoothTransformation));
        logStatusMessage("[4. Serial Number] Serial Number is already written and cannot be modified.");
        Is_FinalTest_Pass = false;
        return;
    }
    if(actual != expect)
    {
        ui->Write_Serial_Number_Icon->setPixmap(QPixmap(":/img/img/Error.png").scaled(16,16,Qt::KeepAspectRatio, Qt::SmoothTransformation));
        Is_FinalTest_Pass = false;
    }
    else
    {
        ui->Write_Serial_Number_Icon->setPixmap(QPixmap(":/img/img/check.png").scaled(16,16,Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    logStatusMessage("[4. Serial Number] Serial Number expect: " + expect +". Serial Number actual: "+ actual);
    logStatusMessage("[4. Serial Number] Finished Serial Number Test");
}

void MainWindow::Result_ResponseTimeTesting(QString status)
{
    if(status == "Pass")
    {
        ui->On_Off_Valve_Icon->setPixmap(QPixmap(":/img/img/check.png").scaled(16,16,Qt::KeepAspectRatio, Qt::SmoothTransformation));
        logStatusMessage("[5. On/OFF Valve] Test passed");
    }
    else
    {
        ui->On_Off_Valve_Icon->setPixmap(QPixmap(":/img/img/Error.png").scaled(16,16,Qt::KeepAspectRatio, Qt::SmoothTransformation));
        logStatusMessage("[5. On/OFF Valve] Test failed.");
        Is_FinalTest_Pass = false;
        // currentTest = NUM_OF_TEST;
        // Is_FinalTest_Pass = false;
    }
}

void MainWindow::Hot_Connect()
{
    if(Is_ECAT_Init)
    {
        ecx_readstate(&ctx);
        for(int i = 1; i <= slave_count;i++)
        {

            uint16 ESC_Status = ecx_FPRDw(&ctx.port,ctx.slavelist[i].configadr, ECT_REG_DLSTAT, EC_TIMEOUTRET3);
            uint8_t Physical_PortA = ESC_Status & (1 << PHYSICAL_LINK_PORTA);
            uint8_t Physical_PortB = ESC_Status & (1 << PHYSICAL_LINK_PORTB);

            if(Physical_PortA == 0)
            {
                for(int j = i; j <= slave_count && slave_disconnect[j] == false; j++)
                {
                    logStatusMessage(QString("Slave %1: Communication interrupted").arg(j));
                    slave_disconnect[j] = true;
                }
                rescan_slave = true;
                break;
            }
        }

        if(rescan_slave)
        {
            for(int i = 1 ; i <= slave_count; i++)
            {
                if(slave_disconnect[i] == true)
                {
                    ecx_recover_slave(&ctx,i, EC_TIMEOUTRET);
                    uint16 ESC_Status = ecx_FPRDw(&ctx.port,ctx.slavelist[i].configadr, ECT_REG_DLSTAT, EC_TIMEOUTRET3);
                    uint8_t Physical_PortA = ESC_Status & (1 << PHYSICAL_LINK_PORTA);
                    uint8_t Physical_PortB = ESC_Status & (1 << PHYSICAL_LINK_PORTB);

                    if(Physical_PortA && slave_reconnecting == false)
                    {
                        slave_reconnecting = true;
                        qDebug()<<QString("Time %1 - Slave %2").arg(EC_LINKDETECT_PORTB_TIMEOUT[i]).arg(i);
                        QTimer::singleShot(EC_LINKDETECT_PORTB_TIMEOUT[i - 1],this,[=]()
                                           {
                                               uint8_t slave_connect;
                                               for(int j = 0; j <= slave_count;j++)
                                               {
                                                   if(slave_disconnect[j] == true)
                                                   {
                                                       slave_connect = j;
                                                       break;
                                                   }
                                               }

                                               slave_disconnect[slave_connect] = false;
                                               logStatusMessage(QString("Slave %1: Communication re-established").arg(slave_connect));
                                               ecx_reconfig_slave(&ctx,slave_connect, EC_TIMEOUTRET);
                                               ctx.slavelist[slave_connect].state = EC_STATE_INIT;
                                               ecx_writestate(&ctx,slave_connect);
                                               ecx_statecheck(&ctx,slave_connect, EC_STATE_INIT, EC_TIMEOUTSTATE);
                                               ctx.slavelist[slave_connect].state = EC_STATE_PRE_OP;
                                               ecx_writestate(&ctx,slave_connect);
                                               ecx_statecheck(&ctx,slave_connect, EC_STATE_PRE_OP, EC_TIMEOUTSTATE);
                                               PDO_mapping(slave_connect);
                                               switch(slave_state_request[slave_connect])
                                               {
                                               case EC_STATE_INIT:
                                                   ctx.slavelist[slave_connect].state = EC_STATE_INIT;
                                                   ecx_writestate(&ctx,slave_connect);
                                                   ecx_statecheck(&ctx,slave_connect, EC_STATE_INIT, EC_TIMEOUTSTATE);
                                                   break;
                                               case EC_STATE_PRE_OP:
                                                   ctx.slavelist[slave_connect].state = EC_STATE_INIT;
                                                   ecx_writestate(&ctx,slave_connect);
                                                   ecx_statecheck(&ctx,slave_connect, EC_STATE_INIT, EC_TIMEOUTSTATE);
                                                   ctx.slavelist[slave_connect].state = EC_STATE_PRE_OP;
                                                   ecx_writestate(&ctx,slave_connect);
                                                   ecx_statecheck(&ctx,slave_connect, EC_STATE_PRE_OP, EC_TIMEOUTSTATE);
                                                   break;
                                               case EC_STATE_SAFE_OP:
                                                   ctx.slavelist[slave_connect].state = EC_STATE_INIT;
                                                   ecx_writestate(&ctx,slave_connect);
                                                   ecx_statecheck(&ctx,slave_connect, EC_STATE_INIT, EC_TIMEOUTSTATE);
                                                   ctx.slavelist[slave_connect].state = EC_STATE_PRE_OP;
                                                   ecx_writestate(&ctx,slave_connect);
                                                   ecx_statecheck(&ctx,slave_connect, EC_STATE_PRE_OP, EC_TIMEOUTSTATE);
                                                   ctx.slavelist[slave_connect].state = EC_STATE_SAFE_OP;
                                                   ecx_writestate(&ctx,slave_connect);
                                                   ecx_statecheck(&ctx,slave_connect, EC_STATE_SAFE_OP, EC_TIMEOUTSTATE);
                                                   break;
                                               case EC_STATE_OPERATIONAL:
                                                   ctx.slavelist[slave_connect].state = EC_STATE_INIT;
                                                   ecx_writestate(&ctx,slave_connect);
                                                   ecx_statecheck(&ctx,slave_connect, EC_STATE_INIT, EC_TIMEOUTSTATE);
                                                   ctx.slavelist[slave_connect].state = EC_STATE_PRE_OP;
                                                   ecx_writestate(&ctx,slave_connect);
                                                   ecx_statecheck(&ctx,slave_connect, EC_STATE_PRE_OP, EC_TIMEOUTSTATE);
                                                   ctx.slavelist[slave_connect].state = EC_STATE_SAFE_OP;
                                                   ecx_writestate(&ctx,slave_connect);
                                                   ecx_statecheck(&ctx,slave_connect, EC_STATE_SAFE_OP, EC_TIMEOUTSTATE);
                                                   ctx.slavelist[slave_connect].state = EC_STATE_OPERATIONAL;
                                                   ecx_writestate(&ctx,slave_connect);
                                                   ecx_statecheck(&ctx,slave_connect, EC_STATE_OPERATIONAL, EC_TIMEOUTSTATE);
                                                   break;
                                               case EC_STATE_BOOT:
                                                   ctx.slavelist[slave_connect].state = EC_STATE_INIT;
                                                   ecx_writestate(&ctx,slave_connect);
                                                   ecx_statecheck(&ctx,slave_connect, EC_STATE_INIT, EC_TIMEOUTSTATE);
                                                   ctx.slavelist[slave_connect].state = EC_STATE_BOOT;
                                                   ecx_writestate(&ctx,slave_connect);
                                                   ecx_statecheck(&ctx,slave_connect, EC_STATE_BOOT, EC_TIMEOUTSTATE);
                                               }
                                               si_sdo(slave_connect);
                                               slave_connect++;
                                               while(slave_connect <= slave_count)
                                               {
                                                   uint16 ESC_Status = ecx_FPRDw(&ctx.port,ctx.slavelist[slave_connect].configadr, ECT_REG_DLSTAT, EC_TIMEOUTRET3);
                                                   uint8 Physical_PortA = ESC_Status & (1 << PHYSICAL_LINK_PORTA);
                                                   uint8 Physical_PortB = ESC_Status & (1 << PHYSICAL_LINK_PORTB);
                                                   if(Physical_PortA)
                                                   {
                                                       slave_disconnect[slave_connect] = false;
                                                       logStatusMessage(QString("Slave %1: Communication re-established").arg(slave_connect));
                                                       ecx_reconfig_slave(&ctx,slave_connect, EC_TIMEOUTRET);
                                                       ctx.slavelist[slave_connect].state = EC_STATE_INIT;
                                                       ecx_writestate(&ctx,slave_connect);
                                                       ecx_statecheck(&ctx,slave_connect, EC_STATE_INIT, EC_TIMEOUTSTATE);
                                                       ctx.slavelist[slave_connect].state = EC_STATE_PRE_OP;
                                                       ecx_writestate(&ctx,slave_connect);
                                                       ecx_statecheck(&ctx,slave_connect, EC_STATE_PRE_OP, EC_TIMEOUTSTATE);
                                                       PDO_mapping(slave_connect);
                                                       switch(slave_state_request[slave_connect])
                                                       {
                                                       case EC_STATE_INIT:
                                                           ctx.slavelist[slave_connect].state = EC_STATE_INIT;
                                                           ecx_writestate(&ctx,slave_connect);
                                                           ecx_statecheck(&ctx,slave_connect, EC_STATE_INIT, EC_TIMEOUTSTATE);
                                                           break;
                                                       case EC_STATE_PRE_OP:
                                                           ctx.slavelist[slave_connect].state = EC_STATE_INIT;
                                                           ecx_writestate(&ctx,slave_connect);
                                                           ecx_statecheck(&ctx,slave_connect, EC_STATE_INIT, EC_TIMEOUTSTATE);
                                                           ctx.slavelist[slave_connect].state = EC_STATE_PRE_OP;
                                                           ecx_writestate(&ctx,slave_connect);
                                                           ecx_statecheck(&ctx,slave_connect, EC_STATE_PRE_OP, EC_TIMEOUTSTATE);
                                                           break;
                                                       case EC_STATE_SAFE_OP:
                                                           ctx.slavelist[slave_connect].state = EC_STATE_INIT;
                                                           ecx_writestate(&ctx,slave_connect);
                                                           ecx_statecheck(&ctx,slave_connect, EC_STATE_INIT, EC_TIMEOUTSTATE);
                                                           ctx.slavelist[slave_connect].state = EC_STATE_PRE_OP;
                                                           ecx_writestate(&ctx,slave_connect);
                                                           ecx_statecheck(&ctx,slave_connect, EC_STATE_PRE_OP, EC_TIMEOUTSTATE);
                                                           ctx.slavelist[slave_connect].state = EC_STATE_SAFE_OP;
                                                           ecx_writestate(&ctx,slave_connect);
                                                           ecx_statecheck(&ctx,slave_connect, EC_STATE_SAFE_OP, EC_TIMEOUTSTATE);
                                                           break;
                                                       case EC_STATE_OPERATIONAL:
                                                           ctx.slavelist[slave_connect].state = EC_STATE_INIT;
                                                           ecx_writestate(&ctx,slave_connect);
                                                           ecx_statecheck(&ctx,slave_connect, EC_STATE_INIT, EC_TIMEOUTSTATE);
                                                           ctx.slavelist[slave_connect].state = EC_STATE_PRE_OP;
                                                           ecx_writestate(&ctx,slave_connect);
                                                           ecx_statecheck(&ctx,slave_connect, EC_STATE_PRE_OP, EC_TIMEOUTSTATE);
                                                           ctx.slavelist[slave_connect].state = EC_STATE_SAFE_OP;
                                                           ecx_writestate(&ctx,slave_connect);
                                                           ecx_statecheck(&ctx,slave_connect, EC_STATE_SAFE_OP, EC_TIMEOUTSTATE);
                                                           ctx.slavelist[slave_connect].state = EC_STATE_OPERATIONAL;
                                                           ecx_writestate(&ctx,slave_connect);
                                                           ecx_statecheck(&ctx,slave_connect, EC_STATE_OPERATIONAL, EC_TIMEOUTSTATE);
                                                           break;
                                                       case EC_STATE_BOOT:
                                                           ctx.slavelist[slave_connect].state = EC_STATE_INIT;
                                                           ecx_writestate(&ctx,slave_connect);
                                                           ecx_statecheck(&ctx,slave_connect, EC_STATE_INIT, EC_TIMEOUTSTATE);
                                                           ctx.slavelist[slave_connect].state = EC_STATE_BOOT;
                                                           ecx_writestate(&ctx,slave_connect);
                                                           ecx_statecheck(&ctx,slave_connect, EC_STATE_BOOT, EC_TIMEOUTSTATE);
                                                       }
                                                       si_sdo(slave_connect);
                                                       slave_connect++;
                                                   }
                                               }
                                               slave_reconnecting = false;
                                           }
                                           );
                    }
                    // ui->DeviceName_label->setText(QString::fromUtf8(slave_info[Slave_select - 1].device_name));
                    // ui->SoftwareVer_label->setText(QString::fromUtf8(slave_info[Slave_select - 1].software_ver));
                    // ui->HardwareVer_label->setText(QString::fromUtf8(slave_info[Slave_select - 1].hardware_ver));
                    // ui->VendorName_label->setText(QString::fromUtf8(slave_info[Slave_select - 1].vendor_name));
                    // ui->VendorID_label->setText(QString::fromUtf8(slave_info_xml[Slave_select - 1].vendor_id));
                    // ui->ProductCode_label->setText(QString::fromUtf8(slave_info_xml[Slave_select - 1].product_code));
                    // ui->SerialNumber_label->setText(QString::fromUtf8(slave_info_xml[Slave_select - 1].serial_number));
                    // ui->EtherCAT_Addr_spinBox->setValue(Slave_AddressInfo[Slave_select].Physical_Address);
                    // ui->Identification_spinBox->setValue(Slave_AddressInfo[Slave_select].Identification);
                    rescan_slave = false;
                }
            }
            for(int i = 1; i <= slave_count;i++)
            {
                if(slave_disconnect[i] == true)
                {
                    rescan_slave = true;
                }
            }
        }
    }
}

void MainWindow::Check_SlaveState()
{
    if(Is_ECAT_Init == true)
    {
        ecx_readstate(&ctx);
        switch (ctx.slavelist[1].state)
        {
        case EC_STATE_INIT:
            ui->State_Slave1_textBox->setText("INIT");
            break;
        case EC_STATE_BOOT:
            ui->State_Slave1_textBox->setText("BOOT");
            break;
        case EC_STATE_PRE_OP:
            ui->State_Slave1_textBox->setText("PRE_OP");
            break;
        case EC_STATE_SAFE_OP:
            ui->State_Slave1_textBox->setText("SAFE_OP");
            break;
        case EC_STATE_OPERATIONAL:
            ui->State_Slave1_textBox->setText("OP");
            break;
        case EC_STATE_NONE:
            ui->State_Slave1_textBox->setText("INIT");
            break;
        default:
            break;
        }
        ui->Rx_Tx_Frame_Slave1_textBox->setText(QString("%1/ %2").arg(Rx_Frames).arg(Tx_Frames));
        ui->AL_Status_Code_Slave1_textBox->setText(QString::number(ctx.slavelist[1].ALstatuscode));
        if(Is_EC_connected == true)
        {
            ui->Link_State_Slave1_textBox->setText("Good");
        }
        else
        {
            ui->Link_State_Slave1_textBox->setText("Disconnect");
        }

        if(ctx.slavecount >1)
        {
            ui->AL_Status_Code_Slave2_textBox->setText(QString::number(ctx.slavelist[2].ALstatuscode));
            switch (ctx.slavelist[2].state)
            {
            case EC_STATE_INIT:
                ui->State_Slave2_textBox->setText("INIT");
                break;
            case EC_STATE_BOOT:
                ui->State_Slave2_textBox->setText("BOOT");
                break;
            case EC_STATE_PRE_OP:
                ui->State_Slave2_textBox->setText("PRE_OP");
                break;
            case EC_STATE_SAFE_OP:
                ui->State_Slave2_textBox->setText("SAFE_OP");
                break;
            case EC_STATE_OPERATIONAL:
                ui->State_Slave2_textBox->setText("OP");
                break;
            case EC_STATE_NONE:
                ui->State_Slave2_textBox->setText("INIT");
                break;
            default:
                break;
            }
            if(Is_EC_connected == true)
            {
                ui->Link_State_Slave2_textBox->setText("Good");
            }
            else
            {
                ui->Link_State_Slave2_textBox->setText("Disconnected");
            }
            ui->Rx_Tx_Frame_Slave2_textBox->setText(QString("%1/ %2").arg(Rx_Frames).arg(Tx_Frames));
        }
        else
        {
            ui->AL_Status_Code_Slave2_textBox->setText("-");
            ui->State_Slave2_textBox->setText("-");
            ui->Link_State_Slave2_textBox->setText("Disconnected");
            ui->Rx_Tx_Frame_Slave2_textBox->setText("-");
        }

        uint16_t value_register;
        uint8_t Rx_err_0, Rx_err_1;
        uint8_t Forw_err_0,Forw_err_1;
        uint8_t link_lost_0, link_lost_1;

        value_register = ecx_FPRDw(&ctx.port,ctx.slavelist[1].configadr, ECT_REG_RXERR, EC_TIMEOUTRET3);
        Rx_err_0 = (value_register >> 8) & 0xFF;

        value_register = ecx_FPRDw(&ctx.port,ctx.slavelist[1].configadr, ECT_REG_RXERR + 2, EC_TIMEOUTRET3);
        Rx_err_1 = (value_register >> 8) & 0xFF;

        value_register = ecx_FPRDw(&ctx.port,ctx.slavelist[1].configadr, ECT_REG_FRXERR, EC_TIMEOUTRET3);
        Forw_err_0 = value_register & 0xFF;
        Forw_err_1 = (value_register >> 8) & 0xFF;


        value_register = ecx_FPRDw(&ctx.port,ctx.slavelist[1].configadr, ECT_REG_LLCNT, EC_TIMEOUTRET3);
        link_lost_0 = value_register & 0xFF;
        link_lost_1 = (value_register >> 8) & 0xFF;

        ui->Rx_Error_Slave1_textBox->setText(QString::number(Rx_err_0));
        ui->Rx_Error_Slave2_textBox->setText(QString::number(Rx_err_1));


        ui->Forw_Error_Slave1_textBox->setText(QString::number(Forw_err_0));
        ui->Forw_Error_Slave2_textBox->setText(QString::number(Forw_err_1));


        ui->Link_Lost_Slave1_textBox->setText(QString::number(link_lost_0));
        ui->Link_Lost_Slave2_textBox->setText(QString::number(link_lost_1));
    }
}

void MainWindow::logStatusMessage(QString msg)
{
    QString timestamp = QDateTime::currentDateTime().toString("HH:mm:ss");
    QString status;
    QString message = QString("%1 |     %2").arg(timestamp).arg(msg);
    logBuffer.append(message);
    ui->Log_textBox->appendPlainText(message);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    responseTime42EVTest->close();
}


void MainWindow::on_Stop_Test_button_clicked()
{
    ui->Start_Test_button->setEnabled(true);
    ui->Stop_Test_button->setEnabled(false);
    if (Is_ECAT_Init == true)
    {
        ctx.slavelist[0].state = EC_STATE_INIT;
        ecx_writestate(&ctx,0);
        ecx_statecheck(&ctx,0,EC_STATE_INIT,EC_TIMEOUTSTATE);

        for(int i = 1; i <= ctx.slavecount; i++)
        {
            slave_state_request[i] = EC_STATE_INIT;
        }
    }
    Is_stop_thread = true;
    Is_ECAT_Init = false;
    // responseTime42EVTest->close();
    ui->Result_Test_textBox->setText("READY");
    ui->Test_Status_Icon->clear();
}


void MainWindow::on_Clear_Log_button_clicked()
{
    logBuffer.clear();
    ui->Log_textBox->clear();
}


void MainWindow::on_On_Valve_Button_clicked()
{
    processData_mutex.lock();
    IOmap[PDO_mapping_output[0].address[0]] = 0xFF;
    IOmap[PDO_mapping_output[0].address[0] + 1] = 0xFF;
    IOmap[PDO_mapping_output[0].address[0] + 2] = 0xFF;
    IOmap[PDO_mapping_output[0].address[0] + 3] = 0xFF;
    IOmap[PDO_mapping_output[0].address[0] + 4] = 0xFF;
    IOmap[PDO_mapping_output[0].address[0] + 5] = 0xFF;
    processData_mutex.unlock();
}


void MainWindow::on_Off_Valve_Button_clicked()
{
    processData_mutex.lock();
    IOmap[PDO_mapping_output[0].address[0]] = 0;
    IOmap[PDO_mapping_output[0].address[0] + 1] = 0;
    IOmap[PDO_mapping_output[0].address[0] + 2] = 0;
    IOmap[PDO_mapping_output[0].address[0] + 3] = 0;
    IOmap[PDO_mapping_output[0].address[0] + 4] = 0;
    IOmap[PDO_mapping_output[0].address[0] + 5] = 0;
    processData_mutex.unlock();
}

