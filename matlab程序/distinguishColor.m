%��ɫ�ָ��㷨���ԣ���ֵ����
a=imread('5.png');
%����ͨ������ĳ���һ��ͼ��RGB����ͨ�� 
R=a(:,:,1);
G=a(:,:,2);
B=a(:,:,3);

%gray
agray = rgb2gray(a);
%agray = histeq(agray);

%hsv
a2 = rgb2hsv(a);
H=a(:,:,1);
S=a(:,:,2);
V=a(:,:,3);

%find
findr = double(R)./(double(G)+double(B))>0.7;
findg = (R<150)&(B<150)&(G>R+5)&(G>B+5);
%H = histeq(H);
findw = agray>200;
findb = (B>R-5)&(B>G-5);%&(H>60)&(H<200);%(B>G-10)&(B>R-10);

figure(1);
subplot(2,3,1);imshow(a);title('ԭʼͼ��'); 
subplot(2,3,2);imshow(agray);title('�Ҷ�ͼ'); 
subplot(2,3,3);imshow(findw);title('W����');
subplot(2,3,4);imshow(findr);title('R����');
subplot(2,3,5);imshow(findg);title('G����');
subplot(2,3,6);imshow(findb);title('B����');
