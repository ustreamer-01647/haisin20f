#include "config.h"
#include <QFileInfo>

Config::Config(QObject *parent) :
	QObject(parent)
{
}

void Config::setArea(int x, int y, int width, int height)
{
	this->areaX = x;
	this->areaY = y;
	this->areaWidth = width;
	this->areaHeight = height;
}

QString Config::setProcess(QProcess *process)
{
	QFileInfo fileinfo(this->taskProgram);
	// 引数
	process->setNativeArguments(this->taskProgramArgment);
	// 作業ディレクトリ
	process->setWorkingDirectory(fileinfo.absolutePath());
	return fileinfo.absoluteFilePath();
}

void Config::setTask(Qt::CheckState enable, int floor, QString filename, QString argment, Qt::CheckState after)
{
	taskEnabled = enable;
	taskFloor = floor;
	taskProgram = filename;
	taskProgramArgment = argment;
	taskAfterThenPause = after;
}

void Config::load(QString filename)
{
	QSettings setting(filename, QSettings::IniFormat);
	// ウインドウ位置
	this->geometry = setting.value("geometry").toByteArray();

	// フォーム部品
	setting.beginGroup("target");
	this->areaX = setting.value("x", 0).toInt();
	this->areaY = setting.value("y", 0).toInt();
	this->areaWidth = setting.value("width", 67).toInt();
	this->areaHeight = setting.value("height", 33).toInt();
	setting.endGroup();
	setting.beginGroup("task");
	this->taskEnabled = (Qt::CheckState)setting.value("enabled", Qt::Unchecked).toUInt();
	this->taskFloor = setting.value("floor", 20).toInt();
	this->taskProgram = setting.value("program").toString();
	this->taskProgramArgment = setting.value("argment").toString();
	this->taskAfterThenPause = (Qt::CheckState)setting.value("after", Qt::Checked).toUInt();
	setting.endGroup();
	setting.beginGroup("preview");
	this->doPreview = (Qt::CheckState)setting.value("doPreview", Qt::Checked).toUInt();
	setting.endGroup();
}

void Config::save(QString filename)
{
	QSettings setting(filename, QSettings::IniFormat);
	// ウインドウ位置
	setting.setValue("geometry", this->geometry);

	// フォーム部品
	setting.beginGroup("target");
	setting.setValue("x", this->areaX);
	setting.setValue("y", this->areaY);
	setting.setValue("width", this->areaWidth);
	setting.setValue("height", this->areaHeight);
	setting.endGroup();
	setting.beginGroup("task");
	setting.setValue("enabled", this->taskEnabled);
	setting.setValue("floor", this->taskFloor);
	setting.setValue("program", this->taskProgram);
	setting.setValue("argment", this->taskProgramArgment);
	setting.setValue("after", this->taskAfterThenPause);
	setting.endGroup();
	setting.beginGroup("preview");
	setting.setValue("doPreview", this->doPreview);
	setting.endGroup();
}

int Config::AreaX() const
{
	return this->areaX;
}

int Config::AreaHeight() const
{
	return this->areaHeight;
}

QByteArray Config::Geometry() const
{
	return this->geometry;
}

void Config::setGeometry(QByteArray geometry)
{
	this->geometry = geometry;
}

Qt::CheckState Config::TaskEnabled() const
{
	return this->taskEnabled;
}

int Config::TaskFloor() const
{
	return this->taskFloor;
}

QString Config::TaskProgram() const
{
	return this->taskProgram;
}

QString Config::TaskProgramArgment() const
{
	return this->taskProgramArgment;
}

Qt::CheckState Config::TaskAfterThenPause() const
{
	return this->taskAfterThenPause;
}

Qt::CheckState Config::DoPreview() const
{
	return this->doPreview;
}

void Config::setDoPreview(Qt::CheckState state)
{
	this->doPreview = state;
}

int Config::AreaWidth() const
{
	return this->areaWidth;
}

int Config::AreaY() const
{
	return this->areaY;
}
