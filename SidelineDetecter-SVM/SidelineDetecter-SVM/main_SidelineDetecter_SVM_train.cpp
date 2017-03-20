//#include <iostream>
//#include <fstream>
//#include <strstream>
//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/objdetect/objdetect.hpp>
//#include <opencv2/ml/ml.hpp>
//
//
//using namespace std;
//using namespace cv;
//
////-----------------------�궨��----------------------------
////---------------------------------------------------------
//#define PosSamNO 22		//����������
//#define NegSamNO 56		//����������
//#define HardExampleNO 0	//HardExample�����������������HardExampleNO����0����ʾ�������ʼ���������󣬼�������HardExample����������  
////��ʹ��HardExampleʱ��������Ϊ0����Ϊ������������������������ά����ʼ��ʱ�õ����ֵ  
//
//
//#define BIAS -0.5	//��ƽ��ƫ��
//
//#define TRAIN true			//�Ƿ����ѵ��,true��ʾѵ����false��ʾ��ȡxml�ļ��е�SVMģ��
//#define SamWidth 32			//�������
//#define SamHight 16			//�����߶�
//#define VideoOutputHight 128//�����Ƶͼ��ĸ߶�
//
//#define TestImage "../Data/TestImage/5.jpg"			//���ڼ��Ĳ���ͼ��
//#define ResultImage "../Data/Result/5.jpg"			//����ͼ��ļ����
//#define TestVideo "../Data/TestVideo/BH_20160924.avi"	//���ڼ��Ĳ�����Ƶ
//#define ResultVideo "../Data/Result/BH_20160924.avi"		//������Ƶ�ļ����
//
//#define PosSetFile "../Data/PosSetResized/"			//������ͼƬ���ļ���
//#define NegSetFile "../Data/NegSetResized/"			//������ͼƬ���ļ���
//#define HardSetFile "../Data/HardSetResized/"		//����������ͼƬ���ļ���
//#define SetName "0SetName.txt"						//����ͼƬ���ļ����б�txt
//
//#define SaveSvmName "../Data/Result/SVM_HOG.xml"	//ѵ�������ģ���ļ�����
//#define LoadSvmName "../Data/Result/SVM_HOG.xml"	//�������е�ģ���ļ�����
//
////-----------------------�̳���----------------------------
////---------------------------------------------------------
////�̳���CvSVM���࣬��Ϊ����setSVMDetector()���õ��ļ���Ӳ���ʱ����Ҫ�õ�ѵ���õ�SVM��decision_func������  
////��ͨ���鿴CvSVMԴ���֪decision_func������protected���ͱ������޷�ֱ�ӷ��ʵ���ֻ�ܼ̳�֮��ͨ����������  
//class MySVM : public CvSVM  
//{  
//public:  
//	//���SVM�ľ��ߺ����е�alpha����  
//	double * get_alpha_vector()  
//	{  
//		return this->decision_func->alpha;  
//	}  
//
//	//���SVM�ľ��ߺ����е�rho����,��ƫ����  
//	float get_rho()  
//	{  
//		return this->decision_func->rho;  
//	}  
//};  
////-----------------------������----------------------------
////---------------------------------------------------------
//
//int main()  
//{
//	//��������
//	int descriptorDim = 3;//��������ά����ɫ�ʿռ�Ϊ��ά
//	MySVM svm;//SVM������
//
//	//----------------ѵ��������orֱ�Ӷ�ȡ������---------------------
//	//---------------------------------------------------------------
//	if(TRAIN) //ѵ����������������XML�ļ�
//	{
//		//0.ѵ����������
//		string ImgName;//ͼƬ��
//		ifstream finPos((string)PosSetFile+SetName);//������ͼƬ���ļ����б�  
//		ifstream finNeg((string)NegSetFile+SetName);//������ͼƬ���ļ����б�
//		ifstream finHard((string)HardSetFile+SetName);//����������ͼƬ���ļ����б� 
//		Mat sampleFeatureMat = Mat::zeros((PosSamNO+NegSamNO+HardExampleNO)*SamHight*SamWidth, descriptorDim, CV_32FC1);
//		//����ѵ������������������ɵľ��������������������ĸ���������������������ά��
//		Mat sampleLabelMat = Mat::zeros((PosSamNO+NegSamNO+HardExampleNO)*SamHight*SamWidth, 1, CV_32FC1);
//		//ѵ����������������������������������ĸ�������������1��1��ʾ�л����ˣ�-1��ʾ�޻�����  
//
//		//1.���ζ�ȡ������ͼƬ������ѵ�����ݾ���
//		for(int num=0; num<PosSamNO && getline(finPos,ImgName); num++)  
//		{
//			ImgName = PosSetFile + ImgName;//������������·����  
//			cout<<"����"<<ImgName<<endl;  
//			Mat src_BGR = imread(ImgName);//��ȡͼƬ
//			Mat src_HSV;
//			resize(src_BGR,src_BGR,Size(SamWidth,SamHight));//��ѵ��������һ��Ϊͳһ��С
//			cvtColor(src_BGR,src_HSV,CV_BGR2HSV);//ת��ΪHSV
//			//��ɫ�ʿռ������������Ƶ�������������sampleFeatureMat
//			for (int w = 0;w<SamWidth;w++)
//			{
//				for (int h = 0;h<SamHight;h++)
//				{
//					//BGR
//					float B = src_BGR.at<Vec3b>(h,w)[0];
//					float G = src_BGR.at<Vec3b>(h,w)[1];
//					float R = src_BGR.at<Vec3b>(h,w)[2];
//					//nBGR
//					float nB = B/(R+G+B);
//					float nG = G/(R+G+B);
//					float nR = R/(R+G+B);
//					//HSV
//					float H = src_HSV.at<Vec3b>(h,w)[0];
//					float S = src_HSV.at<Vec3b>(h,w)[1];
//					float V = src_HSV.at<Vec3b>(h,w)[2];
//					//��������ֵ
//					sampleFeatureMat.at<float>(num*SamHight*SamWidth+w*SamHight+h,0) = R;
//					sampleFeatureMat.at<float>(num*SamHight*SamWidth+w*SamHight+h,1) = G;
//					sampleFeatureMat.at<float>(num*SamHight*SamWidth+w*SamHight+h,2) = B;
//					//sampleFeatureMat.at<float>(num*SamHight*SamWidth+w*SamHight+h,3) = R/(R+G+B);
//					//sampleFeatureMat.at<float>(num*SamHight*SamWidth+w*SamHight+h,4) = G/(R+G+B);
//					//sampleFeatureMat.at<float>(num*SamHight*SamWidth+w*SamHight+h,5) = B/(R+G+B);
//
//					//sampleLabelMat��ֵ
//					sampleLabelMat.at<float>(num*SamHight*SamWidth+w*SamHight+h,0) = 1;//���������Ϊ1
//				}
//			}
//		}  
//
//		//2.���ζ�ȡ������ͼƬ������ѵ�����ݾ���
//		for(int num=0; num<NegSamNO && getline(finNeg,ImgName); num++)  
//		{  
//			ImgName = NegSetFile + ImgName;//������������·����
//			cout<<"����"<<ImgName<<endl;  
//			Mat src_BGR = imread(ImgName);//��ȡͼƬ
//			Mat src_HSV;
//			resize(src_BGR,src_BGR,Size(SamWidth,SamHight));//��ѵ��������һ��Ϊͳһ��С
//			cvtColor(src_BGR,src_HSV,CV_BGR2HSV);//ת��ΪHSV
//
//			//��ɫ�ʿռ������������Ƶ�������������sampleFeatureMat
//			for (int w = 0;w<SamWidth;w++)
//			{
//				for (int h = 0;h<SamHight;h++)
//				{
//					//BGR
//					float B = src_BGR.at<Vec3b>(h,w)[0];
//					float G = src_BGR.at<Vec3b>(h,w)[1];
//					float R = src_BGR.at<Vec3b>(h,w)[2];
//					//nBGR
//					float nB = B/(R+G+B);
//					float nG = G/(R+G+B);
//					float nR = R/(R+G+B);
//					//HSV
//					float H = src_HSV.at<Vec3b>(h,w)[0];
//					float S = src_HSV.at<Vec3b>(h,w)[1];
//					float V = src_HSV.at<Vec3b>(h,w)[2];
//					//��������ֵ
//					sampleFeatureMat.at<float>((num+PosSamNO)*SamHight*SamWidth+w*SamHight+h,0) = R;
//					sampleFeatureMat.at<float>((num+PosSamNO)*SamHight*SamWidth+w*SamHight+h,1) = G;
//					sampleFeatureMat.at<float>((num+PosSamNO)*SamHight*SamWidth+w*SamHight+h,2) = B;
//					//sampleFeatureMat.at<float>((num+PosSamNO)*SamHight*SamWidth+w*SamHight+h,3) = R/(R+G+B);
//					//sampleFeatureMat.at<float>((num+PosSamNO)*SamHight*SamWidth+w*SamHight+h,4) = G/(R+G+B);
//					//sampleFeatureMat.at<float>((num+PosSamNO)*SamHight*SamWidth+w*SamHight+h,5) = B/(R+G+B);
//
//					//sampleLabelMat��ֵ
//					sampleLabelMat.at<float>((num+PosSamNO)*SamHight*SamWidth+w*SamHight+h,0) = -1;//���������Ϊ-1
//				}
//			}
//		}  
//
//		//3.���ζ�ȡHardExample������ͼƬ������ѵ�����ݾ���
//		for(int num=0; num<HardExampleNO && getline(finHard,ImgName); num++)  
//		{
//			ImgName = HardSetFile + ImgName;//������������·����
//			cout<<"����"<<ImgName<<endl;  
//			Mat src_BGR = imread(ImgName);//��ȡͼƬ
//			Mat src_HSV;
//			resize(src_BGR,src_BGR,Size(SamWidth,SamHight));//��ѵ��������һ��Ϊͳһ��С
//			cvtColor(src_BGR,src_HSV,CV_BGR2HSV);//ת��ΪHSV
//
//			//��ɫ�ʿռ������������Ƶ�������������sampleFeatureMat
//			for (int w = 0;w<SamWidth;w++)
//			{
//				for (int h = 0;h<SamHight;h++)
//				{
//					//BGR
//					float B = src_BGR.at<Vec3b>(h,w)[0];
//					float G = src_BGR.at<Vec3b>(h,w)[1];
//					float R = src_BGR.at<Vec3b>(h,w)[2];
//					//nBGR
//					float nB = B/(R+G+B);
//					float nG = G/(R+G+B);
//					float nR = R/(R+G+B);
//					//HSV
//					float H = src_HSV.at<Vec3b>(h,w)[0];
//					float S = src_HSV.at<Vec3b>(h,w)[1];
//					float V = src_HSV.at<Vec3b>(h,w)[2];
//					//��������ֵ
//					sampleFeatureMat.at<float>((num+PosSamNO+NegSamNO)*SamHight*SamWidth+w*SamHight+h,0) = R;
//					sampleFeatureMat.at<float>((num+PosSamNO+NegSamNO)*SamHight*SamWidth+w*SamHight+h,1) = G;
//					sampleFeatureMat.at<float>((num+PosSamNO+NegSamNO)*SamHight*SamWidth+w*SamHight+h,2) = B;
//					//sampleFeatureMat.at<float>((num+PosSamNO+NegSamNO)*SamHight*SamWidth+w*SamHight+h,3) = R/(R+G+B);
//					//sampleFeatureMat.at<float>((num+PosSamNO+NegSamNO)*SamHight*SamWidth+w*SamHight+h,4) = G/(R+G+B);
//					//sampleFeatureMat.at<float>((num+PosSamNO+NegSamNO)*SamHight*SamWidth+w*SamHight+h,5) = B/(R+G+B);
//
//					//sampleLabelMat��ֵ
//					sampleLabelMat.at<float>((num+PosSamNO+NegSamNO)*SamHight*SamWidth+w*SamHight+h,0) = -1;//���������Ϊ-1
//				}
//			}
//		}  
//
//		//4.ѵ��SVM������  
//		//������ֹ��������������1000�λ����С��FLT_EPSILONʱֹͣ����
//		CvTermCriteria criteria = cvTermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 1000, FLT_EPSILON);  
//		//SVM������SVM����ΪC_SVC�����Ժ˺������ɳ�����C=0.01
//		CvSVMParams param(CvSVM::C_SVC, CvSVM::LINEAR, 0, 1, 0, 0.01, 0, 0, 0, criteria);  
//		cout<<"��ʼѵ��SVM������"<<endl;  
//		svm.train(sampleFeatureMat, sampleLabelMat, Mat(), Mat(), param);//ѵ��������
//		cout<<"ѵ�����"<<endl;  
//		svm.save(SaveSvmName);//��ѵ���õ�SVMģ�ͱ���Ϊxml�ļ�  
//	}  
//	else //��TRAINΪfalse����XML�ļ���ȡѵ���õķ�����  
//	{  
//		svm.load(LoadSvmName);//��XML�ļ���ȡѵ���õ�SVMģ��  
//	}  
//
//
//	//----------------��ȡģ����������w+b---------------------
//	descriptorDim = svm.get_var_count();//����������ά������ǰ��ѵ��ʱ�Ĵ�Сһ������Ӵ˾���Ϊ���ڲ�ѵ��ʱҲ���õ�ά����
//	int supportVectorNum = svm.get_support_vector_count();//֧�������ĸ���  
//	//cout<<"֧������������"<<supportVectorNum<<endl;  
//
//	Mat alphaMat = Mat::zeros(1, supportVectorNum, CV_32FC1);//alpha���������ȵ���֧����������  
//	Mat supportVectorMat = Mat::zeros(supportVectorNum, descriptorDim, CV_32FC1);//֧����������  
//	Mat resultMat = Mat::zeros(1, descriptorDim, CV_32FC1);//alpha��������֧����������Ľ��
//	float rho;//bƫ�ƵĽ��
//
//	//��֧�����������ݸ��Ƶ�supportVectorMat������  
//	for(int i=0; i<supportVectorNum; i++)  
//	{  
//		const float * pSVData = svm.get_support_vector(i);//���ص�i��֧������������ָ��  
//		for(int j=0; j<descriptorDim; j++)  
//		{  
//			//cout<<pData[j]<<" ";  
//			supportVectorMat.at<float>(i,j) = pSVData[j];  
//		}  
//	}  
//
//	//��alpha���������ݸ��Ƶ�alphaMat��  
//	double * pAlphaData = svm.get_alpha_vector();//����SVM�ľ��ߺ����е�alpha����  
//	for(int i=0; i<supportVectorNum; i++)  
//	{  
//		alphaMat.at<float>(0,i) = pAlphaData[i];  
//	}  
//
//	//����-(alphaMat * supportVectorMat),����ŵ�resultMat��  
//	//gemm(alphaMat, supportVectorMat, -1, 0, 1, resultMat);//��֪��Ϊʲô�Ӹ��ţ�  
//	//ע����Ϊsvm.predictʹ�õ���alpha*sv*another-rho�����Ϊ���Ļ�����Ϊ����������
//	//��HOG�ļ�⺯���У�ʹ��rho+alpha*sv*another(anotherΪ-1)
//	resultMat = -1 * alphaMat * supportVectorMat;  
//
//	//bƫ�ƽ��
//	rho = svm.get_rho();
//
//
//	////����w+b������
//	//vector<float> myDetector;
//
//	////��resultMat�е����ݸ��Ƶ�����myDetector��  
//	//for(int i=0; i<descriptorDim; i++)  
//	//{  
//	//	myDetector.push_back(resultMat.at<float>(0,i));  
//	//}
//	////������ƫ����rho���õ�w+b������
//	//myDetector.push_back(svm.get_rho());  
//	////cout<<"�����ά��(w+b)��"<<myDetector.size()<<endl;
//	////cout<<myDetector.at(0)<<endl<<myDetector.at(1)<<endl<<myDetector.at(2)<<endl<<myDetector.at(3)<<endl;
//	//float w1 = myDetector.at(0),w2 = myDetector.at(1),w3 = myDetector.at(2),b = myDetector.at(3);
//
//
//	////-----------------------------����ͼ��ָ�---------------------------------
//	////��������
//	//Mat src = imread(TestImage); //��ȡ����ͼ��
//	//Mat dst = Mat::zeros(src.rows,src.cols,CV_8UC1);//���ͼ��
//
//	////����ͼ��ָ�����ز��������ٶȽ�����
//	//for (int r = 0;r<src.rows;r++)
//	//{
//	//	for (int c = 0;c<src.cols;c++)
//	//	{
//	//		//cout<<"a"<<float(src.at<Vec3b>(r,c)[0])<<"b"<<float(src.at<Vec3b>(r,c)[1])<<"c"<<float(src.at<Vec3b>(r,c)[2]);
//	//		Mat tempMat = (Mat_<float>(1,3) << src.at<Vec3b>(r,c)[0],src.at<Vec3b>(r,c)[1],src.at<Vec3b>(r,c)[2]);
//	//		float response = svm.predict(tempMat);  
//
//	//		if (response > 0)  
//	//			dst.at<uchar>(r,c) = 255;
//	//		//else if (response == -1)
//	//		//	src.at<uchar>(r, c)  = 0;
//	//	}
//	//}
//	////������ͼ����
//	//imwrite(ResultImage,dst);  
//	//namedWindow("dst");  
//	//imshow("dst",dst);  
//	//waitKey(0);//ע�⣺imshow֮������waitKey�������޷���ʾͼ��  
//
//	////-----------------------------end---------------------------------
//
//	//-----------------------------������Ƶͼ��ָ�---------------------------------
//	//��������
//	VideoCapture myVideo(TestVideo);//��ȡ��Ƶ  
//	Mat src_BGR,src_HSV,src_BGR_32FC3,src_HSV_32FC3,gray;
//	Mat src_BGR_Channels[3],src_HSV_Channels[3];
//	Mat R,G,B;//RGB��ͨ��
//	Mat nR,nG,nB;//nRGB��ͨ��
//	Mat H,S,V;//HSV��ͨ��
//	Mat rThr,gThr,bThr,wThr,threshold;//��ֵͼ��
//
//	//����Ƶ
//	if(!myVideo.isOpened()){cout<<"��Ƶ��ȡ����"<<endl;system("puase");return -1;}
//
//	//�������ɵ���Ƶ
//	double videoRate=myVideo.get(CV_CAP_PROP_FPS);//��ȡ֡��
//	int videoWidth=myVideo.get(CV_CAP_PROP_FRAME_WIDTH);//��ȡ��Ƶͼ����
//	int videoHight=myVideo.get(CV_CAP_PROP_FRAME_HEIGHT);//��ȡ��Ƶͼ��߶�
//	int videoDelay=1000/videoRate;//ÿ֮֡����ӳ�����Ƶ��֡�����Ӧ�������ܳ����ʱ�򲥷���Ƶ�����ʣ�
//	VideoWriter outputVideo(ResultVideo, CV_FOURCC('M', 'J', 'P', 'G'), videoRate,Size(VideoOutputHight*videoWidth/videoHight,VideoOutputHight));//������Ƶ��
//	Mat videoImage = Mat::zeros(Size(VideoOutputHight*videoWidth/videoHight,VideoOutputHight),CV_8UC3);
//
//
//	//��ʼ��Ƶ����
//	bool stop = false;
//	while (!stop)
//	{
//		//��������
//		if (!myVideo.read(src_BGR)){cout<<"��Ƶ����"<<endl;waitKey(0); break;}//��ȡ��Ƶ֡
//
//		//�ı���Ƶͼ��ߴ�
//		resize(src_BGR,src_BGR,Size(VideoOutputHight*videoWidth/videoHight,VideoOutputHight));
//		cvtColor(src_BGR,src_HSV,CV_BGR2HSV);//ת��ΪHSV
//		cvtColor(src_BGR,gray,CV_BGR2GRAY);//ת��Ϊ�Ҷ�ͼ
//		src_BGR.convertTo(src_BGR_32FC3,CV_32FC3);//ת��ͼ����������
//		src_HSV.convertTo(src_HSV_32FC3,CV_32FC3);//ת��ͼ����������
//		split(src_BGR_32FC3,src_BGR_Channels);//����RGBͨ��
//		split(src_HSV_32FC3,src_HSV_Channels);//����HSVͨ��
//		B = src_BGR_Channels[0];G = src_BGR_Channels[1];R = src_BGR_Channels[2];
//		nB = B/(R+G+B);nG = G/(R+G+B);nR = R/(R+G+B);
//		H = src_HSV_Channels[0];S = src_HSV_Channels[1];V = src_HSV_Channels[2];
//
//		//����ͼ��ָɫ�ʿռ�SVM����������������������ٶȿ죩
//		threshold = resultMat.at<float>(0)*R + resultMat.at<float>(1)*G + resultMat.at<float>(2)*B + rho > BIAS;
//		//threshold = - (0.273370087)*R - (-0.377990603)*G - (0.350543886)*B + (32.809016951026422) > BIAS;//0.273370087��Ϊxml�е�����
//
//		////����ͼ��ָ��ֵ����
//		//rThr = (R/(G+B))>0.7;
//		//gThr = (R<150)&(B<150)&(G>R+5)&(G>B+5);
//		//bThr = (R>110)&(G>110)&(B>110)&(B>R-5)&(B>G-5);
//		//wThr = gray>200;
//		//threshold = ~(bThr|wThr|rThr);
//
//		//����������Ƶ��������������������������
//		for (int r = 0;r<src_BGR.rows;r++)
//		{
//			for (int c = 0;c<src_BGR.cols;c++)
//			{
//				if (threshold.at<uchar>(r,c)>0)
//				{
//					videoImage.at<Vec3b>(r,c) = Vec3b(255,255,255);
//				} 
//				else
//				{
//					videoImage.at<Vec3b>(r,c) = Vec3b(0,0,0);
//				}
//
//			}
//		}
//
//		////����ͼ��ָ�����ز��������ٶȽ�����
//		//Mat dst = Mat::zeros(VideoOutputHight,VideoOutputHight*videoWidth/videoHight,CV_8U);//���ͼ��
//		//for (int r = 0;r<src.rows;r++)
//		//{
//		//	for (int c = 0;c<src.cols;c++)
//		//	{
//		//		//cout<<"a"<<float(src.at<Vec3b>(r,c)[0])<<"b"<<float(src.at<Vec3b>(r,c)[1])<<"c"<<float(src.at<Vec3b>(r,c)[2]);
//		//		Mat tempMat = (Mat_<float>(1,3) << src.at<Vec3b>(r,c)[0],src.at<Vec3b>(r,c)[1],src.at<Vec3b>(r,c)[2]);
//		//		float response = svm.predict(tempMat);  
//		//		if (response == 1)  
//		//			dst.at<uchar>(r,c) = 255;
//		//		else if (response == -1)
//		//			dst.at<uchar>(r, c)  = 0;
//
//		//		//����������Ƶ��������������������������
//		//		if (response == 1)  
//		//			src.at<Vec3b>(r,c) = Vec3b(255,255,255);
//		//		else if (response == -1)
//		//			src.at<Vec3b>(r,c) = Vec3b(0,0,0);
//		//	}
//		//}
//
//		//������Ƶͼ��
//		outputVideo<<videoImage;
//		imshow("videoImage",videoImage);
//		imshow("src",src_BGR);
//
//		//dst��threshold���Ǹõõ��Ķ�ֵͼ�񣬵��Ƕ�ֵͼ����д����Ƶ
//
//
//		if(waitKey(1)>=0)stop = true;//ͨ������ֹͣ��Ƶ
//	}
//	//-----------------------------end---------------------------------
//
//}  