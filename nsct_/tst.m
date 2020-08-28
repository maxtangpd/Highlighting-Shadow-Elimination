nlevels = [0] ;        % Decomposition level
pfilter = 'maxflat' ;              % Pyramidal filter
dfilter = 'dmaxflat7' ;              % Directional filter
if ~exist('option', 'var')
    option = 'auto' ;
end;
x1=imread('tst.jpg');
x1 = rgb2gray(x1);
x2=imread('tst1.jpg');
x2 = rgb2gray(x2);
coeffs1 = decdemo( x1, option );
coeffs2 = decdemo( x2, option );


for N=1:2
 for i=1:512
     for j=1:512
         if abs(coeffs1{1,N}(i,j))>abs(coeffs2{1,N}(i,j))
         coeffs1{1,N}(i,j)=coeffs1{1,N}(i,j);
         else  coeffs1{1,N}(i,j)=coeffs2{1,N}(i,j);
         end
 %          coeffs1{1,1}(i,j)=(coeffs1{1,1}(i,j)+coeffs2{1,1}(i,j))/2;
     end
 end
end
% 
%     for i=1:512
%           for j=1:512
%           if abs(coeffs1{1,2}(i,j))>abs(coeffs2{1,2}(i,j))
%           coeffs1{1,2}(i,j)=coeffs1{1,2}(i,j);
%           else  coeffs1{1,2}(i,j)=coeffs2{1,2}(i,j);
%           end
%  %          coeffs1{1,2}(i,j)=(coeffs1{1,2}(i,j)+coeffs2{1,2}(i,j))/2;
%          end
%     end
%  for N=1:4
%     for i=1:512
%          for j=1:512
%           if abs(coeffs1{1,3}{1,N}(i,j))>abs(coeffs2{1,3}{1,N}(i,j))
%           coeffs1{1,3}{1,N}(i,j)=coeffs1{1,3}{1,N}(i,j);
%           else  coeffs1{1,3}{1,N}(i,j)=coeffs2{1,3}{1,N}(i,j);
%           end
%  %           coeffs1{1,3}{1,N}(i,j)=(coeffs1{1,3}{1,N}(i,j)+coeffs2{1,3}{1,N}(i,j))/2;
%          end
%     end
%  end
%   for N=1:8
%     for i=1:512
%          for j=1:512
%           if abs(coeffs1{1,4}{1,N}(i,j))>abs(coeffs2{1,4}{1,N}(i,j))
%           coeffs1{1,4}{1,N}(i,j)=coeffs1{1,4}{1,N}(i,j);
%           else  coeffs1{1,4}{1,N}(i,j)=coeffs2{1,4}{1,N}(i,j);
%           end
%  %          coeffs1{1,4}{1,N}(i,j)=(coeffs1{1,4}{1,N}(i,j)+coeffs2{1,4}{1,N}(i,j))/2;
%          end
%     end
%   end



 imrec = nsctrec( coeffs1, dfilter, pfilter ) ;
 imshow( imrec, [0, 255] );
 title('Rong image' ) ;