#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSettings>
#include <QFileDialog>
#include <QDesktopWidget>
#include <QWidget>
#include "function.h"
#include "extractnumber.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	// 設定をファイルから読み込む
	config.load();
	// フォームに反映する
	configuration();
	// スクリーンショット定期撮影
	connect(&timer, SIGNAL(timeout()), SLOT(shootScreen()));
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	// ウインドウ位置保存
	config.setGeometry(saveGeometry());
	// 各種設定保存
	config.save();
	QMainWindow::closeEvent(event);
}

void MainWindow::configuration()
{
	// ウインドウ位置
	restoreGeometry(config.Geometry());
	// target
	ui->lineEditX->setText(QString().setNum(config.AreaX()));
	ui->lineEditY->setText(QString().setNum(config.AreaY()));
	ui->lineEditWidth->setText(QString().setNum(config.AreaWidth()));
	ui->lineEditHeight->setText(QString().setNum(config.AreaHeight()));
	// task
	ui->checkBoxTaskEnable->setCheckState(config.TaskEnabled());
	ui->lineEditTaskFloor->setText(QString().setNum(config.TaskFloor()));
	ui->lineEditTaskProgram->setText(config.TaskProgram());
	ui->lineEditTaskArgment->setText(config.TaskProgramArgment());
	ui->checkBoxTaskAfter->setCheckState(config.TaskAfterThenPause());
	// preview
	ui->checkBoxDoPreview->setCheckState(config.DoPreview());

}

void MainWindow::shootScreen()
{
	this->screen = QPixmap();
	this->screen = QPixmap::grabWindow(QApplication::desktop()->screen()->winId(),
							config.AreaX(), config.AreaY(),
							config.AreaWidth(),config.AreaHeight());

	// 青平面取得
	cv::Mat img, blue;
	img = qimage2mat(this->screen.toImage());
	blue = topPlaneofMat(img);


	// 二値化
	cv::Mat bin;
	cv::threshold(blue, bin, 230.0, 255.0, cv::THRESH_BINARY); // アスカ階層数値のB輝度値は231

//	// 細線化
//	cv::Mat thin;
//	thin = thinningMomma(bin);
//	cv::namedWindow("thin");
//	cv::imshow("thin", thin);
#if 0
	// デバッグ表示
	cv::imshow("blue", blue);
	cv::imshow("bin", bin);
#endif

	// 数値読み出し
	ExtractNumber en(bin);
	this->extractedFloor = en.extract();

	// プレビュー
	if ( ui->checkBoxDoPreview->isChecked() )
	{
		updateLabelScreen();
		updateLabelExtractedFloor();
	}

	// タスク
	if ( Qt::Checked == config.TaskEnabled()
		 && config.TaskFloor() <= this->extractedFloor )
	{
		// 作動後停止設定
		if ( Qt::Checked == config.TaskAfterThenPause()
			 && this->timer.isActive())
		{
			on_buttonStop_clicked();
		}
		// タスク実行
		on_buttonExecProgram_clicked();
	}
}

void MainWindow::start()
{
	if( !timer.isActive())
	{
		timer.start( 5 * 1000 );
	}
}

void MainWindow::stop()
{
	timer.stop();
}

void MainWindow::updateLabelScreen()
{
		ui->labelScreen->setPixmap(this->screen.scaled(ui->labelScreen->size(),
														   Qt::KeepAspectRatio,
														   Qt::SmoothTransformation));
}

void MainWindow::updateLabelExtractedFloor()
{
	ui->labelExtractValue->setNum(this->extractedFloor);
}

void MainWindow::on_toolButtonProgramRef_clicked()
{
	QString filename = QFileDialog::getOpenFileName(this);
	if( !filename.isEmpty() )
	{
		ui->lineEditTaskProgram->setText( filename );
	}
}

void MainWindow::on_buttonApplyArea_clicked()
{
	config.setArea(ui->lineEditX->text().toInt()
				   ,ui->lineEditY->text().toInt()
				   ,ui->lineEditWidth->text().toInt()
				   ,ui->lineEditHeight->text().toInt());
}

void MainWindow::on_buttonExecProgram_clicked()
{
	QProcess *process = new QProcess();
	QString filename = config.setProcess(process);
	process->start(filename);
}

void MainWindow::on_buttonApplyTask_clicked()
{
	config.setTask(ui->checkBoxTaskEnable->checkState()
				   ,ui->lineEditTaskFloor->text().toInt()
				   ,ui->lineEditTaskProgram->text()
				   ,ui->lineEditTaskArgment->text()
				   ,ui->checkBoxTaskAfter->checkState());
}

void MainWindow::on_buttonQuit_clicked()
{
	this->close();
}

void MainWindow::on_buttonShoot_clicked()
{
	shootScreen();
}

void MainWindow::on_buttonStart_clicked()
{
	start();
	ui->buttonStart->setEnabled(false);
	ui->buttonStop->setEnabled(true);
}

void MainWindow::on_buttonStop_clicked()
{
	stop();
	ui->buttonStart->setEnabled(true);
	ui->buttonStop->setEnabled(false);
}

void MainWindow::on_checkBoxDoPreview_toggled(bool checked)
{
	if ( checked )
	{
		config.setDoPreview(Qt::Checked);
	}else
	{
		config.setDoPreview(Qt::Unchecked);
	}
}
