#include "extractnumber.h"
#include "function.h"

ExtractNumber::ExtractNumber(cv::Mat image)
{
	setSource(image);
}

// CV_8UC1を設定する
void ExtractNumber::setSource(cv::Mat image)
{
	this->source = image.clone();
}

int ExtractNumber::extract()
{
	if ( this->source.empty() )
		return 0;

	std::vector<cv::Mat> character;
	split(character);
	int characterCount = character.size();
	int value = 0;
	for ( int cc = 0; cc < characterCount; cc++ )
	{
		int n = number(character[cc]);
		int keta = std::pow(10, characterCount-cc-1);
		value += n*keta;
		//		value += (std::pow(10, characterCount-cc-1) * number(character[cc]));
	}

	return value;
}

// 1字ずつ切り出す
void ExtractNumber::split(std::vector<cv::Mat> &character)
{
	// 上下の余白を除去する
	int width = source.cols;
	int height = source.rows;
	// 上端を調べる
	int w, h, sum = 0;
	int startRow = 0;
	for ( h = startRow; h < height; h++ )
	{
		for ( w = 0; w < width; w++ )
		{
			sum += (int)((unsigned char*)(source.data[h*width+w]));
		}
		// 下へ走査し，最初の輝点があるとき上端
		if ( 0 < sum )
		{
			startRow = h;
			break;
		}
	}
	int endRow = height;
	// 下端を調べる
	sum = 0;
	for ( h = height-1 ; 0 <= h; h-- )
	{
		for ( w = 0; w < width; w++ )
		{
			sum += (int)((unsigned char*)(source.data[h*width+w]));
		}
		// 上へ走査し，輝点があるとき下端
		if ( 0 < sum )
		{
			endRow = h;
			break;
		}
	}

	// 左右端を探索し，1文字ずつ切り出す
	std::vector<int> sliceCol; // 文字切り出し位置．始点終点を交互に入力する
	bool isSearchStart = true;
	// 横方向に探索
	sum = 0;
	for ( w = 0; w < width; w++ )
	{
		// 縦方向に探索する
		for ( h = startRow; h < endRow; h++ )
		{
			sum += (int)((unsigned char*)(source.data[h*width+w]));
		}
		if( isSearchStart && sum > 0 )
		{
			// 輝点があれば始端登録
			sliceCol.push_back(w);
			isSearchStart = false;
		}else if ( !isSearchStart && sum == 0 )
		{
			// 輝点がなければ終端登録
			sliceCol.push_back(w);
			isSearchStart = true;
		}
		sum = 0;
	}
	// 終端探索中に終了したら終端登録
	if ( false == isSearchStart )
	{
		sliceCol.push_back(w);
	}

	// 上下左右の下端情報を得て，文字を切り出す
	int characterCount = sliceCol.size()/2;

	// このへんの#if0は，数字を赤色で囲む
#if 0
	cv::Mat kakomu;
	std::vector<cv::Mat> kako;
	kako.push_back(source);
	kako.push_back(source);
	kako.push_back(source);
	cv::merge(kako, kakomu);
#endif
	for ( int cc = 0 ; cc < characterCount; cc++ )
	{
		character.push_back( source(cv::Range(startRow, endRow),
									cv::Range(sliceCol[cc*2], sliceCol[cc*2+1])).clone() );
#if 0
		cv::rectangle(kakomu, cv::Point(sliceCol[cc*2], startRow),
					  cv::Point(sliceCol[cc*2+1], endRow), cv::Scalar(0,0,255) );
#endif
	}
#if 0
	cv::namedWindow("kakomu");
	cv::imshow("kakomu", kakomu);
#endif
}

