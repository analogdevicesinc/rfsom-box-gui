#include "appvideoplayer.h"
#include <QJsonObject>
#include <QCoreApplication>
#include <QScrollBar>
#include <QApplication>
#include <QDebug>
#include "common.h"
#include <QTime>
#include <QPlainTextEdit>

AppVideoPlayer::AppVideoPlayer(QJsonValue params, QLayout *lay, //QPlainTextEdit* te,
			       QWidget *parent) : App(parent) , params(params), lay(lay), te(nullptr)
{
	proc=nullptr;
	post_cmd="";
	hangOnFinish=false;
	consoleEnabled = true;
	scrollConsole = false;
	if(params.toObject().contains("post_cmd"))
	{
		post_cmd=params.toObject()["post_cmd"].toString();
	}
	if(params.toObject().contains("hang_on_finish"))
	{
		hangOnFinish=params.toObject()["hang_on_finish"].toBool();
	}

	if(params.toObject().contains("console"))
	{
		consoleEnabled=params.toObject()["console"].toBool();
	}

	if(params.toObject().contains("scroll_console"))
	{
		scrollConsole=params.toObject()["scroll_console"].toBool();
	}
	if(params.toObject().contains("input_map"))
	{
		auto inputMapJson = params.toObject()["input_map"].toObject();
		/*if(inputMapJson.contains("left_btn")) {
			inputMap[INPUT_LEFT]=inputMapJson["left_btn"].toString();
		}*/
		if(inputMapJson.contains("right_btn"))	{
			inputMap[INPUT_RIGHT]=inputMapJson["right_btn"].toString();
		}
		if(inputMapJson.contains("up_btn")) {
			inputMap[INPUT_UP]=inputMapJson["up_btn"].toString();
		}
		if(inputMapJson.contains("down_btn")) {
			inputMap[INPUT_DOWN]=inputMapJson["down_btn"].toString();
		}
		if(inputMapJson.contains("center_btn")) {
			inputMap[INPUT_CENTER]=inputMapJson["center_btn"].toString();
		}
		if(inputMapJson.contains("wheel_up")) {
			inputMap[INPUT_WHEEL_UP]=inputMapJson["wheel_up"].toString();
		}
		if(inputMapJson.contains("wheel_down"))	{
			inputMap[INPUT_WHEEL_DOWN]=inputMapJson["wheel_down"].toString();
		}
	}

	cmd = params.toObject()["cmd"].toString();
	te=new QPlainTextEdit(parent);
	te->setPlainText("");
	lay->addWidget(te);
	te->setReadOnly(true);
	te->setFocusPolicy(Qt::NoFocus);
	te->installEventFilter(this);
	te->setStyleSheet("font-size:8px");
	exitRequested=false;
	scrollToBottom=true;


}

AppVideoPlayer::~AppVideoPlayer()
{

}

void AppVideoPlayer::buildUi()
{
	load();
}

void AppVideoPlayer::destroyUi()
{
	unload();
	if(te!=nullptr)
	{
		te->removeEventFilter(this);
		te->deleteLater();
		te=nullptr;
	}
}

bool AppVideoPlayer::eventFilter(QObject *watched, QEvent *event)
{
	if(watched==te && event->type()==QEvent::Wheel)	{

		QWheelEvent *wheelEvent = static_cast<QWheelEvent *>(event);

		if(scrollConsole) {
			auto val = te->verticalScrollBar()->value();
			if (wheelEvent->delta()>0) {
				val--;
			} else {
				val++;
			}
			te->verticalScrollBar()->setValue(val);
			if(val!=te->verticalScrollBar()->maximum())
			{
				scrollToBottom=false;
			}
		}
		if(wheelEvent->delta()>0) {
			if(inputMap.contains(INPUT_WHEEL_UP)) {
				proc->write(inputMap[INPUT_WHEEL_UP].toLatin1().data());
				return true;
			}
		} else {
			if(inputMap.contains(INPUT_WHEEL_DOWN)) {
				proc->write(inputMap[INPUT_WHEEL_DOWN].toLatin1().data());
				return true;
			}
		}

	}
	return false;
}

bool AppVideoPlayer::isKeyMapped(int key)
{
	inputKeys val=INPUT_NONE;
	switch(key)
	{
	case Qt::Key_Up:
		val=INPUT_UP;
		break;
	case Qt::Key_Down:
		val=INPUT_DOWN;
		break;
	/*case Qt::Key_Left:
		val=INPUT_LEFT;
		break;*/
	case Qt::Key_Right:
		val=INPUT_RIGHT;
		break;
	case Qt::Key_Return:
		val=INPUT_CENTER;
		break;
	}
	return inputMap.contains(val);
}

void AppVideoPlayer::keyPressEvent(QKeyEvent *event)
{
	QKeyEvent *key = static_cast<QKeyEvent *>(event);
	inputKeys val=INPUT_NONE;
	switch(key->key())
	{
	case Qt::Key_Up:
		val=INPUT_UP;
		break;
	case Qt::Key_Down:
		val=INPUT_DOWN;
		break;
	case Qt::Key_Left:
		App::keyPressEvent(event);
		break;
	case Qt::Key_Right:
		val=INPUT_RIGHT;
		break;
	case Qt::Key_Return:
		val=INPUT_CENTER;
		break;
	}
	if(inputMap.contains(val)) {
		proc->write(inputMap[val].toLatin1().data());
		return;
	}
}

void AppVideoPlayer::unload()
{
	if (proc!=nullptr) {
		disconnect(proc,SIGNAL(finished(int)),this,SLOT(handleExitCode(int)));
		proc->write("q");
		proc->waitForFinished(1000);		
		proc->kill();
		proc->deleteLater();
		if(!post_cmd.isEmpty())
		{
			proc = new QProcess(this);
			proc->start("/bin/sh",QStringList() << "-c" <<  post_cmd);
			proc->waitForFinished(1000);
			proc->kill();
			proc->deleteLater();
		}
		proc=nullptr;
		exitRequested=true;
	}
}

void AppVideoPlayer::load()
{
	//unload();
	proc = new QProcess(this);
	proc->setWorkingDirectory(sharedResPath);
	proc->start("/bin/sh",QStringList() << "-c" <<  cmd);
	qDebug()<<proc->readAll();
	connect(proc,SIGNAL(finished(int)),this,SLOT(handleExitCode(int)));
	if(consoleEnabled) {
		connect(proc,SIGNAL(readyReadStandardError()),this,SLOT(readStdErr()));
	}
}

void AppVideoPlayer::readStdErr()
{
	if(proc) {
		auto errStr=proc->readAllStandardError();//.simplified();
		te->insertPlainText(errStr);
		if(scrollToBottom) {
			te->verticalScrollBar()->setValue(te->verticalScrollBar()->maximum());
		}
	}
}

void AppVideoPlayer::handleExitCode(int exitCode)
{
	if(exitCode!=0) {
		QString text="Errorcode "+QString::number(exitCode,16)+"\n";
		te->insertPlainText(text);
		auto errStr=proc->readAllStandardError();
		te->insertPlainText(errStr);
		setFocus();
	} else {
		qDebug()<<"App finished";
		if(!exitRequested && !hangOnFinish) {
			QKeyEvent *kev = new QKeyEvent( QEvent::KeyPress,Qt::Key_Left,Qt::NoModifier);
			QApplication::sendEvent(this, kev);
		}
	}
}
