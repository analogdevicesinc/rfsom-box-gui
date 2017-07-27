#include "mainwindow.h"
#include <QApplication>
#include <QFont>
#include <QFontDatabase>


int main(int argc, char *argv[])
{
	//QFontDatabase::addApplicationFont(":/fonts/font/dejavu-fonts-ttf-2.37/ttf/DejaVuSans.ttf");

	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	return a.exec();
}
