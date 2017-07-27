#ifndef LANDINGPAGE_H
#define LANDINGPAGE_H

#include "menupage.h"
#include <QTimer>
#include "sysfsvalue.h"

class LandingPage : public MenuPage
{
	Q_OBJECT
public:

	explicit LandingPage(QWidget *parent = 0);
	virtual void unload();
	virtual void load();
	virtual void initialize();
	void keyPressEvent(QKeyEvent *);

public slots:
	virtual void updateUi();

private:
	const QString temperaturePath="/home/analog/temperature";
	const QString chargePath="/home/analog/charge";
	const QString rssiPath="/home/analog/rssi";
	const QString networkPath="/home/analog/network";

	SysFSValue *temperature;
	SysFSValue *charge;
	SysFSValue *rssi;
	SysFSValue *network;

	QTimer *refreshTimer;
};


#endif // LANDINGPAGE_H
