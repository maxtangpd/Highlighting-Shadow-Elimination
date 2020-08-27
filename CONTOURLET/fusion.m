%图象融合（基于小波变化与局部能量）
clc;
clear all;
Y=imread('tst.jpg'); %装入原图像
Y= rgb2gray(Y);
figure(1)
subplot(221)
imshow(Y);
title('clockA-t.jpg')
X1=double(Y)/255;
subplot(222)
imhist(X1);
Y1=imread('tst1.jpg'); %装入原图像
Y1 = rgb2gray(Y1);
subplot(223);
imshow(Y1);
title('clockB-t.jpg')
X2=double(Y1)/255;
subplot(224);
imhist(X2);
%对上述二图像进行三层小波分解
%X1=double(X1)/255;
%X2=double(X2)/255;
[c1,s1]=wavedec2(X1,3,'sym4');
[c2,s2]=wavedec2(X2,3,'sym4');

cA31=appcoef2(c1,s1,'sym4',3);
 cH31=detcoef2('h',c1,s1,3);
 cV31=detcoef2('v',c1,s1,3);
 cD31=detcoef2('d',c1,s1,3);
 cH21=detcoef2('h',c1,s1,2);    
 cV21=detcoef2('v',c1,s1,2);
 cD21=detcoef2('d',c1,s1,2);
cH11=detcoef2('h',c1,s1,1);
cV11=detcoef2('v',c1,s1,1);
cD11=detcoef2('d',c1,s1,1);


 cA32=appcoef2(c2,s2,'sym4',3);
 cH32=detcoef2('h',c2,s2,3);
 cV32=detcoef2('v',c2,s2,3);
 cD32=detcoef2('d',c2,s2,3);
 cH22=detcoef2('h',c2,s2,2);
 cV22=detcoef2('v',c2,s2,2);
 cD22=detcoef2('d',c2,s2,2);
cH12=detcoef2('h',c2,s2,1);
cV12=detcoef2('v',c2,s2,1);
cD12=detcoef2('d',c2,s2,1);


 cA33=cA31.*cA32;
 
fun = inline('sum(x(:))');
 Ea=nlfilter(cA31,[3 3],fun);
 Eb=nlfilter(cA32,[3 3],fun);
 E=nlfilter(cA33,[3 3],fun);
 T=1.5;
 for i=1:numel(Ea)
 M(i)=2*E(i)/(Ea(i)+Eb(i));
 Wmin(i)=0.5-0.5*((1-M(i))/(1-T));
 end
Wmax=1-Wmin;
for k=1:numel(M)
 if M(k)<T
     for i=1:numel(Ea)
         if Ea(i)==1 || Eb(i)==1
             if abs(cA31(i))>abs(cA32(i))|| Ea(i)>Eb(i) 
                 c(i)=cA31(i);
             else 
             c(i)=cA32(i);
             end
         end 
      end
    else
     for j=1:numel(Ea)
         if Ea(j)>Eb(j)
             c(j)=Wmax(j)*cA31(j)+Wmin(j)*cA32(j);
         else
             c(j)=Wmin(j)*cA31(j)+Wmax(j)*cA32(j);
         end
     end
end
 end

 for i=(numel(cA31)+1):numel(c1)
    if  abs(c1(i))>abs(c2(i))
       c(i)=c1(i);
    else
        c(i)=c2(i);
 end 
 end

%应用融合系数进行图像重构并显示
figure(2)
xx=waverec2(c,s1,'sym4');
subplot(121);
imshow(xx,[]);
imwrite(xx,'rh.bmp');
subplot(122);
%colormap(map1);
%axis square
title('本文提出的算法')
imhist(xx);
F1=imread('rh.bmp');

jiaohushang3=CERF(Y,F1)%交叉熵
piancha3=D(Y,F1)%求两图像的偏差
xianguanxishu3=REL(Y,F1)% 求两图的相关系数  
 pingjunwucha3=MAE(Y,F1)%求两图像的平均误差
 [m,n]=size(Y1);
 figure(3)
 z1=Y1(100:m,1:100);
  z2=xx(100:m,1:100);
  subplot(221)
 imshow(z1,[]);
 subplot(222)
 imhist(z1);
  subplot(223)
 imshow(z2,[]);
 subplot(224)
 imhist(z2);
 Entropy31=fusionshang(z1)
 Entropy32=fusionshang(z2)