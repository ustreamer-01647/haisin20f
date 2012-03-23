#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QRect>
#include <QProcess>
#include <QSettings>

class Config : public QObject
{
	Q_OBJECT
public:
	explicit Config(QObject *parent = 0);
	void setArea(int x, int y, int width, int height);
	QString setProcess(QProcess *process);
	void setTask(Qt::CheckState enable, int floor
				 , QString filename, QString argment, Qt::CheckState after );
	void load(QString filename = "config.ini");
	void save(QString filename = "config.ini");
	int AreaX() const;
	int AreaY() const;
	int AreaWidth() const;
	int AreaHeight() const;
	QByteArray Geometry() const;
	void setGeometry(QByteArray geometry);

	Qt::CheckState TaskEnabled() const;
	int TaskFloor() const;
	QString TaskProgram() const;
	QString TaskProgramArgment() const;
	Qt::CheckState TaskAfterThenPause() const;

	Qt::CheckState DoPreview() const;
	void setDoPreview(Qt::CheckState state);

signals:
	
public slots:
	
private:
	/// ウインドウ位置
	QByteArray geometry;

	/// 注目範囲
	int areaX;
	int areaY;
	int areaWidth;
	int areaHeight;

	/// タスク有効
	Qt::CheckState taskEnabled;
	/// タスク発動階層
	int taskFloor;
	/// タスクプログラム
	QString taskProgram;
	/// タスクプログラム引数
	QString taskProgramArgment;
	/// タスクプログラム実行後の監視動作停止
	Qt::CheckState taskAfterThenPause;

	/// プレビュー有効
	Qt::CheckState doPreview;
};

#endif // CONFIG_H
