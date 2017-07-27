#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtDebug>
#include <QKeyEvent>
#include <QWheelEvent>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	QApplication::setOverrideCursor(Qt::BlankCursor);
	ui->setupUi(this);
	ui->main_menu->initialize(ui);
	//ui->main_menu->loadLandingPage();

}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
//	ui->main_menu->inputHandling(e);
	qDebug()<<"mainWindow";
	QMainWindow::keyPressEvent(e);
}

/*void MainWindow::wheelEvent(QWheelEvent *e)
{
//	ui->main_menu->inputHandling(e);
	qDebug()<<"mainwindow" << e;
	QMainWindow::wheelEvent(e);
}*/

MainWindow::~MainWindow()
{
	delete ui;
}

