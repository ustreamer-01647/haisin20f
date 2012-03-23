#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QPixmap>
#include <QTimer>
#include <QImage>
#include "config.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:

	void on_toolButtonProgramRef_clicked();

	void on_buttonApplyArea_clicked();

	void on_buttonExecProgram_clicked();

	void on_buttonApplyTask_clicked();

	void on_buttonQuit_clicked();

	void on_buttonShoot_clicked();

	void on_buttonStart_clicked();

	void on_buttonStop_clicked();

	void shootScreen();

	void on_checkBoxDoPreview_toggled(bool checked);

private:
	Ui::MainWindow *ui;
	void closeEvent(QCloseEvent *event);
	Config config;
	void configuration();
	QPixmap screen;
	QTimer timer;
	void start();
	void stop();
	void updateLabelScreen();
	int extractedFloor;
	void updateLabelExtractedFloor();
};

#endif // MAINWINDOW_H

