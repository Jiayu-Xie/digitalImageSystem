#include "snapshot.h"

snapshot::snapshot() {

}
snapshot::~snapshot() {

}

wchar_t* snapshot::multi_Byte_To_Wide_Char(const string& str) {

	//string ת char*
	const char* pCStrKey = str.c_str();
	//��һ�ε��÷���ת������ַ������ȣ�����ȷ��Ϊwchar_t*���ٶ����ڴ�ռ�
	int pSize = MultiByteToWideChar(CP_OEMCP, 0, pCStrKey, strlen(pCStrKey) + 1, NULL, 0);
	wchar_t *pWCStrKey = new wchar_t[pSize];
	//�ڶ��ε��ý����ֽ��ַ���ת����˫�ֽ��ַ���
	MultiByteToWideChar(CP_OEMCP, 0, pCStrKey, strlen(pCStrKey) + 1, pWCStrKey, pSize);
	return pWCStrKey;
}

int snapshot::putText(cv::Mat& image, const string& str, cv::Point pos, cv::Scalar color, double fontScale, string fontPath, double fontInterval) {

	this->addWaterMark_img = image;
	if (!addWaterMark_img.data || addWaterMark_img.channels() != 3) {
		fprintf(stderr, "read image fail\n");
		return -1;
	}

	//��������ļ�Ϊ�գ�������Ĭ����������
	//����·���������·��
	if (fontPath.empty()) {
		fontPath = "../test-data/fonts/simsun.ttc";
	}
	//stringתΪconst char*����
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
//��ֵ���ӽ���
cv::Mat snapshot::MeanNoiseReduction(vector<cv::Mat> &matArr, int num) {
	cout << "����MeanNoiseReduction" << endl;
	cv::Mat MeanMat;
	//cout << "create MeanMat" << endl;
	if (num > 0) {
		MeanMat = matArr[0].clone();
		for (int i = 1; i < num; i++) {
			//֮ǰ�����mat��8λ�޷���������Ӻ��ֵ�������255�����Ϊ255��
			//���ｫ���֮��Ľ������ΪCV_32S,���֮����Եõ������ֵ������֮�����ƽ��
			cv::add(MeanMat, matArr[i], MeanMat, cv::Mat(), CV_32S);
		}
		MeanMat = MeanMat / num;
		//cout << "����ƽ������" << endl;
		return MeanMat;

	}
	else {
		cout << "mat������û��mat��Ա" << endl;
		return MeanMat;
	}


}
//��ֵ���ӽ��룬������ص�����
cv::Mat snapshot::MedianNoiseReduction(vector<cv::Mat> &matArr, int num) {
	cout << "����MedianNoisrReduction" << endl;
	cv::Mat MedianMat;
	if (num <= 0) {
		cout << "mat������û��mat��Ա" << endl;
		return MedianMat;
	}
	MedianMat = matArr[0].clone();
	//cout << "����MedianMat" << endl;
	//����ǻҶ�ͼ��channels��1
	//����ǲ�ɫͼ��channels��3
	int cols = matArr[0].cols*matArr[0].channels();
	int rows = matArr[0].rows;
	//һ��num��ͼ��median_num��ʾ�м�����
	int median_num = 0;
	if (num % 2 == 0)
		median_num = num / 2;
	else
		median_num = num / 2 + 1;
	/*cout << "cols is:" << cols << endl;
	cout << "rows is:" << rows << endl;
	cout << "median_num is:" << median_num << endl;*/
	//arr��ʱ�洢ͬһ��λ�õ�����ֵ�������
	int *arr = new int[num];
	for (int i = 0; i < rows; i++) {
		//cout << "i is:" << i << endl;
		for (int j = 0; j < cols; j++) {
			//matArr��ʼ��
			for (int k = 0; k < num; k++) {
				arr[k] = matArr[k].at<uchar>(i, j);
			}
			//matArr��С��������
			quickSort(0, num - 1, arr);
			//����ֵ���浽Median_Mat��Ӧ��λ����
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
	base = arr[left];  //ȡ����ߵ���Ϊ��׼��
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
	//��׼����λ
	arr[left] = arr[i];
	arr[i] = base;
	quickSort(left, i - 1, arr);//�ݹ����
	quickSort(i + 1, right, arr);//�ݹ��ұ�
}

void snapshot::TestAddWaterMark() {
	string filename = "../test-data/images/37.jpg";
	//imread�������ڶ�������Ĭ��Ϊ1��ԭͼ���أ�0��ʾ�Ҷ�ͼ���ء�
	cv::Mat img = cv::imread(filename, 1);
	if (!img.data || img.channels() != 3) {
		fprintf(stderr, "read image fail\n");
		return ;
	}
	else {
		cout << "image ok" << endl;
	}
	const string str = "ˮӡ��Ϣ��Ӳ���hello";

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