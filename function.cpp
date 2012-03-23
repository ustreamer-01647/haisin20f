#include "function.h"

cv::Mat qimage2mat(const QImage& qimage)
{
	cv::Mat mat = cv::Mat(qimage.height(), qimage.width(), CV_8UC4, (uchar*)qimage.bits(), qimage.bytesPerLine());
	cv::Mat mat2 = cv::Mat(mat.rows, mat.cols, CV_8UC3 );
	int from_to[] = { 0,0,  1,1,  2,2 };
	cv::mixChannels( &mat, 1, &mat2, 1, from_to, 3 );
	return mat2;
}

QImage mat2qimage(const cv::Mat& mat, const bool isIndexed8) {
	if ( isIndexed8 )
	{
		return QImage((const unsigned char*)(mat.data), mat.cols, mat.rows, QImage::Format_Indexed8);
	}else{
		cv::Mat rgb;
		cv::cvtColor(mat, rgb, CV_BGR2RGB);
		return QImage((const unsigned char*)(rgb.data), rgb.cols, rgb.rows, QImage::Format_RGB888);
	}
 }

cv::Mat topPlaneofMat(const cv::Mat &mat)
{
	std::vector<cv::Mat> planes;
	cv::split(mat, planes);
	return planes[0];
}

/** thinningMommaで使用するフィルタを作る
  kernel pattern white/black?
  */
void thinningMommaInit(std::vector<cv::Mat> &kpw, std::vector<cv::Mat> &kpb)
{
	// cv::Filter2D用のカーネル
	/* アルゴリズムでは白，黒のマッチングとなっているのを
		kpwカーネルと二値画像，
		kpbカーネルと反転した二値画像
		の2組に分けて畳み込み，その後でANDをとる
	*/
	// kernel1
	kpb.push_back((cv::Mat_<unsigned char>(3,3) <<
				   1, 1, 0, 1, 0, 0, 0, 0, 0));
	kpw.push_back((cv::Mat_<unsigned char>(3,3) <<
				   0, 0, 0, 0, 1, 1, 0, 1, 0));
	// kernel2
	kpb.push_back((cv::Mat_<unsigned char>(3,3) <<
				   1, 1, 1, 0, 0, 0, 0, 0, 0));
	kpw.push_back((cv::Mat_<unsigned char>(3,3) <<
				   0, 0, 0, 0, 1, 0, 1, 1, 0));
	// kernel3
	kpb.push_back((cv::Mat_<unsigned char>(3,3) <<
				   0, 1, 1, 0, 0, 1, 0, 0, 0));
	kpw.push_back((cv::Mat_<unsigned char>(3,3) <<
				   0, 0, 0, 1, 1, 0, 0, 1, 0));
	// kernel4
	kpb.push_back((cv::Mat_<unsigned char>(3,3) <<
				   0, 0, 1, 0, 0, 1, 0, 0, 1));
	kpw.push_back((cv::Mat_<unsigned char>(3,3) <<
				   1, 0, 0, 1, 1, 0, 0, 0, 0));
	// kernel5
	kpb.push_back((cv::Mat_<unsigned char>(3,3) <<
				   0, 0, 0, 0, 0, 1, 0, 1, 1));
	kpw.push_back((cv::Mat_<unsigned char>(3,3) <<
				   0, 1, 0, 1, 1, 0, 0, 0, 0));
	// kernel6
	kpb.push_back((cv::Mat_<unsigned char>(3,3) <<
				   0, 0, 0, 0, 0, 0, 1, 1, 1));
	kpw.push_back((cv::Mat_<unsigned char>(3,3) <<
				   0, 1, 1, 0, 1, 0, 0, 0, 0));
	// kernel7
	kpb.push_back((cv::Mat_<unsigned char>(3,3) <<
				   0, 0, 0, 1, 0, 0, 1, 1, 0));
	kpw.push_back((cv::Mat_<unsigned char>(3,3) <<
				   0, 1, 0, 0, 1, 1, 0, 0, 0));
	// kernel8
	kpb.push_back((cv::Mat_<unsigned char>(3,3) <<
				   1, 0, 0, 1, 0, 0, 1, 0, 0));
	kpw.push_back((cv::Mat_<unsigned char>(3,3) <<
				   0, 0, 0, 0, 1, 1, 0, 0, 1));
}

/** Momma Wikiの細線化
  一続きのはずが，ちぎれてしまうので，要改善
  @see http://www.eml.ele.cst.nihon-u.ac.jp/~momma/wiki/wiki.cgi/OpenCV/%e7%b4%b0%e7%b7%9a%e5%8c%96.html
  */
cv::Mat thinningMomma(const cv::Mat &input)
{
	// カーネル
	std::vector<cv::Mat> kpw, kpb;
	thinningMommaInit(kpw, kpb);
	// 写し間違えた？転置してみる
//	for(int i = 0; i < 8; i ++ )
//	{
//		kpw[i] = kpw[i].t();
//		kpb[i] = kpb[i].t();
//	}

	// 32Fで作業する
	// white 作業バッファ
	// black 作業バッファ（反転）
	cv::Mat inFloat, white, black;
	inFloat = cv::Mat(input.size(), CV_32F);
	white = cv::Mat(input.size(), CV_32F);
	black = cv::Mat(input.size(), CV_32F);
	input.convertTo(inFloat, CV_32F, 1.0/255.0);
	cv::threshold(inFloat, inFloat, 0.5, 1.0, CV_THRESH_BINARY);
	cv::threshold(inFloat, white, 0.5, 1.0, CV_THRESH_BINARY);
	cv::threshold(inFloat, black, 0.5, 1.0, CV_THRESH_BINARY_INV);

	// 1ターンでマッチしていなければ終了
	double sum = 1;
	while ( sum > 0 )
	{
		sum = 0;
		for ( int i = 0; i < 8; i++)
		{
			cv::filter2D(white, white, -1, kpw[i]);
			cv::filter2D(black, black, -1, kpb[i]);
			// 各カーネルで注目するのは3画素ずつなので，マッチした注目画素の濃度は3となる
			// カーネルの値を1/9にしておけば，しきい値は0.99で良い
			cv::threshold(white, white, 2.99, 1.0, CV_THRESH_BINARY);
			cv::threshold(black, black, 2.99, 1.0, CV_THRESH_BINARY);
			cv::bitwise_and(white, black, white);
			// この時点でのwhiteが消去候補点となり，全カーネルで候補点が0となった時に処理が終わる
			sum += cv::sum(white)[0];
			// 原画像から候補点を消去する．二値画像なのでXOR
			cv::bitwise_xor(inFloat, white, inFloat);
			// 作業バッファを更新
			inFloat.copyTo(white);
			cv::threshold(inFloat, black, 0.5, 1, CV_THRESH_BINARY_INV);
		}
	}

	// 8Uの画像に戻す
	cv::Mat thin;
	cv::convertScaleAbs(inFloat, thin, 255.0);

#if 1
	// 画像表示
	cv::imshow("thin", thin);
#endif

	return thin;
}
