#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "data_struct.h"
#include "response_time_testing_42ev.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_Start_Test_button_clicked();

    void on_Stop_Test_button_clicked();

    void Hot_Connect();

    void Check_SlaveState();

    void on_Clear_Log_button_clicked();

    void on_On_Valve_Button_clicked();

    void on_Off_Valve_Button_clicked();

private:
    Ui::MainWindow *ui;

public:
    void FormInit();

private:
    void Change_NetworkCard(int index);
    void Scan_NetworkCards();

private:
    uint8_t currentTest;
    QString SerialNumber_str;
    bool Is_FinalTest_Pass;
    void Run_Next_Test();
    bool Run_ECAT_IN_Test();
    void Run_ECAT_OUT_Test();
    void Check_Address_Test();
    void Run_SerialNumber_Test();
    void Run_On_Off_Valve_Test();
    void Result_WriteSerialNumber_Test(QString actual, QString expect, bool Is_writed);
    void Result_ResponseTimeTesting(QString status);

private:
    response_time_testing_42ev *responseTime42EVTest;

private:
    QTimer *Timer_HotConnect;
    QTimer *Timer_Check_SlaveState;

private:
    void logStatusMessage(QString msg);
    QStringList logBuffer;

signals:
    void finish_WriteSerialNumber(FinalTest_Log logs);

protected:
    void closeEvent(QCloseEvent *event) override;

};
#endif // MAINWINDOW_H
