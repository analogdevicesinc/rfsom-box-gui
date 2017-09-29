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
	virtual void loadJsonConfig(QString);
	void keyPressEvent(QKeyEvent *);
	void wheelEvent(QWheelEvent *event);

public slots:
	virtual void updateUi();

private:
	QList<QTimer *> timers;
	const int cMinimumTimerPeriod = 100;
};


#endif // LANDINGPAGE_H
