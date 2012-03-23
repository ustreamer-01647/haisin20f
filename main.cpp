#include <QtGui/QApplication>
#include "mainwindow.h"
#include <QTextCodec>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QTextCodec *codec = QTextCodec::codecForName(("UTF-8"));
	QTextCodec::setCodecForTr(codec);
	QTextCodec::setCodecForLocale(codec);
	QTextCodec::setCodecForCStrings(codec);
	MainWindow w;
	w.show();

	return a.exec();
}

/*
  */

/* 動作
  画面全体のスクリーンショットを基にする．
  数値部分を解析し，判別する．
  解析範囲を限定する．
  */

/* 設定
  スクリーンショット取得間隔
  実行するプログラム
  条件値
  解析範囲
  動作開始/停止
  解析間隔
  */

/* UI
  解析範囲．テキストボックス4個．左上を原点とする．始点座標と幅と高さ．
  主動作制御ボタン．ボタン3個．開始，停止，終了
  解析経過プレビュー．QPixmap複数個
  解析結果プレビュー．テキストボックス1個
  タスク登録
  */

/* タスクUI
  有効/無効チェックボックス．
  動作条件
  実行するプログラム
  発動後のアプリケーション動作状態を停止するか否か．チェックボックス
  */
