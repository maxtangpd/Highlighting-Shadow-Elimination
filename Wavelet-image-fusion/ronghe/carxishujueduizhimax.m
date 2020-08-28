clear all;
close all;
x=imread('clockA.jpg');
y=imread('clockB.jpg');
a=rgb2gray(x);
b=rgb2gray(y);
a=double(a);
b=double(b);
[aA,aH,aV,aD]=dwt2(a,'bior2.4');
[bA,bH,bV,bD]=dwt2(b,'bior2.4');
newA=zeros(size(aA));
newH=zeros(size(aH));
newV=zeros(size(aV));
newD=zeros(size(aD));
[m,n]=size(aA);
for i=1:m
    for j=1:n
        newA(i,j)=(aA(i,j)+bA(i,j))/2;
    end;
end;
for i=1:m
    for j=1:n
        if abs(aH(i,j))>=abs(bH(i,j));
            newH(i,j)=aH(i,j);
        else
            newH(i,j)=bH(i,j);
        end
    end
end
for i=1:m
    for j=1:n
        if abs(aV(i,j))>=abs(bV(i,j));
            newV(i,j)=aV(i,j);
        else
            newV(i,j)=bV(i,j);
        end
    end
end
for i=1:m
    for j=1:n
        if abs(aD(i,j))>=abs(bD(i,j));
            newD(i,j)=aD(i,j);
        else
            newD(i,j)=bD(i,j);
        end
    end
end
new=idwt2(newA,newH,newV,newD,'bior2.4');
new=uint8(new);
figure;
imshow(new);
title('系数绝对值之取大')