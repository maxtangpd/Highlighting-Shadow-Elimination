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
[m,n]=size(aH);
for i=1:m
    for j=1:n
        newH(i,j)=(aH(i,j)+bH(i,j))/2;
    end;
end;
[m,n]=size(aV);
for i=1:m
    for j=1:n
        newV(i,j)=(aV(i,j)+bV(i,j))/2;
    end;
end;
[m,n]=size(aD);
for i=1:m
    for j=1:n
        newD(i,j)=(aD(i,j)+bD(i,j))/2;
    end;
end;

new=idwt2(newA,newH,newV,newD,'bior2.4');
new=uint8(new);
figure;
imshow(new);
title('¾ùÖµ')