%---------------------ImageFusion-------------------------------%
%----像素的灰度值空域直接比较,即绝对值取大
%---2009-03-05@储彬彬---QQ304150436---E_mail:cbbpaper@126.com---%
%---------------------------------------------------------------%
clear  all
clc
x1=imread('1.bmp');
subplot(221)
imshow(x1)
x1=rgb2gray(x1);
x1=double(x1)/255; 
x2=imread('2.bmp');
subplot(222)
imshow(x2)
x2=rgb2gray(x2);
x2=double(x2)/255; 

[row,col,d]=size(x1);
x=[];
for k=1:d
for i=1:row
    for j=1:col
        if x1(i,j,k)>x2(i,j,k)
            x(i,j,k)=x1(i,j,k);
       else 
           x(i,j,k)=x2(i,j,k);
        end
        % x(i,j,k)=(x1(i,j,k)+x2(i,j,k))/2;
    end
end
end
x3=uint8(x*255);
imwrite(x3,'3.bmp');
subplot(223)
imshow(x3)

    

