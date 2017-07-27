#ifndef APP_H
#define APP_H

#include <QWidget>

class App : public QWidget
{
	Q_OBJECT
public:
	explicit App(QWidget *parent = 0);
	virtual ~App() {}

public slots:
	virtual void setKeys() {}
	virtual void buildUi() {}
	virtual void destroyUi() {}
	virtual void load() {}
	virtual void unload() {}


};

#endif // APP_H
