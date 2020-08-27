nlevels = [0, 3, 3, 4] ;        % 分解层次
pfilter = '9-7' ;               % 金字塔分解
dfilter = 'pkva' ;              % 方向滤波器

x1=imread('tst1.jpg');
x1 = rgb2gray(x1);
if ~exist('option', 'var')
    option = 'auto' ;
end;

x2=imread('tst.jpg');
x2 = rgb2gray(x2);
coeffs1 = decdemo( x1, option );
coeffs2 = decdemo( x2, option );
%采用最大阈值的融合方法
%第一个细胞元
 for i=1:32
     for j=1:32
         if abs(coeffs1{1,1}(i,j))>abs(coeffs2{1,1}(i,j))
         coeffs1{1,1}(i,j)=coeffs1{1,1}(i,j);
         else  coeffs1{1,1}(i,j)=coeffs2{1,1}(i,j);
         end
 %          coeffs1{1,1}(i,j)=(coeffs1{1,1}(i,j)+coeffs2{1,1}(i,j))/2;
     end
 end
 %第二个细胞元
 for N=1:3
    for i=1:32
         for j=1:32
          if abs(coeffs1{1,2}{1,N}(i,j))>abs(coeffs2{1,2}{1,N}(i,j))
          coeffs1{1,2}{1,N}(i,j)=coeffs1{1,2}{1,N}(i,j);
          else  coeffs1{1,2}{1,N}(i,j)=coeffs2{1,2}{1,N}(i,j);
          end
 %             coeffs1{1,2}{1,N}(i,j)=(coeffs1{1,2}{1,N}(i,j)+coeffs2{1,2}{1,N}(i,j))/2;
         end
    end
 end
 %第三个细胞元
  for N=1:4
    for i=1:32
         for j=1:64
          if abs(coeffs1{1,3}{1,N}(i,j))>abs(coeffs2{1,3}{1,N}(i,j))
          coeffs1{1,3}{1,N}(i,j)=coeffs1{1,3}{1,N}(i,j);
          else  coeffs1{1,3}{1,N}(i,j)=coeffs2{1,3}{1,N}(i,j);
          end
 %           coeffs1{1,3}{1,N}(i,j)=(coeffs1{1,3}{1,N}(i,j)+coeffs2{1,3}{1,N}(i,j))/2;
         end
    end
  end
  for N=5:8
    for i=1:64
         for j=1:32
          if abs(coeffs1{1,3}{1,N}(i,j))>abs(coeffs2{1,3}{1,N}(i,j))
          coeffs1{1,3}{1,N}(i,j)=coeffs1{1,3}{1,N}(i,j);
          else  coeffs1{1,3}{1,N}(i,j)=coeffs2{1,3}{1,N}(i,j);
          end
 %           coeffs1{1,3}{1,N}(i,j)=(coeffs1{1,3}{1,N}(i,j)+coeffs2{1,3}{1,N}(i,j))/2;
         end
    end
  end
  
  
  %第四个细胞元
  for N=1:4
    for i=1:64
         for j=1:128
          if abs(coeffs1{1,4}{1,N}(i,j))>abs(coeffs2{1,4}{1,N}(i,j))
          coeffs1{1,4}{1,N}(i,j)=coeffs1{1,4}{1,N}(i,j);
          else  coeffs1{1,4}{1,N}(i,j)=coeffs2{1,4}{1,N}(i,j);
          end
%             coeffs1{1,4}{1,N}(i,j)=(coeffs1{1,4}{1,N}(i,j)+coeffs2{1,4}{1,N}(i,j))/2;
         end
    end
  end
  for N=5:8
    for i=1:128
         for j=1:64
          if abs(coeffs1{1,4}{1,N}(i,j))>abs(coeffs2{1,4}{1,N}(i,j))
          coeffs1{1,4}{1,N}(i,j)=coeffs1{1,4}{1,N}(i,j);
          else  coeffs1{1,4}{1,N}(i,j)=coeffs2{1,4}{1,N}(i,j);
          end
 %            coeffs1{1,4}{1,N}(i,j)=(coeffs1{1,4}{1,N}(i,j)+coeffs2{1,4}{1,N}(i,j))/2;
         end
    end
  end
  
 for N=1:8
    for i=1:64
         for j=1:256
          if abs(coeffs1{1,5}{1,N}(i,j))>abs(coeffs2{1,5}{1,N}(i,j))
          coeffs1{1,5}{1,N}(i,j)=coeffs1{1,5}{1,N}(i,j);
          else  coeffs1{1,5}{1,N}(i,j)=coeffs2{1,5}{1,N}(i,j);
          end
 %            coeffs1{1,5}{1,N}(i,j)=(coeffs1{1,5}{1,N}(i,j)+coeffs2{1,5}{1,N}(i,j))/2;
         end
    end
  end
  for N=9:16
    for i=1:256
         for j=1:64
          if abs(coeffs1{1,5}{1,N}(i,j))>abs(coeffs2{1,5}{1,N}(i,j))
          coeffs1{1,5}{1,N}(i,j)=coeffs1{1,5}{1,N}(i,j);
          else  coeffs1{1,5}{1,N}(i,j)=coeffs2{1,5}{1,N}(i,j);
          end

%coeffs1{1,5}{1,N}(i,j)=(coeffs1{1,5}{1,N}(i,j)+coeffs2{1,5}{1,N}(i,j))/2;
         end
    end
  end
 imrec = pdfbrec( coeffs1, pfilter, dfilter ) ;
 imshow( imrec, [0, 255] );
 imwrite(imrec,'conterlet.jpg');
 title('Rong image' ) ;
  
