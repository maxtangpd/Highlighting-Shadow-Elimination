%-----ImageFusion Based on Wavelet------------------------------%
%低频小波系数绝对值较大融合策略，高频基于局部显著性准则
%---2009-03-20@储彬彬---QQ304150436---E_mail:cbbpaper@126.com---%
%---------------------------------------------------------------%

clear  all
clc
x1=imread('1.jpg');
x1=rgb2gray(x1);
x1=double(x1)/255; 
x2=imread('2.jpg');
x2=rgb2gray(x2);
x2=double(x2)/255; 
subplot(221)
imshow('1.jpg')
title('待融合图像1')
subplot(222)
imshow('2.jpg')
title('待融合图像2')
[ca1,ch1,cv1,cd1]=dwt2(x1,'db1');
[ca2,ch2,cv2,cd2]=dwt2(x2,'db1');
[row,col]=size(ca1);

fun1 = inline('max(abs(x(:)))')
sh1=nlfilter(ch1,[3 3],fun1);
sh2=nlfilter(ch2,[3 3],fun1);
sv1=nlfilter(cv2,[3 3],fun1);
sv2=nlfilter(cv2,[3 3],fun1);
sd1=nlfilter(cd1,[3 3],fun1);
sd2=nlfilter(cd2,[3 3],fun1);

for i=1:row
    for j=1:col       
 %-----低频融合策略 -----%  
       %低频取平均
       % ca(i,j)=(ca1(i,j)+ca2(i,j))/2;
        %低频取绝对值大    
       if abs(ca1(i,j))>abs(ca2(i,j))
            ca(i,j)=ca1(i,j);
       else 
           ca(i,j)=ca2(i,j);
       end
  %-----高频融合策略 -----%        
       if sh1(i,j)>=sh2(i,j)
        mhx(i,j)=1;
        mhy(i,j)=0;
       else
        mhx(i,j)=0;
        mhy(i,j)=1;
       end
        if sv1(i,j)>=sv2(i,j)
        mvx(i,j)=1;
        mvy(i,j)=0;
       else
        mvx(i,j)=0;
        mvy(i,j)=1;
        end
        if sd1(i,j)>=sd2(i,j)
        md(i,j)=1;
        md(i,j)=0;
       else
        md(i,j)=0;
        md(i,j)=1;
        end
    end
end

fun2 = inline('sum(x(:))')
mxx=nlfilter(mhx,[3 3],fun2); 
mvv=nlfilter(mvx,[3 3],fun2); 
mdd=nlfilter(mhx,[3 3],fun2);
for i=1:row
    for j=1:col
        
        if mxx(i,j)>=5
           ch(i,j)=ch1(i,j);
        else
            ch(i,j)=ch2(i,j);
        end
        if mvv(i,j)>=5
           cv(i,j)=cv1(i,j);
        else
            cv(i,j)=cv2(i,j);
        end  
        if mdd(i,j)>=5
           cd(i,j)=cd1(i,j);
        else
            cd(i,j)=cd2(i,j);
        end     
    end
end
     
x=idwt2(ca,ch,cv,cd,'db1');
%imwrite(x,'wavefuion2.bmp');
subplot(223)
imshow(x)
title('融合后图像')
