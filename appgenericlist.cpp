#include "appgenericlist.h"
#include "hardware.h"
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QFile>
#include <QCoreApplication>
#include "scriptresult.h"
#include "readonlyelementui.h"
#include "buttonelementui.h"
#include "checkboxelementui.h"
#include "editboxelementui.h"
#include "numericelementui.h"
#include "ipaddresselementui.h"
#include <QDirIterator>
#include <QKeyEvent>
#include <QApplication>

AppGenericList::AppGenericList(QJsonValue params, QLayout *lay,
                               QWidget *parent) : App(parent) , params(params)
{
	list = new QListWidget(this);
	list->setMaximumWidth(maxWidth);
	list->setMaximumHeight(maxHeight);
	list->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	list->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	// add stylesheet for item spacing

	auto filePath = QCoreApplication::applicationDirPath()
	                +"/../stylesheets/appGenericList.qss";
	QFile f(filePath);

	if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QTextStream s(&f);
		QString str = s.readAll();
		list->setStyleSheet(str);
	} else {
		qDebug()<<"Stylesheet not found @ " << filePath;
	}

	lay->addWidget(list);
	qDebug()<<"AppGenericListConstructor";
}

QWidget *AppGenericList::setupReadOnlyElementUi(QJsonObject obj)
{
	auto ui_element = new ReadOnlyElementUi(list);
	ui_element->setStrVal(new ScriptResult(obj["cmd"].toString(),ui_element));

	if (obj.contains("one_row")) {
		ui_element->setOneRow(obj["one_row"].toBool());
	}

	if (obj.contains("description")) {
		ui_element->setDescription(obj["description"].toString());
	}

	if (obj.contains("stylesheet")) {
		ui_element->setStyleSheet(obj["stylesheet"].toString());
	}

	/*if(obj.contains(""))
		ui_element->setIconRight(bool value);
	if(obj.contains("")
		ui_element->setIconPath(const QString &value);
	if(obj.contains("icon_size")
		ui_element->setIconSize(obj["icon_size"].toInt());*/

	ui_element->update();

	if (obj.contains("timer")) {
		bool ok = false;
		int timer_id = obj["timer"].toString().toInt(&ok);

		if (!(ok && timer_id >= 0 && timer_id < timers.count())) {
			timer_id = 0; // connects to the default timer 0
		}

		connect(timers[timer_id],SIGNAL(timeout()),ui_element,SLOT(update()));

	}

	return ui_element;


}


QWidget *AppGenericList::setupButtonElementUi(QJsonObject obj)
{
	auto ui_element = new ButtonElementUi(list);

	ui_element->setScript(new ScriptResult(obj["cmd"].toString(),ui_element));

	if (obj.contains("description")) {
		ui_element->setDescription(obj["description"].toString());
	}

	if (obj.contains("stylesheet")) {
		ui_element->setStyleSheet(obj["stylesheet"].toString());
	}


	return ui_element;
}

QWidget *AppGenericList::setupCheckboxElementUi(QJsonObject obj)
{
	auto ui_element = new CheckboxElementUi(list);

	//ui_element->setScript(new ScriptResult(obj["cmd"].toString(),ui_element));

	if (obj.contains("description")) {
		ui_element->setDescription(obj["description"].toString());
	}

	if (obj.contains("stylesheet")) {
		ui_element->setStyleSheet(obj["stylesheet"].toString());
	}

	if (obj.contains("cmd_read")) {
		ui_element->setRead(new ScriptResult(obj["cmd_read"].toString(),ui_element));
	}

	if (obj.contains("cmd_write")) {
		ui_element->setWrite(new ScriptResult(obj["cmd_write"].toString(),ui_element));
	}




	if (obj.contains("cmd_read_timer")) {
		bool ok = false;
		int timer_id = obj["timer"].toString().toInt(&ok);

		if (!(ok && timer_id >= 0 && timer_id < timers.count())) {
			timer_id = 0; // connects to the default timer 0
		}

		connect(timers[timer_id],SIGNAL(timeout()),ui_element,SLOT(update()));

	}

	ui_element->update();
	return ui_element;
}

QWidget *AppGenericList::setupEditElementUi(QJsonObject obj)
{
	EditboxElementUi *ui_element;
	NumericElementUi *num_element = nullptr;
	IPAddressElement *ip_element = nullptr;

	if (obj.contains("subtype")) {
		if (obj["subtype"].toString()=="numeric") {
			ui_element = new NumericElementUi(list);
			num_element = dynamic_cast<NumericElementUi *>(ui_element);
		} else if (obj["subtype"].toString()=="ip") {
			ui_element = new IPAddressElement(list);
			ip_element = dynamic_cast<IPAddressElement *>(ui_element);
		} else {
			ui_element = new EditboxElementUi(list);
		}
	} else {
		ui_element = new EditboxElementUi(list);
	}


	if (obj.contains("description")) {
		ui_element->setDescription(obj["description"].toString());
	}

	if (obj.contains("stylesheet")) {
		ui_element->setStyleSheet(obj["stylesheet"].toString());
	}

	if (obj.contains("cmd_read")) {
		ui_element->setRead(new ScriptResult(obj["cmd_read"].toString(),ui_element));
	}

	if (obj.contains("cmd_write")) {
		ui_element->setWrite(new ScriptResult(obj["cmd_write"].toString(),ui_element));
	}

	if (obj.contains("cmd_read_only")) {
		ui_element->setReadOnly(new ScriptResult(obj["cmd_read_only"].toString(),
		                        ui_element));
	}


	if (obj.contains("cmd_read_timer")) {
		bool ok = false;
		int timer_id = obj["timer"].toString().toInt(&ok);

		if (!(ok && timer_id >= 0 && timer_id < timers.count())) {
			timer_id = 0; // connects to the default timer 0
		}

		connect(timers[timer_id],SIGNAL(timeout()),ui_element,SLOT(update()));

	}



	if (num_element!=nullptr) {
		if (obj.contains("nr_of_digits")) {
			auto nr_of_digits = obj["nr_of_digits"].toInt();
			num_element->setNrOfDigits(nr_of_digits);
			/*QString mask;
			for(auto i=0;i<nr_of_digits;i++)
			{
				if(i%3==0 && i!=0)
					mask = mask.prepend(" ");
				mask = mask.prepend("0");
			}
			qDebug()<<mask;

			num_element->setInputMask(mask);*/
		}

		return num_element;

	}

	ui_element->update();
	return ui_element;

}

