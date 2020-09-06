function x = nsscrec(y, dfilt, pfilt)
% nsct重构
%
%	x = nsscrec(y, [dfilt, pfilt] )
%
% INPUT:
%   y:  一个多维的数组
%   dfilt: 方向滤波器
%   pfilt: 金子塔滤波器
%
% OUTPUT:
%   x:一个多维的cell数组


% Check input
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
[h1, h2] = dfilters(dfilt, 'r');
% A scale is required for the nonsubsampled case.
h1 = h1./sqrt(2) ;
h2 = h2./sqrt(2) ;

% Generate the first-level fan filters by modulations.
filters{1} = modulate2(h1, 'c');
filters{2} = modulate2(h2, 'c'); 

% Obtain the parallelogram filters from the diamond filters
[filters{3}, filters{4}] = parafilters( h1, h2 ) ;

% Currently only one filter by Arthur Cunha
% It has been normalized.
[h1, h2, g1, g2] = atrousfilters(pfilt) ; 



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Nonsubsampled Contourlet transform with tree structure filter banks
% Nonsubsampled pyramids make multiresolution decomposition.
% Nonsubsampled directional filter banks make directional decomposition.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
n = length(y) - 1;
xlo = y{1};
nIndex = n-1;
for i=1:n
    
    % Process the detail subbands
    if iscell( y{i+1} )
        % Nonsubsampled DFB reconstruction
        xhi = nsdfbrec( y{i+1}, filters ); 
    else
        % No DFB decomposition, copy directly
        xhi = y{i+1};
    end
                 
    % Nonsubsampled Pyramid reconstruction
    x = nsfbrec(xlo, xhi, g1, g2, nIndex);            
        
    % Prepare for the next level
    xlo = x ;
    nIndex = nIndex -1;     
end