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
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
	QMainWindow::keyPressEvent(e);
}

MainWindow::~MainWindow()
{
	delete ui;
}

