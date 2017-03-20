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
//#define PosSamNO 25		//����������
//#define NegSamNO 94		//����������
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
//#define TestVideo "../Data/TestVideo/bh_20160923_celue.avi"	//���ڼ��Ĳ�����Ƶ
//#define ResultVideo "../Data/Result/bh_20160923_celue.avi"		//������Ƶ�ļ����
//
//#define PosSetFile "../Data/PosSetResized/"			//������ͼƬ���ļ���
//#define NegSetFile "../Data/NegSetResized/"			//������ͼƬ���ļ���
//#define HardSetFile "../Data/HardSetResized/"		//����������ͼƬ���ļ���
//#define SetName "0SetName.txt"						//����ͼƬ���ļ����б�txt
//
//#define SaveSvmName "../Data/Result/SVM_HOG.xml"	//ѵ�������ģ���ļ�����
//#define LoadSvmName "../Data/Result/SVM_HOG.xml"	//�������е�ģ���ļ�����
//
////---------------------------------------------------------
//
//int main()  
//{
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
//	Mat rThr,gThr,bThr,wThr,threshold1,threshold2,threshold;//��ֵͼ��
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
//	Mat videoImage = Mat::zeros(Size(videoWidth,videoHight),CV_8UC3);
//
//	//��ʼ��Ƶ����
//	bool stop = false;
//	while (!stop)
//	{
//		//��������
//		if (!myVideo.read(src_BGR)){cout<<"��Ƶ����"<<endl;waitKey(0); break;}//��ȡ��Ƶ֡
//
//		//�ı���Ƶͼ��ߴ�
//		
//		//cvtColor(src_BGR,src_HSV,CV_BGR2HSV);//ת��ΪHSV
//		//cvtColor(src_BGR,gray,CV_BGR2GRAY);//ת��Ϊ�Ҷ�ͼ
//		src_BGR.convertTo(src_BGR_32FC3,CV_32FC3);//ת��ͼ����������
//		//src_HSV.convertTo(src_HSV_32FC3,CV_32FC3);//ת��ͼ����������
//		split(src_BGR_32FC3,src_BGR_Channels);//����RGBͨ��
//		//split(src_HSV_32FC3,src_HSV_Channels);//����HSVͨ��
//		B = src_BGR_Channels[0];G = src_BGR_Channels[1];R = src_BGR_Channels[2];
//		//nB = B/(R+G+B);nG = G/(R+G+B);nR = R/(R+G+B);
//		//H = src_HSV_Channels[0];S = src_HSV_Channels[1];V = src_HSV_Channels[2];
//
//		//����ͼ��ָɫ�ʿռ�SVM����������������������ٶȿ죩
//		//threshold = resultMat.at<float>(0)*R + resultMat.at<float>(1)*G + resultMat.at<float>(2)*B + rho > BIAS;
//		//-2.66061854e-002 1.34229800e-002 2.48698164e-002 -9.9427669280579056e-001;
//		//5.06762154e-002 -2.20872685e-001 2.25031987e-001 2.3481620039919280e+000;
//		threshold1 = -(-0.02661)*R-(0.01342)*G-(0.02487)*B+(-0.9943)>-0.5;//r
//		//threshold2 = -(0.03109)*R-(-0.05308)*G-(0.03566)*B+(0.3493)>-0.1;//g
//		threshold2 = -(0.050676)*R-(-0.2209)*G-(0.2250)*B+(2.34816)>0.5;//g
//		threshold = threshold1 | threshold2;
//		morphologyEx(threshold,threshold,MORPH_OPEN,getStructuringElement(MORPH_ELLIPSE, Size(3,3)));//��̬ѧ�˲�������ȥ��//#9.23
//		morphologyEx(threshold,threshold,MORPH_CLOSE,getStructuringElement(MORPH_ELLIPSE, Size(3,3)));//��̬ѧ�˲�������ȥ��//#9.23
//		//////////////////////////threshold2 = -(0.050676)*R-(-0.2209)*G-(0.2250)*B+(2.34816)>0.5;//g
//		//medianBlur(threshold,threshold,5);
//
//		//threshold = - (0.273370087)*R - (-0.377990603)*G - (0.350543886)*B + (32.809016951026422) > BIAS;//0.273370087��Ϊxml�е�����
//
//		////����ͼ��ָ��ֵ����
//		//rThr = (R/(G+B))>0.7;
//		//gThr = (R<150)&(B<150)&(G>R+5)&(G>B+5);
//		//bThr = (R>110)&(G>110)&(B>110)&(B>R-5)&(B>G-5);
//		//wThr = gray>200;
//		//threshold = ~(bThr|wThr|rThr);
//
//		////����������Ƶ��������������������������
//		//for (int r = 0;r<src_BGR.rows;r++)
//		//{
//		//	for (int c = 0;c<src_BGR.cols;c++)
//		//	{
//		//		if (threshold.at<uchar>(r,c)>0)
//		//		{
//		//			videoImage.at<Vec3b>(r,c) = Vec3b(255,255,255);
//		//		} 
//		//		else
//		//		{
//		//			videoImage.at<Vec3b>(r,c) = Vec3b(0,0,0);
//		//		}
//
//		//	}
//		//}
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
//		//outputVideo<<videoImage;
//		namedWindow("threshold",WINDOW_NORMAL);
//		imshow("threshold",threshold);
//		namedWindow("src",WINDOW_NORMAL);
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