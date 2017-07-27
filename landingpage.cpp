#include "landingpage.h"
#include <QDebug>
#include <QFile>
#include "ui_mainwindow.h"

LandingPage::LandingPage(QWidget *parent) : MenuPage(parent)
{
}

void LandingPage::load()
{
	setFocus();
	refreshTimer->start(250);
}

void LandingPage::unload()
{
	qInfo()<<"unload";
	refreshTimer->stop();
}

void LandingPage::initialize()
{
	rssi = new SysFSValue(rssiPath,this);
	network = new SysFSValue(networkPath,this);
	temperature = new SysFSValue(temperaturePath,this);
	charge = new SysFSValue(chargePath,this);
	refreshTimer = new QTimer(this);
	connect(refreshTimer,SIGNAL(timeout()),this,SLOT(updateUi()));
	refreshTimer->start(250);
}

void LandingPage::keyPressEvent(QKeyEvent *e)
{
	qDebug()<<"landingpage";
	MenuPage::keyPressEvent(e);
}

void LandingPage::updateUi()
{
	ui->labelRadio->setText(rssi->get());
	ui->labelNetwork->setText(network->get());
	ui->labelCharge->setText(charge->get());
	ui->labelTemp->setText(temperature->get());
}
