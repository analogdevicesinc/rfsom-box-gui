#ifndef UIELEMENT_H
#define UIELEMENT_H

#include <QWidget>

class UiElement : public QWidget
{
	Q_OBJECT
public:
	explicit UiElement(QWidget *parent = 0) : QWidget(parent) {}
	virtual ~UiElement() {}
signals:

public slots:
	virtual void click() {}
	virtual void update() {}
};

#endif // UIELEMENT_H
