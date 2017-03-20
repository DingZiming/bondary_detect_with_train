%�鿴������ɫ�ռ������Matlab����ʵ��
a=imread('2.png');  
%����ͨ������ĳ���һ��ͼ��RGB����ͨ�� 
R=a(:,:,1);
G=a(:,:,2);
B=a(:,:,3);

figure(1);
subplot(2,2,1);imshow(a);title('ԭʼͼ��'); 
subplot(2,2,2);imshow(R);title('R����ͼ��');
subplot(2,2,3);imshow(G);title('G����ͼ��');
subplot(2,2,4);imshow(B);title('B����ͼ��');

figure(2);
dr = double(R);
dg = double(G);
db = double(B);

r = dr./(dr+dg+db);
g = dg./(dr+dg+db);
b = db./(dr+dg+db);
subplot(2,2,1);imshow(a);title('ԭʼͼ��'); 
subplot(2,2,2);imshow(r);title('��һ��R����ͼ��');
subplot(2,2,3);imshow(g);title('��һ��G����ͼ��');
subplot(2,2,4);imshow(b);title('��һ��B����ͼ��');

figure(3);
a2 = rgb2hsv(a);
H=a(:,:,1);
S=a(:,:,2);
V=a(:,:,3);
subplot(2,2,1);imshow(a);title('ԭʼͼ��'); 
subplot(2,2,2);imshow(H);title('H����ͼ��');
subplot(2,2,3);imshow(S);title('S����ͼ��');
subplot(2,2,4);imshow(V);title('V����ͼ��');

figure(4);
a3 = rgb2ycbcr(a);
Y = a(:,:,1);
Cr = a(:,:,2);
Cb = a(:,:,3);
subplot(2,2,1);imshow(a);title('ԭʼͼ��'); 
subplot(2,2,2);imshow(Y);title('Y����ͼ��');
subplot(2,2,3);imshow(Cr);title('Cr����ͼ��');
subplot(2,2,4);imshow(Cb);title('Cb����ͼ��');

figure(5);
cform = makecform('srgb2lab'); 
lab = applycform(a, cform);
L=lab(:,:,1);
A=lab(:,:,2);
B=lab(:,:,3);
subplot(2,2,1);imshow(a);title('ԭʼͼ��'); 
subplot(2,2,2);imshow(L);title('L����ͼ��');
subplot(2,2,3);imshow(A);title('A����ͼ��');
subplot(2,2,4);imshow(B);title('B����ͼ��');

