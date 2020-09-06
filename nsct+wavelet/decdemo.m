function coeffs = decdemo( im, option )
% 进行单幅图像的nsct分解与重构 
%
% Input:
%	im: 输入一副图像
%   option: option for the demos. The default value is 'auto'
%       'auto' ------  automtatical demo, no input
%       'user' ------  semi-automatic demo, simple interactive inputs
%       'expert' ----  mannual, complete interactive inputs. 
%                      (Not implmented in this version)
%
% Output:
%	coeffs:一个多维的cell数组根据所选择的的分解层数来定
%  
disp(' ');

% Input image
if ~exist('im', 'var')
    im = imread ('tst.jpg') ;
    im = rgb2gray(im);
end

% 展示图像
disp( 'Displaying the input image...');
clf;
imagesc(im, [0, 255]);

title( 'Input image' ) ;
axis image off;
colormap(gray);


% Parameteters:
nlevels = [0,0] ;               % 高频分解层数，可以自己定义，0001表示第1、2、3层分解1次，第4层分解2^1次
pfilter = 'maxflat' ;                 % 金字塔滤波器
dfilter = 'dmaxflat7' ;              % 方向滤波器

%对图像进行金字塔分解，然后进行方向滤波，结果保存为一个多维数组
coeffs = nsctdec( double(im), nlevels, dfilter, pfilter );

% 展示多维cell数组
disp('Displaying the contourlet coefficients...') ;
%shownsct( coeffs ) ;

% 图像重构
imrec = nsctrec( coeffs, dfilter, pfilter ) ;%对图像进行金字塔分解，然后进行方向滤波

disp('Displaying the reconstructed image...') ;
disp('It should be a perfect reconstruction' ) ;
disp(' ') ;

% 展示分解和重构图像
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