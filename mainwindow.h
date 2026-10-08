#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

private:
    Ui::MainWindow *ui;

public:
    void FormInit();

private:
    void Change_NetworkCard(int index);
    void Scan_NetworkCards();

private:
    uint8_t currentTest;
    void Run_Next_Test();
    void Check_Address_Task();
};
#endif // MAINWINDOW_H
