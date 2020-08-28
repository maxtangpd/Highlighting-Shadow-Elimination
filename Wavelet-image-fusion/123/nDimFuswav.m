%---------------------ImageFusion Based on Wavelet------------------%
%-ʹ�ö�㣨3�㣩С���任,����ںϹ���,Ч���ܺõ����������ں�ʱ��̫��
%----------��Ƶ\��Ƶȡ����ֵ�ϴ��ںϲ���-----------------------------
%---2009-03-20@�����---QQ304150436---E_mail:cbbpaper@126.com-------%
%-------------------------------------------------------------------%
clear  all
clc
x1=imread('possionrcel.jpg');
x1=rgb2gray(x1);
x1=double(x1)/255; 

x2=imread('possionlcer.jpg');
x2=rgb2gray(x2);
x2=double(x2)/255; 

subplot(221)
imshow('1.jpg')
subplot(222)
imshow('2.jpg')

[c1,s1] = wavedec2(x1,3,'db3');
[c2,s2] = wavedec2(x2,3,'db3');

[row,col]=size(c1);

for i=1:row
    for j=1:col   
        if abs(c1(i,j))>abs(c2(i,j))
            c(i,j)=c1(i,j);
        else 
           c(i,j)=c2(i,j);
        end
    end
end

%  for i=1:row
%      for j=1:col   
% 
%           c(i,j)=(c2(i,j)+c1(i,j))/2;
% 
%      end
% end

x = waverec2(c,s1,'db3');
%x=uint(x)*255; //���ϻᱨ������
imwrite(x,'nDimfus6.jpg');
subplot(223)
imshow(x)

