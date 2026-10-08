#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "ec_info.h"
#include "ec_control.h"
#include <QStandardPaths>
#include <QMessageBox>
#include "data_struct.h"
#include <QTimer>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Start_Test_button_clicked()
{
    QString Local_Path = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    QString ENI_Path = Local_Path + "/ENI/42EV.xml";
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
        if(Is_ECAT_Init == false)
        {
            QTimer::singleShot(1000,this,[=](){
                currentTest = 0;
                Run_Next_Test();
            });
            return;
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

        QTimer::singleShot(200,this,[=](){
            currentTest++;
            Run_Next_Test();
        });
        break;
    case ECAT_OUT_TEST:
        if(ui->Ecat_Out_checkBox->isChecked())
        {
            if(ctx.slavecount > 1)
            {
                ui->Product_Code_2_textBox->setText(QString::fromUtf8(slave_info_xml[1].product_code));
                ui->Vendor_ID_2_textBox->setText(QString::fromUtf8(slave_info_xml[1].vendor_id));
                ui->Serial_Number_2_textBox->setText(QString::fromUtf8(slave_info_xml[1].serial_number));
                ui->Rev_Number_2_textBox->setText(QString::fromUtf8(slave_info[1].revision_number));
                ui->Software_Ver_2_textBox->setText(QString::fromUtf8(slave_info[1].software_ver));
                ui->Hardware_Ver_2_textBox->setText(QString::fromUtf8(slave_info[1].hardware_ver));
                ui->Ecat_Out_Icon->setPixmap(QPixmap(":/img/img/check.png").scaled(16,16,Qt::KeepAspectRatio, Qt::SmoothTransformation));
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
            }
        }
        QTimer::singleShot(200,this,[=](){
            currentTest++;
            Run_Next_Test();
        });
        break;
    case CHECK_ADDRESS_TEST:
        if(ui->Check_Address_checkBox->isChecked())
        {
            uint16_t address_actual = ecx_FPRDw(&ctx.port,ctx.slavelist[1].configadr, 0x12, EC_TIMEOUTRET3);
            bool ok;
            uint16_t address_expect = ui->Check_Address_textBox->text().toUInt(&ok, 16);
            if(address_actual != address_expect)
            {
                ui->Check_Address_Icon->setPixmap(QPixmap(":/img/img/Error.png").scaled(16,16,Qt::KeepAspectRatio, Qt::SmoothTransformation));
            }
            else
            {
                ui->Check_Address_Icon->setPixmap(QPixmap(":/img/img/check.png").scaled(16,16,Qt::KeepAspectRatio, Qt::SmoothTransformation));
            }
        }
        QTimer::singleShot(200,this,[=](){
            currentTest=9;
            Run_Next_Test();
        });
        break;
    case WRITE_SERIAL_NUMBER_TEST:
        break;
    case ON_OFF_VALVE_TEST:
        break;
    default:
        break;
    }
}

void MainWindow::Check_Address_Task()
{

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
}

