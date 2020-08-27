function x = pdfbrec(y, pfilt, dfilt)
% PDFBREC   Pyramid Directional Filterbank Reconstruction
%
%	x = pdfbrec(y, pfilt, dfilt)
%
% Input:
%   pfilt:  金字塔滤波器
%   dfilt:  方向滤波器
%
% Output:
%   x:     重构图像


n = length(y) - 1;
if n <= 0
    x = y{1};
    
else
    %对低频进行重构
    xlo = pdfbrec(y(1:end-1), pfilt, dfilt);
    
    %选择对应分解滤波器
    [h, g] = pfilters(pfilt);
    
    % 处理细节子带递归吃力
    if length(y{end}) ~= 3
        switch dfilt        
            case {'pkva6', 'pkva8', 'pkva12', 'pkva'}	
                xhi = dfbrec_l(y{end}, dfilt);
            otherwise	
                xhi = dfbrec(y{end}, dfilt); 
        end
        
        x = lprec(xlo, xhi, h, g);
   
    else    
       %特殊情况，当分解数为0时
        x = wfb2rec(xlo, y{end}{1}, y{end}{2}, y{end}{3}, h, g);
    end
end