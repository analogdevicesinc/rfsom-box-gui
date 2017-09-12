#ifndef TOASTNOTIFICATION_H
#define TOASTNOTIFICATION_H

#include <QMessageBox>
#include <QTimer>

class ToastNotification : public QMessageBox
{

public:
	ToastNotification(QString str, int timeout, QWidget *parent=nullptr) : QMessageBox(parent)
	{
		QTimer *tim=new QTimer(this);
		setStandardButtons(0);
		move(15,50);
		setStyleSheet("QMessageBox {  background-color: #000000;}\nQLabel {color:#ffffff} ");
		setText(str);
		connect(tim,SIGNAL(timeout()),this,SLOT(deleteLater()));
		tim->setSingleShot(true);
		tim->setInterval(timeout);
		tim->start();
		show();}
};

#endif // TOASTNOTIFICATION_H
