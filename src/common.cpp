#include <QFileInfo>
#include <QDebug>
#include "common.h"

QString sharedResPath;
QString resolveFileLocation(QString fileName)
{
	QFileInfo check_file(fileName);
	if( check_file.exists() && check_file.isFile())
	{
		return fileName;
	}

	auto newfilename = QCoreApplication::applicationDirPath()+"/"+fileName;
	check_file.setFile(newfilename);
	if(check_file.exists() && check_file.isFile())
		return newfilename;

	newfilename =sharedResPath+fileName;
	check_file.setFile(newfilename);
	if(check_file.exists() && check_file.isFile())
		return newfilename;

	qDebug()<<"Launcher json not found.\n jsonFileName "<<fileName<<"\nsharedResPath "<<sharedResPath;
		return "";
}