void AppGenericList::setupTimers()
{
	// search for timers
	if (params.toObject().contains("timers")) {
		for (QJsonValue val : params.toObject()["timers"].toArray()) {
			if (val.toInt() > cMinimumTimerPeriod) {
				QTimer *tim = new QTimer(this);
				timers.append(tim);
				tim->start(val.toInt());
			}
		}
	} else {
		qInfo()<<"No timers defined in the configuration json. Creating default 1 second timer";
		QTimer *tim  = new QTimer(this);
		timers.append(tim);
		tim->start(1000); // default 1 second timer
	}
}

void AppGenericList::buildUi()
{

	setupTimers();

	// search for Ui elements
	if (!params.toObject().contains("elements")) {
		qDebug()<<"No elements defined in the configuration json.";
		return;
	}

	for (auto val : params.toObject()["elements"].toArray()) {
		auto obj = val.toObject();
		QWidget *element = nullptr;

		if (obj["type"]=="cmd_read") {
			element = setupReadOnlyElementUi(obj);
		} else if (obj["type"]=="btn") {
			element = setupButtonElementUi(obj);
		} else if (obj["type"]=="checkbox") {
			element = setupCheckboxElementUi(obj);
		} else if (obj["type"]=="edit")	{
			element = setupEditElementUi(obj);
		}


		if (element!=nullptr) {
			int cnt = list->count();//get number of items before adding
			list->addItem("");

			if (cnt % 2 == 1) {
				element->setStyleSheet("background-color:black");
			}

			list->setItemWidget(list->item(cnt),element);
		}

	}

	list->setFocus();
	list->installEventFilter(this);
}

bool AppGenericList::eventFilter(QObject *watched, QEvent *event)
{
	if (event->type()==QEvent::Wheel) {
		watched = QApplication::focusWidget();
	}

	if (watched == list) {
		qDebug()<<event;

		if (event->type()==QEvent::KeyPress) {
			QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

			switch (keyEvent->key()) {
			case Qt::Key_Up:
			case Qt::Key_Down:
				return false;

			case Qt::Key_Return:
				confirmListSelection();
				return true;

			case Qt::Key_Left:
			case Qt::Key_Right:
				return false;

			default:
				return true;
			}
		}
	}

	auto editElement = dynamic_cast<EditboxElementUi *>(watched);

	if (editElement!=nullptr) {
		qDebug()<<"eventfilter - "<<event;

		if (event->type()==QEvent::KeyPress) {
			QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
			qDebug()<<keyEvent;

			switch (keyEvent->key()) {
			case Qt::Key_Left:
				return editElement->cursorLeft();

			case Qt::Key_Right:
				return editElement->cursorRight();

			case Qt::Key_Up:
			case Qt::Key_Down:
			case Qt::Key_Return:
				editElement->click();
				editElement->removeEventFilter(this);
				editElement->releaseKeyboard();
				editElement->releaseMouse();
				editElement->setSelected(false);
				list->setFocus();
				return true;

			case Qt::Key_PageUp:
				editElement->increment();
				return true;

			case Qt::Key_PageDown:
				editElement->decrement();
				return true;

			}
		}

		if (event->type()==QEvent::Wheel) {
			QWheelEvent *wheelEvent = static_cast<QWheelEvent *>(event);
			qDebug()<<wheelEvent;

			if (wheelEvent->delta()>0) {
				editElement->increment();
			} else {
				editElement->decrement();
			}
		}
	}

	qDebug()<<event << "not handeld by eventfilter";
	return false;
}

void AppGenericList::destroyUi()
{

}

AppGenericList::~AppGenericList()
{
	delete list;
}

void AppGenericList::confirmListSelection()
{
	auto current = list->currentItem();
	auto wid = list->itemWidget(current);
	ButtonElementUi *btnElement = dynamic_cast<ButtonElementUi *>(wid);
	CheckboxElementUi *chkElement = dynamic_cast<CheckboxElementUi *>(wid);
	EditboxElementUi *editElement = dynamic_cast<EditboxElementUi *>(wid);


	// create listelement class
	if (btnElement!=nullptr) {
		btnElement->click();
	}

	if (chkElement!=nullptr) {
		chkElement->click();
	}

	if (editElement!=nullptr) {
		if (!editElement->isReadOnly()) {
			editElement->installEventFilter(this);
			editElement->grabKeyboard();
			editElement->grabMouse();
			editElement->click();
		}
	}

}

