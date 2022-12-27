#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>
#include <QDebug>

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


void MainWindow::on_pushButton_clicked()
{
    QString selfilter = tr("XLS (*.xls)");
    this->file_1 = QFileDialog::getOpenFileName(this, "Выбрать файл", "C://", tr("All files (*.*);;XLS (*.xls)"), &selfilter);
    ui->label->setText(this->file_1);
}


void MainWindow::on_pushButton_2_clicked()
{
    QString selfilter = tr("XLS (*.xls)");
    this->file_2 = QFileDialog::getOpenFileName(this, "Выбрать файл", "C://", tr("All files (*.*);;XLS (*.xls)"), &selfilter);
    ui->label_2->setText(this->file_2);
}


void MainWindow::on_spinBox_valueChanged(int arg1)
{
    this->n_best = arg1;
}


void MainWindow::on_spinBox_2_valueChanged(int arg1)
{
    this->n_gen = arg1;
}

void MainWindow::on_pushButton_3_clicked()
{
    if (!this->file_1.isEmpty() && !this->file_2.isEmpty()) {
        QStringList params;
        QProcess *appProcess = new QProcess();
            appProcess->setStandardOutputFile("Результат.txt");
            appProcess->setProcessChannelMode(QProcess::MergedChannels);
            connect(appProcess, &QProcess::started, [appProcess]() {
                qDebug() << "Started!";
                qDebug() << "PID: " << appProcess->processId();
                qDebug() << "Process state: " << appProcess->state();
            });
            connect(appProcess, qOverload<int, QProcess::ExitStatus>(&QProcess::finished), [appProcess, this](int exitCode, QProcess::ExitStatus exitStatus) {
                qDebug() << exitCode;
                qDebug() << exitStatus;
                if (exitCode == 0) {
                    QMessageBox::information(this, "Готово!", "Распределение сохранено в файл.");
                } else {
                    QMessageBox::critical(this, "Ошибка", "При создании распределения произошла ошибка.\nОписание сохранено в файл.");
                }
                appProcess->deleteLater(); //clear the QProcess object
            });
            params << "--form" << this->file_1 << "--profiles" << this->file_2 << "--n_best" << QString::number(this->n_best) << "--n_gen" << QString::number(this->n_gen);
            appProcess->start("main.exe", params);
            appProcess->waitForFinished();
            QString output(appProcess->readAllStandardOutput());
            qDebug() << output;
    } else if (this->file_1.isEmpty()) {
        QMessageBox::critical(this, "Ошибка", "Файл с формой не выбран!");
    } else if (this->file_2.isEmpty()) {
        QMessageBox::critical(this, "Ошибка", "Файл с направлениями не выбран!");
    } else {
        QMessageBox::critical(this, "Ошибка", "Неизвестная ошибка!");
    }
}

