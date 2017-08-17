#include "mainwindow.h"
#include <QApplication>
#include <QFont>
#include <QDebug>
#include <QFontDatabase>
#include "common.h"



int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	sharedResPath = QCoreApplication::applicationDirPath()+ "/../share/rfsom-box-gui/";
	int id = QFontDatabase::addApplicationFont(resolveFileLocation("font/dejavu-fonts-ttf-2.37/ttf/DejaVuSansMono.ttf"));
	QString family = QFontDatabase::applicationFontFamilies(id).at(0);
	QFont monospace(family);
	monospace.setPointSize(10);

	QApplication::setFont(monospace);
	MainWindow w;
	w.show();
	return a.exec();
}
