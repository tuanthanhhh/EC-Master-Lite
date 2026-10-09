#ifndef RESPONSE_TIME_TESTING_42EV_H
#define RESPONSE_TIME_TESTING_42EV_H

#include <QWidget>
#include <QDialog>
#include "data_struct.h"
#include <QCloseEvent>
#include <QTimer>
#include <stdint.h>
#include <stdio.h>
#include <QRegularExpression>
#include <QVector>
#include <mutex>

#define VALVE_ON        0x01
#define VALVE_OFF       0x00
namespace Ui {
class response_time_testing_42ev;
}

class response_time_testing_42ev : public QWidget
{
    Q_OBJECT

public:
    explicit response_time_testing_42ev(QWidget *parent = nullptr);
    ~response_time_testing_42ev();

private slots:
    void on_slave_comboBox_1_currentIndexChanged(int index);

    void on_slave_comboBox_2_currentIndexChanged(int index);

    void on_slave_comboBox_3_currentIndexChanged(int index);

    void on_Start_Test_button_clicked();

    void on_Next_Step_textBox_editingFinished();

    void on_Max_Response_textBox_editingFinished();

    void on_Verify_Step_textBox_editingFinished();

    void on_Valve_Test_textBox_editingFinished();

    void on_Valve_Test_textBox_textChanged(const QString &arg1);

    void on_pushButton_clicked();

    void RunIOTest();

    void RunRandomTest();

    void RunManualTest();

    void on_Manual_radioButton_toggled(bool checked);

private:
    Ui::response_time_testing_42ev *ui;

private:
    int PDO_ValveControl_1 = 0;
    int PDO_ValveReadback_1 = 0;
    int PDO_ValveControl_2 = 0;
    int PDO_ValveReadback_2 = 0;
    int PDO_ValveControl_3 = 0;
    int PDO_ValveReadback_3 = 0;


    int PDO_ValveControl_Address_1 = 0;
    int PDO_ValveReadback_Address_1 = 0;
    int PDO_ValveControl_Address_2 = 0;
    int PDO_ValveReadback_Address_2 = 0;
    int PDO_ValveControl_Address_3 = 0;
    int PDO_ValveReadback_Address_3 = 0;


    bool Valve_State = VALVE_OFF;
    int max_response_time;
    int slave1_test;
    int slave2_test;
    int slave3_test;
    int step = 0;
    int valve_random[42] = {0};

private:
    void Read_Valve();
    void display_ValveReadback();
    void display_ValveControl();
    bool Is_Start_Test= false;
    bool Is_Test_Fail = false;
    bool slaveTest_valid_format = false;
    bool Is_checking_ValveChange = false;
    bool Complete_Stop_Check_1 = false;
    bool Complete_Stop_Check_2 = false;
    bool Complete_Stop_Check_3 = false;
    int bit_change;
    float Min_Response_OnTime_1 = 0;
    float Min_Response_OnTime_2 = 0;
    float Min_Response_OnTime_3 = 0;
    float Min_Response_OffTime_1 = 0;
    float Min_Response_OffTime_2 = 0;
    float Min_Response_OffTime_3 = 0;

    float Max_Response_OnTime_1 = 0;
    float Max_Response_OnTime_2 = 0;
    float Max_Response_OnTime_3 = 0;
    float Max_Response_OffTime_1 = 0;
    float Max_Response_OffTime_2 = 0;
    float Max_Response_OffTime_3 = 0;
    QStringList logBuffer;

public:
    Response_Time_AutoTest* response_time_autoTest = new Response_Time_AutoTest();
    Response_Time_IO* response_time_io_1 = new Response_Time_IO();
    Response_Time_IO* response_time_io_2 = new Response_Time_IO();
    Response_Time_IO* response_time_io_3 = new Response_Time_IO();

private:
    void logStatus(QString msg);
    void updateStartTestButton(QString msg);
    void resetTableWidget();
private:
    void check_time_response(int valve, int valve_bit);
    void check_time_response_random(int numValveRandom);
    void check_time_response_manual(int numValve_Manual);
    bool Check_ValveTest_String(QString str);

private:
    QVector<int> valve_manual_test;
    QVector<int> Valve_Change_State_cnt;
    std::mutex dataMutex;

private:
    QTimer *Timer_Test;
    QTimer *Timer_RandomTest;
    QTimer *Timer_ManualTest;

private:
    void LoadParamConfig();
    void SaveParamConfig();

public:
    void LoadForm();
    void RunAllTest_API();
    void RunNormalTest_API();
    void RunManualTest_API();
    void RunRandomTest_API();

protected:
    void closeEvent(QCloseEvent *event) override ;

signals:
    void logTest(FinalTest_Log log);
};

#endif // RESPONSE_TIME_TESTING_42EV_H
