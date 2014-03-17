#include "progressform.h"
#include "ui_progressform.h"

#include <iostream>
using namespace std;

ProgressForm::ProgressForm(QWidget *parent) :
    QWidget(parent),
    timer(new QTimer(this)),
    ui(new Ui::ProgressForm)
{
    ui->setupUi(this);
    time.start();
    timer->setInterval(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTimeLabels()));
    timer->start();


    QTime t(0, 0);
    ui->timeLbl->setText(t.toString());
    ui->etaLbl->setText(t.toString());
}

ProgressForm::~ProgressForm()
{
    delete ui;
}

void ProgressForm::updateProgress(float progress)
{
    ui->progressBar->setValue(progress * 100);
}

void ProgressForm::updateTimeLabels()
{
    QTime t(0, 0);
    t = t.addMSecs(time.elapsed());
    ui->timeLbl->setText(t.toString());
    if(ui->progressBar->value())
    {
        t = QTime(0, 0);
        t = t.addMSecs(time.elapsed() * 100 / ui->progressBar->value() * (1.0 - ui->progressBar->value() / 100.0));
        ui->etaLbl->setText(t.toString());
    }
    else
    {
        ui->etaLbl->setText("--:--:--");
    }
}

void ProgressForm::finish()
{
    timer->stop();
    QTime t(0, 0);
    t = t.addMSecs(time.elapsed());
    ui->timeLbl->setText(t.toString());
    ui->etaLbl->setText("Done!");
}
