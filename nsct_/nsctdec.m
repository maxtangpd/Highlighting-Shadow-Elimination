function y = nsctdec(x, levels, dfilt, pfilt )
% nsct分解
%
%	y = nsctdec(x, levels, [dfilt, pfilt] )
%
% Input:
%   x:  输入图像
%   levels:  nsct变换的层次
%   dfilt:  方向滤波器
%   pfilt:  塔形滤波器
% Output:
%   y:  多维数组
%



% Check input
if ~isnumeric( levels )
    error('The decomposition levels shall be integers');
end
if isnumeric( levels )
    if round( levels ) ~= levels
        error('The decomposition levels shall be integers');
    end
end
        
if ~exist('dfilt', 'var')
    dfilt = 'dmaxflat7' ;
end;

if ~exist('pfilt', 'var')
    pfilt = 'maxflat' ; 
end;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Get fan filters, parallelogram filters, and pyramid filters
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Get the directional filters for the critically sampled DFB.
filters = cell(4) ;
[h1, h2] = dfilters(dfilt, 'd');
% A scale is required for the nonsubsampled case.
h1 = h1./sqrt(2) ;
h2 = h2./sqrt(2) ;

% Generate the first-level fan filters by modulations.
filters{1} = modulate2(h1, 'c');
filters{2} = modulate2(h2, 'c'); 

% Obtain the parallelogram filters from the diamond filters
[filters{3}, filters{4}] = parafilters( h1, h2 ) ;

[h1, h2, g1, g2] = atrousfilters(pfilt); 


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Nonsubsampled Contourlet transform with tree structure filter banks
% Nonsubsampled pyramids make multiresolution decomposition.
% Nonsubsampled directional filter banks make directional decomposition.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Numer of levels:
clevels = length( levels ) ;
nIndex = clevels + 1 ;
% Initialize the output
y = cell(1, nIndex) ;

% Nonsubsampled pyramid decomposition
for i= 1 : clevels    
    
    % Nonsubsampled Contourlet transform
    % Nonsubsampled pyramids decomposition
    [xlo, xhi] = nsfbdec(x, h1, h2, i-1) ;
        
    if levels(nIndex-1) > 0        
        % Nonsubsampled DFB decomposition on the bandpass image
        xhi_dir = nsdfbdec(xhi, filters, levels(nIndex-1));        
        y{nIndex}=xhi_dir ;
    else
        % Copy the result directly:
        y{nIndex}=xhi ;
    end
    
    % Update the index for the Nonsubsampled Pyramdis
    nIndex = nIndex - 1 ;
    
    % Prepare for next iteration
    x = xlo ;
    
end

% The lowpass output
y{1}=x;
