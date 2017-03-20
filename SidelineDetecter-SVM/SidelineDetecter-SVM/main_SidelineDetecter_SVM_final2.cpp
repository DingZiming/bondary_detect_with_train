//#include <iostream>
//#include <fstream>
//#include <strstream>
//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/objdetect/objdetect.hpp>
//#include <opencv2/ml/ml.hpp>
//
//using namespace std; 
//using namespace cv;
////-----------------------�궨��----------------------------
//#define VideoOutputHight 128		//�����Ƶͼ��ĸ߶�
//#define xMin 0						//���ط�Χ#8.27
//#define xMax 20						//���ط�Χ#8.27
//#define yMin 0						//���ط�Χ#8.27
//#define yMax 20						//���ط�Χ#8.27
//#define BIAS -0.5					//��ƽ��ƫ��
//#define TestVideo "../Data/TestVideo/out9.15.avi"	//���ڼ��Ĳ�����Ƶ
//#define ResultVideo "../Data/Result/out9.15.avi"		//������Ƶ�ļ����
//#define LoadSvmName "../Data/Result/SVM_HOG.xml"	//�������е�ģ���ļ�����
//
//
////-----------------------�̳���----------------------------
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
//
//enum xSideType{noxSide,leftSide,rightSide};
//enum ySideType{noySide,topSide,bottomSide};
//
////�����P0ָ��P1��P2����ֱ�ߵ��������س���
//double computeDistance(Point2d P0,Point2d P1,Point2d P2);
////�����س���ת��Ϊʵ�ʳ���
//double imgDis2realDis(double h, double f,double imgDis);
//
//
////*******************************�������********************************
////����
//Mat src;
//double quadHeight = 1.4;//������߶�
//double camF = 419.0;//��������س���
//double angleBias = 0.0;//�߽ǶȽ�������������ͼ����Ӧ�еĽǶ�(��ʱ��Ϊ��)
////���
//xSideType xSide;//���߽��
//ySideType ySide;//���߽��
//Point2d Px1,Px2;//���߶ζ˵㣨û�ҵ��򷵻�-1,-1��
//Point2d Py1,Py2;//���߶ζ˵㣨û�ҵ��򷵻�-1,-1��
//double xDis;//��������������루������ͼ�����ʱΪ������û�ҵ��򷵻�10000��
//double yDis;//��������������루������ͼ���±�ʱΪ������û�ҵ��򷵻�10000��
//Point3d outputResult = Point3d(999,999,0);//������꣺x��y����־λ#8.27
////x������Ϊ������y������Ϊ������(û���ҵ���Ϊ999)����־λ��(0-�ޱ߽磬1-y���£�2-x���£�3-xy����)#8.27
//
////*********************************end***********************************
//int main()
//{
//	//----------------��ȡģ����������w+b---------------------
//	//��������
//	MySVM svm;//SVM������
//	svm.load(LoadSvmName);//��XML�ļ���ȡѵ���õ�SVMģ��
//	int descriptorDim = 3;//��������ά����ɫ�ʿռ�Ϊ��ά
//	int supportVectorNum = svm.get_support_vector_count();//֧�������ĸ���  
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
//			supportVectorMat.at<float>(i,j) = pSVData[j];  
//		}  
//	}  
//	//��alpha���������ݸ��Ƶ�alphaMat��  
//	double * pAlphaData = svm.get_alpha_vector();//����SVM�ľ��ߺ����е�alpha����  
//	for(int i=0; i<supportVectorNum; i++)  
//	{  
//		alphaMat.at<float>(0,i) = pAlphaData[i];  
//	}  
//	//����-(alphaMat * supportVectorMat),����ŵ�resultMat��  
//	resultMat = -1 * alphaMat * supportVectorMat;  
//	//bƫ�ƽ��
//	rho = svm.get_rho();
//
//	//-----------------------��Ƶ����--------------------------
//	//����Ƶ
//	VideoCapture cap(TestVideo);
//	if(!cap.isOpened()){cout<<"��Ƶ��ȡ����"<<endl;system("puase");return -1;}
//	//��ȡ��Ƶ��Ϣ
//	double rate=cap.get(CV_CAP_PROP_FPS);//��ȡ֡��
//	int delay=1000/rate;//ÿ֮֡����ӳ�����Ƶ��֡�����Ӧ
//	double width=cap.get(CV_CAP_PROP_FRAME_WIDTH);//��ȡͼ����
//	double height=cap.get(CV_CAP_PROP_FRAME_HEIGHT);//��ȡͼ��߶�
//	//���������Ƶ
//	VideoWriter outputVideo(ResultVideo, CV_FOURCC('M', 'J', 'P', 'G'), rate, Size(VideoOutputHight*width/height,VideoOutputHight));//д��Ƶ��
//
//	//��������
//	Mat src32FC3,threshold,edges;//����֡��ת���������ͣ��Ҷ�ͼ����ֵͼ����Եͼ
//	Mat srcChannels[3];//RBG
//	Mat R,G,B;//RGB��ͨ��
//	vector<Vec4i> lines,vlines,hlines;//lines�߶�ʸ������(�У��У��У���)��vlines�����߼��ϣ�hlines�����߼���
//	Point2d Pcenter = Point2d(VideoOutputHight*width/(2*height),VideoOutputHight/2);//ͼ�����ĵ�����
//
//	//��ʼ��Ƶ����
//	bool stop = false;
//	while(!stop)
//	{
//		//��ȡͼ��
//		if(!cap.read(src)){cout<<"��Ƶ����"<<endl;waitKey(0);break;}
//
//		//ͼ��Ԥ����
//		resize(src,src,Size(VideoOutputHight*width/height,VideoOutputHight));//������С
//		//GaussianBlur(src,src,Size(7,7),1.5);//��˹�˲�
//		src.convertTo(src32FC3,CV_32FC3);//ת��ͼ����������
//		split(src32FC3,srcChannels);//����RGBͨ��
//		B = srcChannels[0];G = srcChannels[1];R = srcChannels[2];
//
//		//����ͼ��ָɫ�ʿռ�SVM����������������������ٶȿ죩
//		threshold = resultMat.at<float>(0)*R + resultMat.at<float>(1)*G + resultMat.at<float>(2)*B + rho > BIAS;
//		//threshold = - 0.273370087*B - (-0.377990603)*G - 0.350543886*R + 32.809016951026422 > 0;//0.273370087��Ϊxml�е�����
//
//		imshow("s22",threshold);
//		waitKey(1);
//
//		//��̬ѧ�˲�
//		morphologyEx(threshold,threshold,MORPH_CLOSE,getStructuringElement(MORPH_ELLIPSE, Size(9,9)));//��̬ѧ�˲�������ȥ��
//		morphologyEx(threshold,threshold,MORPH_OPEN,getStructuringElement(MORPH_ELLIPSE, Size(11,11)));//��̬ѧ�˲�������ȥ��
//		
//
//
//		//��̬ѧ�˲�����Ե���
//		morphologyEx(threshold,edges,MORPH_GRADIENT ,getStructuringElement(MORPH_RECT, Size(7,7)));
//
//		//���л���任
//		HoughLinesP(edges,lines,1,CV_PI/360,48,48,0);
//		/*	�����������Accumulator threshold parameter. Only those lines are returned that get enough votes (  ).
//			������������double���͵�minLineLength����Ĭ��ֵ0����ʾ����߶εĳ��ȣ�������趨�����̵��߶ξͲ��ܱ����ֳ�����
//			���߸�������double���͵�maxLineGap����Ĭ��ֵ0������ͬһ�е����֮���������������ľ���*/
//
//		//�ҳ����߼���
//		vlines.clear();
//		vector<double> anglex;//���߼�����߽�ļнǾ���ֵ
//		for( size_t i = 0; i < lines.size(); i++ )//�ҳ����߼���
//		{
//			Vec4i l = lines[i];
//			double angle = atan(double(l[2]-l[0])/double(l[3]-l[1]))/CV_PI*180;//������y�����ʽǶ�
//			if (abs(angle-angleBias)<15)
//			{
//				vlines.push_back(l);
//				anglex.push_back(abs(angle-angleBias));
//				line( src, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,255,255), 1, CV_AA);//���ߣ�point:(�У���)
//			}
//		}
//
//		//�ҳ�����
//		if (vlines.size()>0)
//		{
//			vector<double> anglexS(anglex);//�ǶȴӴ�С����
//			sort(anglexS.begin(),anglexS.end());
//
//			for (size_t i = 0;i < anglex.size();i++)
//			{
//				//cout<<anglexS[i]<<" ";
//				if (anglex[i] == anglexS[anglexS.size()-1])//ȷ������
//				{
//					//cout<<"minx"<<anglex[i];
//					Px1 = Point2d(vlines[i][0],vlines[i][1]);
//					Px2 = Point2d(vlines[i][2],vlines[i][3]);
//
//					//���ຯ��ϵ��
//					int f2a = (Px1.y-Px2.y)*(Px1.x-Px2.x);
//					int f2b = -(Px1.y-Px2.y)*(Px1.y-Px2.y);
//					int f2c = (Px1.y-Px2.y)*(Px1.y*Px2.x-Px2.y*Px1.x);
//
//					//���㳤��
//					double xImgDis = computeDistance(Pcenter,Px1,Px2);//�������ĵ㵽���ߵ����س���
//					if (f2a*Pcenter.y+f2b*Pcenter.x+f2c<0)//���븳���ţ��������ұ�
//						xImgDis = -xImgDis;
//					xDis = imgDis2realDis(quadHeight,camF,xImgDis * height/VideoOutputHight);//�����ؾ���ת����ʵ�ʾ���
//					cout<<"xImgDis:"<<xImgDis<<endl;
//					cout<<"xDis:"<<xDis<<endl;
//
//					//�ж���������
//					int countLeft = 0,countRight = 0;//����ֵ
//					int areaLeft = 0,areaRight = 0;//���
//					for (int xi = 0;xi<threshold.cols;xi++)
//					{
//						for (int yi = 0;yi<threshold.rows;yi++)
//						{
//							if (f2a*yi+f2b*xi+f2c>0)//��
//							{
//								areaLeft++;
//								if (threshold.at<uchar>(yi,xi)!=0)
//								{
//									countLeft++;
//								} 
//							} 
//							else//��
//							{
//								areaRight++;
//								if (threshold.at<uchar>(yi,xi)!=0)
//								{
//									countRight++;
//								}
//							}
//
//						}
//					}
//					if (1.0*countLeft/areaLeft>0.6)//�ж�����
//					{
//						cout<<"left"<<1.0*countLeft/areaLeft<<endl;
//						xSide = leftSide;
//						line( src, Px1, Px2, Scalar(255,0,0), 1, CV_AA);//���ߣ�point:(�У���)
//
//					} 
//					else if (1.0*countRight/areaRight>0.6)
//					{
//						cout<<"right"<<1.0*countRight/areaRight<<endl;
//						xSide = rightSide;
//						line( src, Px1, Px2, Scalar(0,255,0), 1, CV_AA);//���ߣ�point:(�У���)
//
//					}
//					else
//					{
//						cout<<"xfalse"<<endl;
//						xSide = noxSide;
//					}
//					break;
//				}
//			}
//		}
//		else
//		{
//			xSide = noxSide;
//			Px1 = Point2d(-1,-1);
//			Px2 = Point2d(-1,-1);
//			xDis = 10000;
//		}
//
//		//�ҳ����߼���
//		hlines.clear();
//		vector<double> angley;//���߼��Ϻͱ߽�ļнǾ���ֵ
//		for( size_t i = 0; i < lines.size(); i++ )//�ҳ����߼���
//		{
//			Vec4i l = lines[i];
//			double angle = atan(double(l[2]-l[0])/double(l[3]-l[1]))/CV_PI*180;//������y�����ʽǶ�
//			if (abs(angle-90-angleBias)<15)
//			{
//				hlines.push_back(l);
//				angley.push_back(abs(angle-90-angleBias));
//				line( src, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,255,255), 1, CV_AA);//���ߣ�point:(�У���)
//			}
//		}
//
//		//�ҳ�����
//		if (hlines.size()>0)
//		{
//			vector<double> angleyS(angley);//�ǶȴӴ�С����
//			sort(angleyS.begin(),angleyS.end());
//			for (size_t i = 0;i < angley.size();i++)
//			{
//				//cout<<angleyS[i]<<" ";
//				if (angley[i] == angleyS[angleyS.size()-1])//ȷ������
//				{
//					//cout<<"miny"<<angley[i];
//					Py1 = Point2d(hlines[i][0],hlines[i][1]);
//					Py2 = Point2d(hlines[i][2],hlines[i][3]);
//
//					//���ຯ��ϵ��
//					int f1a = (Py1.x-Py2.x)*(Py1.y-Py2.y);
//					int f1b = -(Py1.x-Py2.x)*(Py1.x-Py2.x);
//					int f1c = (Py1.x-Py2.x)*(Py1.x*Py2.y-Py2.x*Py1.y);
//
//					//���㳤��
//					double yImgDis = computeDistance(Pcenter,Py1,Py2);//�������ĵ㵽���ߵ����س���
//					if (f1a*Pcenter.x+f1b*Pcenter.y+f1c>0)//���븳���ţ��������ϱ�
//						yImgDis = -yImgDis;
//					yDis = imgDis2realDis(quadHeight,camF,yImgDis * height/VideoOutputHight);//�����ؾ���ת����ʵ�ʾ���
//					cout<<"yImgDis:"<<yImgDis<<endl;
//					cout<<"yDis:"<<yDis;
//
//					//�ж���������
//					int countBottom = 0,countTop = 0;//����ֵ
//					int areaBottom = 0,areaTop = 0;//���
//					for (int xi = 0;xi<threshold.cols;xi++)
//					{
//						for (int yi = 0;yi<threshold.rows;yi++)
//						{
//							if (f1a*xi+f1b*yi+f1c<0)//��
//							{
//								areaBottom++;
//								if (threshold.at<uchar>(yi,xi)!=0)
//								{
//									countBottom++;
//								} 
//							} 
//							else//��
//							{
//								areaTop++;
//								if (threshold.at<uchar>(yi,xi)!=0)
//								{
//									countTop++;
//								}
//							}
//
//						}
//					}
//					if (1.0*countBottom/areaBottom>0.6)//�ж�����
//					{
//						cout<<"Bottom"<<1.0*countBottom/areaBottom<<endl;
//						ySide = bottomSide;
//						line( src, Py1, Py2, Scalar(0,0,255), 1, CV_AA);//���ߣ�point:(�У���)
//
//					} 
//					else if (1.0*countTop/areaTop>0.6)
//					{
//						cout<<"Top"<<1.0*countTop/areaTop<<endl;
//						ySide = topSide;
//						line( src, Py1, Py2, Scalar(255,255,0), 1, CV_AA);//���ߣ�point:(�У���)
//
//					}
//					else
//					{
//						cout<<"yfalse"<<endl;
//						ySide = noySide;
//					}
//					break;
//				}
//			}
//		}
//		else
//		{
//			ySide = noySide;
//			Py1 = Point2d(-1,-1);
//			Py2 = Point2d(-1,-1);
//			yDis = 10000;
//		}
//
//		//��������������#8.27
//		Point3d detectResult = Point3d(999,999,0);//ÿ֡��ʼ��Ϊ�ޱ߽�
//		//x��ͼ��������Ϊ������y��ͼ��������Ϊ������(û���ҵ���Ϊ999)����־λ��(0-�ޱ߽磬1-x���£�2-y���£�3-xy����)#8.27
//
//		if (xSide != noxSide)//x����#8.27
//		{
//			if (xSide == leftSide)
//			{
//				detectResult.x = xMin - xDis;
//			} 
//			else
//			{
//				detectResult.x = xMax - xDis;
//			}
//			detectResult.z = 1;
//		}
//		if (ySide != noySide)//y����#8.27
//		{
//			if (ySide == bottomSide)
//			{
//				detectResult.y = yMin - yDis;
//			} 
//			else
//			{
//				detectResult.y = yMax - yDis;
//			}
//			detectResult.z = 2;
//		}
//		if(xSide != noxSide && ySide != noySide)//xy����#8.27
//		{
//			detectResult.z = 3;
//		}
//		if(quadHeight<1)//����߶�̫�ͣ�����Ϊ����
//		{
//			detectResult.z = 0;
//		}
//		
//		outputResult = Point3d(detectResult.y,detectResult.x,detectResult.z);//������긳ֵ
//		cout<<"x"<<outputResult.x<<"  y"<<outputResult.y<<"  flag"<<outputResult.z<<endl;//#8.27
//
//
//		//��ʾ
//		namedWindow("threshold",WINDOW_NORMAL);
//		imshow("threshold",threshold);
//		namedWindow("edges",WINDOW_NORMAL);
//		imshow("edges",edges);
//		namedWindow("src",WINDOW_NORMAL);
//		imshow("src",src);
//
//
//		outputVideo<<src;
//
//		if(waitKey(50)>=0)//�����ӳ٣�ͨ������ֹͣ��Ƶ
//			stop = true;
//	}
//	return 0;
//}
//
//double computeDistance(Point2d P0,Point2d P1,Point2d P2)
//{
//	double c_2 = (P0.x - P1.x)*(P0.x - P1.x) + (P0.y - P1.y)*(P0.y - P1.y);//б��ƽ��
//	double a = ((P2.x - P1.x)*(P0.x - P1.x) + (P2.y - P1.y)*(P0.y - P1.y))/sqrt((P2.x - P1.x)*(P2.x - P1.x)+(P2.y - P1.y)*(P2.y - P1.y));//ֱ�Ǳ�a
//
//	return sqrt(c_2-a*a);
//}
//double imgDis2realDis(double h, double f,double imgDis)
//{
//	return h*imgDis/f;
//}
