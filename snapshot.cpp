#include "snapshot.h"

snapshot::snapshot() {

}
snapshot::~snapshot() {

}

wchar_t* snapshot::multi_Byte_To_Wide_Char(const string& str) {

	//string 转 char*
	const char* pCStrKey = str.c_str();
	//第一次调用返回转换后的字符串长度，用于确认为wchar_t*开辟多大的内存空间
	int pSize = MultiByteToWideChar(CP_OEMCP, 0, pCStrKey, strlen(pCStrKey) + 1, NULL, 0);
	wchar_t *pWCStrKey = new wchar_t[pSize];
	//第二次调用将单字节字符串转换成双字节字符串
	MultiByteToWideChar(CP_OEMCP, 0, pCStrKey, strlen(pCStrKey) + 1, pWCStrKey, pSize);
	return pWCStrKey;
}

int snapshot::putText(cv::Mat& image, const string& str, cv::Point pos, cv::Scalar color, double fontScale, string fontPath, double fontInterval) {

	this->addWaterMark_img = image;
	if (!addWaterMark_img.data || addWaterMark_img.channels() != 3) {
		fprintf(stderr, "read image fail\n");
		return -1;
	}

	//如果字体文件为空，则设置默认字体宋体
	//字体路径采用相对路径
	if (fontPath.empty()) {
		fontPath = "../test-data/fonts/simsun.ttc";
	}
	//string转为const char*类型
	CvxText cvxtext(fontPath.c_str());
	cv::Scalar size{ fontScale,0,fontInterval,0 };

	wchar_t * wStr = multi_Byte_To_Wide_Char(str);
	cvxtext.setFont(nullptr, &size, nullptr, 0);
	int i = 0;
	i = cvxtext.putText(addWaterMark_img, wStr, pos, color);

	delete[]wStr;
	return i;
}
void snapshot::storeImage(string filename) {
	if (filename.empty()) {
		cout << "the file path is empty" << endl;
		return;
	}
	cv::imwrite(filename, addWaterMark_img);
}
//均值叠加降噪
cv::Mat snapshot::MeanNoiseReduction(vector<cv::Mat> &matArr, int num) {
	cout << "进入MeanNoiseReduction" << endl;
	cv::Mat MeanMat;
	//cout << "create MeanMat" << endl;
	if (num > 0) {
		MeanMat = matArr[0].clone();
		for (int i = 1; i < num; i++) {
			//之前读入的mat是8位无符号数，相加后的值如果超过255，会变为255。
			//这里将相加之后的结果设置为CV_32S,相加之后可以得到更大的值，便于之后的求平均
			cv::add(MeanMat, matArr[i], MeanMat, cv::Mat(), CV_32S);
		}
		MeanMat = MeanMat / num;
		//cout << "计算平均结束" << endl;
		return MeanMat;

	}
	else {
		cout << "mat数组中没有mat成员" << endl;
		return MeanMat;
	}


}
//中值叠加降噪，逐个像素点运算
cv::Mat snapshot::MedianNoiseReduction(vector<cv::Mat> &matArr, int num) {
	cout << "进入MedianNoisrReduction" << endl;
	cv::Mat MedianMat;
	if (num <= 0) {
		cout << "mat数组中没有mat成员" << endl;
		return MedianMat;
	}
	MedianMat = matArr[0].clone();
	//cout << "创建MedianMat" << endl;
	//如果是灰度图，channels是1
	//如果是彩色图，channels是3
	int cols = matArr[0].cols*matArr[0].channels();
	int rows = matArr[0].rows;
	//一共num张图像，median_num表示中间的序号
	int median_num = 0;
	if (num % 2 == 0)
		median_num = num / 2;
	else
		median_num = num / 2 + 1;
	/*cout << "cols is:" << cols << endl;
	cout << "rows is:" << rows << endl;
	cout << "median_num is:" << median_num << endl;*/
	//arr临时存储同一个位置的像素值（排序后）
	int *arr = new int[num];
	for (int i = 0; i < rows; i++) {
		//cout << "i is:" << i << endl;
		for (int j = 0; j < cols; j++) {
			//matArr初始化
			for (int k = 0; k < num; k++) {
				arr[k] = matArr[k].at<uchar>(i, j);
			}
			//matArr从小到大排序
			quickSort(0, num - 1, arr);
			//将中值保存到Median_Mat相应的位置中
			MedianMat.at<uchar>(i, j) = arr[median_num];
		}
	}
	return MedianMat;
}

void snapshot::quickSort(int left, int right, int* arr) {
	if (left >= right)
		return;
	int i, j, base, temp;
	i = left, j = right;
	base = arr[left];  //取最左边的数为基准数
	while (i < j)
	{
		while (arr[j] >= base && i < j)
			j--;
		while (arr[i] <= base && i < j)
			i++;
		if (i < j)
		{
			temp = arr[i];
			arr[i] = arr[j];
			arr[j] = temp;
		}
	}
	//基准数归位
	arr[left] = arr[i];
	arr[i] = base;
	quickSort(left, i - 1, arr);//递归左边
	quickSort(i + 1, right, arr);//递归右边
}

void snapshot::TestAddWaterMark() {
	string filename = "../test-data/images/37.jpg";
	//imread函数，第二个参数默认为1，原图返回；0表示灰度图返回。
	cv::Mat img = cv::imread(filename, 1);
	if (!img.data || img.channels() != 3) {
		fprintf(stderr, "read image fail\n");
		return ;
	}
	else {
		cout << "image ok" << endl;
	}
	const string str = "水印信息添加测试hello";

	putText(img, str, cv::Point(50, 100), cv::Scalar(0, 255, 0));
	string new_filename = filename.insert(filename.length() - 4, "_processed");
	storeImage(new_filename);

}

void snapshot::TestMeanNoiseReduction() {
	vector<cv::Mat> matArr;
	string str = "../test-data/images/";
	int num = 5;
	for (int i = 1; i < num+1; i++) {
		string filename = str + to_string(i) + ".jpg";
		//cout << i << " is " << filename << endl;
		cv::Mat img = cv::imread(filename, 1);
		matArr.push_back(img);
	}
	string new_filename = str + "_mean.jpg";

	cv::Mat new_mat = MeanNoiseReduction(matArr, num);
	/*cv::imshow("new_img", new_mat);
	cv::waitKey(0);*/
	cv::imwrite(new_filename, new_mat);
}

void snapshot::TestMedianNoiseReduction() {
	vector<cv::Mat> matArr;
	string str = "../test-data/images/";
	for (int i = 1; i < 6; i++) {
		string filename = str + to_string(i) + ".jpg";
		//cout << i << " is " << filename << endl;
		cv::Mat img = cv::imread(filename, 1);
		matArr.push_back(img);
	}
	string new_filename = str + "_median.jpg";
	cv::Mat new_mat = MedianNoiseReduction(matArr, 5);
	cv::imshow("new_img", new_mat);
	cv::waitKey(0);
	cv::imwrite(new_filename, new_mat);
}


void snapshot::printInfo() {
	cout << "image process print" << endl;
}