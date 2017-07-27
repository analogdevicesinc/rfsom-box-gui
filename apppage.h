#ifndef APPPAGE_H
#define APPPAGE_H

#include "menupage.h"
#include <QTimer>
#include "sysfsvalue.h"
#include "readonlyelementui.h"

class AppPage : public MenuPage
{
	Q_OBJECT
public:

	explicit AppPage(QWidget *parent = 0);

	virtual void unload();
	virtual void load();
	virtual void initialize();

public slots:
	void updateUi();

private:

};


#endif // APPPAGE_H