// 数字1字を認識する
int ExtractNumber::number(cv::Mat character)
{
	// 輪郭抽出で分別する
	// 12357, 0469, 8
	/// @see http://opencv.jp/cookbook/opencv_img.html#id54
	std::vector<std::vector<cv::Point> > contours; // 輪郭情報
	std::vector<cv::Vec4i> hierarchy; // 階層構造
	// 2値画像，輪郭（出力），階層構造（出力），輪郭抽出モード，輪郭の近似手法
	cv::findContours(character, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

	// 8, 02469, 12357
	// 輪郭数で分別する
	if ( 3 == hierarchy.size())
	{
		return 8;
	}
	else if( 2 == hierarchy.size() )
	{
		// 6, 049
		// 第2輪郭始点が上部3分の1にないとき6
		if ( contours[1][0].y > (character.rows/3) )
		{
			return 6;
		}
		else
		{
			// 9, 04
			// 中央部の縦の輝点パターン
			// 9: 10101
			// 0,4: 101
			if( 3 == rowBrightPattern(character, character.cols/2) )
			{
				return 9;
			}else
			{
				// 4, 0
				// 右端の上から3分の1に輝点があるとき0
				int u = utan(character);
				if ( 0 < ((unsigned char*)character.data[character.rows/3*character.cols+u] ))
				{
					return 0;
				}else
				{
					return 4;
				}
			}
		}
	}
	else if ( 1 == hierarchy.size() )
	{
		// 17, 235
		// 左端の輝点パターン
		// 17: 1
		// 235: 101
		int s = satan(character);
		switch (rowBrightPattern(character, s) )
		{
		case 1:
			// 1, 7
			// 中央部の縦の輝点パターン
			// 1: 1
			// 7: 101
			switch (rowBrightPattern(character, character.cols/2))
			{
			case 1:
				return 1;
			case 2:
				return 7;
			default:
				return 0;
			}
		case 2:
			// 5, 23
			// 右端の上から3分の1に暗点があるとき5
			int u = utan(character);
			if ( 0 == ((unsigned char*)character.data[character.rows/3*character.cols+u]) )
			{
				return 5;
			}
			// 2, 3
			// 右端の上から3分の2に暗点があるとき2
			if ( 0 == ((unsigned char*)character.data[character.rows/3*2*character.cols+u]) )
			{
				return 2;
			}
			return 3;
		}
	}

	return 0;
}

// 指定した列の輝点パターン数を返す
int ExtractNumber::rowBrightPattern(const cv::Mat &bin, const int column)
{
	int pattern = 0;
	bool isSearchBright = true;
	for ( int y = 0; y < bin.rows; y++ )
	{
		if ( isSearchBright && 0 < (unsigned char*)bin.data[y*bin.cols+column] )
		{
			// 輝点を探していて，かつ輝点を見つけた時
			pattern++;
			isSearchBright = false;
		}else if( !isSearchBright && 0 == (unsigned char*)bin.data[y*bin.cols+column] )
		{
			// 暗点を探していて，かつ暗点を見つけた時
			isSearchBright = true;
		}
	}
	return pattern;
}

// 指定した行の輝点パターン数を返す
int ExtractNumber::colBrightPattern(const cv::Mat &bin, const int row)
{
	int pattern = 0;
	bool isSearchBright = true;
	for ( int x = 0; x < bin.cols; x++ )
	{
		if ( isSearchBright && 0 < (unsigned char*)bin.data[row*bin.cols+x] )
		{
			// 輝点を探していて，かつ輝点を見つけた時
			pattern++;
			isSearchBright = false;
		}else if( !isSearchBright && 0 == (unsigned char*)bin.data[row*bin.cols+x] )
		{
			// 暗点を探していて，かつ暗点を見つけた時
			isSearchBright = true;
		}
	}
	return pattern;
}

// 右端を探す
int ExtractNumber::utan(const cv::Mat &bin)
{
	int sum = 0;
	for( int x = bin.cols-1; 0 <= x; x-- )
	{
		// 縦方向に走査する
		for ( int y = 0; y < bin.rows; y++ )
		{
			sum += (int)((unsigned char*)bin.data[y*bin.cols+x]);
		}
		if ( sum > 0 )
			return x;
	}
	return 0;
}

// 左端を探す
int ExtractNumber::satan(const cv::Mat &bin)
{
	int sum = 0;
	for ( int x = 0; x < bin.cols; x++ )
	{
		for ( int y = 0; y < bin.rows; y++ )
		{
			sum += (int)((unsigned char*)bin.data[y*bin.cols+x]);
		}
		if ( sum > 0 )
		{
			return x;
		}
	}
	return bin.cols-1;
}
