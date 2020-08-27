function coeffs = decdemo( im, option )

% DECDEMO  对图像进行轮廓小波变换的分解和重构
%
% Input:
%	image:  输入一个图像
%   option: option for the demos. The default value is 'auto'
%       'auto' ------  automtatical demo, no input
%       'user' ------  semi-automatic demo, simple interactive inputs
%       'expert' ----  mannual, complete interactive inputs. 
%                      (Not implmented in this version)
%
% Output:
%	coeffs: 一个多维cell数组


disp('Welcome to the contourlet decomposition demo! :)');
disp('Type help decdemo for help' ) ;
disp('You can also view decdemo.m for details.') ;
disp(' ');

% Input image
if ~exist('im', 'var')
    im = imread('tst1.jpg');
    im = rgb2gray(im);
end

% Show the input image
disp( 'Displaying the input image...');
clf;
imagesc(im, [0, 255]);
title( 'Input image' ) ;
axis image off;
colormap(gray);
disp( ' ' );

% Parameteters:
nlevels = [0,3,3,4] ;        % Decomposition level
pfilter = '9-7' ;              % Pyramidal filter
dfilter = 'pkva' ;              % Directional filter

%轮廓小波分解
coeffs = pdfbdec( double(im), pfilter, dfilter, nlevels );

% 展示图像
disp('Displaying the contourlet coefficients...') ;
imcoeff = showpdfb( coeffs ) ;
title('Contourlet coefficients');
disp(' ');

imshow( imcoeff, [0,255] );
title('mid');

disp(' ');

% 重构
imrec = pdfbrec( coeffs, pfilter, dfilter ) ;

disp('Displaying the reconstructed image...') ;
disp('It should be a perfect reconstruction' ) ;
disp(' ') ;

imshow( imrec, [0, 255] );
title('Reconstructed image' ) ;
axis image off;

mse = sum( sum( (imrec - double(im)).^2 ) );
mse = mse / prod(size(im));

disp( sprintf('The mean square error is: %f', mse ) );
disp(' ');