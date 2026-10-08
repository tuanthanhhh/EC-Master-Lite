#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPixmap Company_logo(":/img/img/logo.png");
    int height = ui->App_Name->height();
    ui->Company_Logo->setPixmap(Company_logo.scaled(150,80, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->Check_Address_PassIcon->setPixmap(QPixmap(":/img/img/check.png").scaled(16,16,Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->Write_Serial_Number_PassIcon->setPixmap(QPixmap(":/img/img/check.png").scaled(16,16,Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->On_Off_Valve_PassIcon->setPixmap(QPixmap(":/img/img/Error.png").scaled(16,16,Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->Ecat_Out_PassIcon->setPixmap(QPixmap(":/img/img/check.png").scaled(16,16,Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

MainWindow::~MainWindow()
{
    delete ui;
}
