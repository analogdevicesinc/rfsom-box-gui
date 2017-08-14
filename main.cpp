#include "mainwindow.h"
#include <QApplication>
#include <QFont>
#include <QFontDatabase>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	int id = QFontDatabase::addApplicationFont(":/font/dejavu-fonts-ttf-2.37/ttf/DejaVuSansMono.ttf");
	QString family = QFontDatabase::applicationFontFamilies(id).at(0);
	QFont monospace(family);
	monospace.setPointSize(10);

	QApplication::setFont(monospace);
	MainWindow w;
	w.show();
	return a.exec();
}
