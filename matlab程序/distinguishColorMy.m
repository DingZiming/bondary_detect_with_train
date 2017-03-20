%ɫ�ʿռ�KNN

%��ͼƬ�ϵĵ���ʾ��ɫ�ʿռ���
clear;
clc;
clf;
src = imread('6.png');
src = imresize(src,128.0/size(src,1));% ��һ����С
RGB = boolean(zeros(255,255,255));% RGB�ռ䣨1-255��

%ͼƬ��Χ
figure(1)
imshow(src);
[x0,y0] = ginput(2);
x1 = max(1,int32(x0));
y1 = max(1,int32(y0));

ii = 1;
for i = x1(1,1):x1(2,1)
    for j = y1(1,1):y1(2,1)
        x(ii,1) = i;%ѡȡ���������
        y(ii,1) = j;%ѡȡ���������
        ii = ii + 1;
    end
end

%RGB�ռ丳ֵ
for i = 1:size(x,1)
    R(i) = src(y(i),x(i),1);%��ȡ����RGB
    G(i) = src(y(i),x(i),2);
    B(i) = src(y(i),x(i),3);
    R(i) = max(1,R(i));G(i) = max(1,G(i));B(i) = max(1,B(i));
    RGB(R(i),G(i),B(i)) = 1;    
end

%����RGB�ռ��еĵ�
dilateSize = 5;%���Ͱ뾶
RGB_dilate = boolean(zeros(255,255,255));
for i = 1:255
    for j = 1:255
        for k = 1:255
            if RGB(i,j,k) == 1
                    Rkernel = max(1,i-dilateSize):min(255,i+dilateSize);%���ͺ�
                    Gkernel = max(1,j-dilateSize):min(255,j+dilateSize);
                    Bkernel = max(1,k-dilateSize):min(255,k+dilateSize);
                    RGB_dilate(Rkernel,Gkernel,Bkernel) = ones(size(Rkernel,2),size(Gkernel,2),size(Bkernel,2));
            end
        end
    end
end

%��ʴRGB�ռ��еĵ�
erodeSize = 3;%��ʴ�뾶
RGB_erode = boolean(zeros(255,255,255));
for i = 1+erodeSize:255-erodeSize
    for j = 1+erodeSize:255-erodeSize
        for k = 1+erodeSize:255-erodeSize
            if RGB_dilate(i,j,k) ==1
                Rkernel = i-erodeSize:i+erodeSize;
                Gkernel = j-erodeSize:j+erodeSize;
                Bkernel = k-erodeSize:k+erodeSize;
                if sum(sum(sum(RGB_dilate(Rkernel,Gkernel,Bkernel)))) == (2*erodeSize+1)^3
                    RGB_erode(i,j,k) = 1;
                end
            end
        end
    end
end

%��ʾRGB�ռ�ͼ��
figure(2)
hold on;
grid on;
for i = 1:255
    for j = 1:255
        for k = 1:255
            if RGB_erode(i,j,k) == 1
                    plot3(i,j,k,'ob');
            end
        end
    end
end
xlabel('R');
ylabel('G');
zlabel('B');
title('RGB');
% axis([0,256,0,256,0,256]);
view(30,30);

%����ͼ��ָ�
figure(3)
dst = zeros(size(src,1),size(src,2));
for i = 1:size(src,1)
    for j = 1:size(src,2)
        srcR = max(1,src(i,j,1));srcG = max(1,src(i,j,2));srcB = max(1,src(i,j,3));
        dst(i,j) = RGB_erode(srcR,srcG,srcB);
    end
end
imshow(dst);

