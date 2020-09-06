function coeffs = decdemo( im, option )
% ���е���ͼ���nsct�ֽ����ع� 
%
% Input:
%	im: ����һ��ͼ��
%   option: option for the demos. The default value is 'auto'
%       'auto' ------  automtatical demo, no input
%       'user' ------  semi-automatic demo, simple interactive inputs
%       'expert' ----  mannual, complete interactive inputs. 
%                      (Not implmented in this version)
%
% Output:
%	coeffs:һ����ά��cell���������ѡ��ĵķֽ��������
%  
disp(' ');

% Input image
if ~exist('im', 'var')
    im = imread ('tst.jpg') ;
    im = rgb2gray(im);
end

% չʾͼ��
disp( 'Displaying the input image...');
clf;
imagesc(im, [0, 255]);

title( 'Input image' ) ;
axis image off;
colormap(gray);


% Parameteters:
nlevels = [0,0] ;               % ��Ƶ�ֽ�����������Լ����壬0001��ʾ��1��2��3��ֽ�1�Σ���4��ֽ�2^1��
pfilter = 'maxflat' ;                 % �������˲���
dfilter = 'dmaxflat7' ;              % �����˲���

%��ͼ����н������ֽ⣬Ȼ����з����˲����������Ϊһ����ά����
coeffs = nsctdec( double(im), nlevels, dfilter, pfilter );

% չʾ��άcell����
disp('Displaying the contourlet coefficients...') ;
%shownsct( coeffs ) ;

% ͼ���ع�
imrec = nsctrec( coeffs, dfilter, pfilter ) ;%��ͼ����н������ֽ⣬Ȼ����з����˲�

disp('Displaying the reconstructed image...') ;
disp('It should be a perfect reconstruction' ) ;
disp(' ') ;

% չʾ�ֽ���ع�ͼ��
figure;
subplot(1,2,1), imagesc( im, [0, 255] ); 
title('Original image' ) ;
colormap(gray);
axis image off;
subplot(1,2,2), imagesc( imrec, [0, 255] );
imwrite(imrec,'try.jpg');
title('Reconstructed image' ) ;
colormap(gray);
axis image off;

figure;
mse = sum( sum( (imrec - double(im)).^2 ) );
mse = mse / prod(size(im));

disp( sprintf('The mean square error is: %f', mse ) );
disp(' ');