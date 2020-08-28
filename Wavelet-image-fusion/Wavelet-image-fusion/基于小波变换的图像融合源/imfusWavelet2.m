%-----ImageFusion Based on Wavelet------------------------------%
%��ƵС��ϵ������ֵ�ϴ��ںϲ��ԣ���Ƶ���ھֲ�������׼��
%---------------------------------------------------------------%
clear  all
clc
x1=imread('trans.jpg');
x1=rgb2gray(x1);
x1=double(x1)/255; 
x2=imread('possion.jpg');
x2=rgb2gray(x2);
x2=double(x2)/255; 
subplot(221)
imshow('trans.jpg')
title('���ں�ͼ��1')
subplot(222)
imshow('possion.jpg')
title('���ں�ͼ��2')
[ca1,ch1,cv1,cd1]=dwt2(x1,'db1');
[ca2,ch2,cv2,cd2]=dwt2(x2,'db1');
[row,col]=size(ca1);

fun1 = inline('max(abs(x(:)))')
% sc1=nlfilter(ca1,[3 3],fun1);
% sc2=nlfilter(ca2,[3 3],fun1);
sh1=nlfilter(ch1,[3 3],fun1);
sh2=nlfilter(ch2,[3 3],fun1);
sv1=nlfilter(cv2,[3 3],fun1);
sv2=nlfilter(cv2,[3 3],fun1);
sd1=nlfilter(cd1,[3 3],fun1);
sd2=nlfilter(cd2,[3 3],fun1);

for i=1:row
    for j=1:col       
 %-----��Ƶ�ںϲ��� -----%  
       %��Ƶȡƽ��
       % ca(i,j)=(ca1(i,j)+ca2(i,j))/2;
        %��Ƶȡ����ֵ��  
        if ca1(i,j)>ca2(i,j)
           ca(i,j)=ca1(i,j);
       else 
          ca(i,j)=ca2(i,j);
        end
%      if sc1(i,j)>=sc2(i,j)
%         mcx(i,j)=1;
%         mcy(i,j)=0;
%        else
%         mcx(i,j)=0;
%         mcy(i,j)=1;
%        end
  %-----��Ƶ�ںϲ��� -----%   
%     ch(i,j)=(ch1(i,j)+ch2(i,j))/2;
%     cv(i,j)=(ch1(i,j)+ch2(i,j))/2;
%     cd(i,j)=(ch1(i,j)+ch2(i,j))/2;
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
% mcc=nlfilter(mcx,[3 3],fun2); 
mxx=nlfilter(mhx,[3 3],fun2); 
mvv=nlfilter(mvx,[3 3],fun2); 
mdd=nlfilter(mhx,[3 3],fun2);
for i=1:row
    for j=1:col
%         if mcx(i,j)>=1
%            ca(i,j)=ca1(i,j);
%         else
%             ca(i,j)=ca2(i,j);
%         end
        if mxx(i,j)>=2
           ch(i,j)=ch1(i,j);
        else
            ch(i,j)=ch2(i,j);
        end
        if mvv(i,j)>=2
           cv(i,j)=cv1(i,j);
        else
            cv(i,j)=cv2(i,j);
        end  
        if mdd(i,j)>=2
           cd(i,j)=cd1(i,j);
        else
            cd(i,j)=cd2(i,j);
        end     
    end
end
     
x=idwt2(ca,ch,cv,cd,'db1');
%imwrite(x,'wavefuion2.jpg');
subplot(223)
imshow(x)
title('�ںϺ�ͼ��')
