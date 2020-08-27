function y = pdfbdec(x, pfilt, dfilt, nlevs)
% PDFBDEC   Pyramidal Directional Filter Bank (or Contourlet) Decomposition
%
%	y = pdfbdec(x, pfilt, dfilt, nlevs)
%
% Input:
%   x:      input image
%   pfilt: 金字塔分解滤波器
%   dfilt: 方向滤波器
%   nlevs:  分解层次
% Output:
%   y:      一个多维cell数组


if length(nlevs) == 0
    y = {x};
    
else
    [h, g] = pfilters(pfilt);
    
    if nlevs(end) ~= 0
        % 拉普拉斯金字塔分解
        [xlo, xhi] = lpdec(x, h, g);
    
        % 方向滤波器
        switch dfilt        % 选择滤波器种类
            case {'pkva6', 'pkva8', 'pkva12', 'pkva'}   
 
                xhi_dir = dfbdec_l(xhi, dfilt, nlevs(end));
            
            otherwise       
         
                xhi_dir = dfbdec(xhi, dfilt, nlevs(end));                
        end
        
    else        
        % 特殊情况所在层数的分解数为0时，分解3次
        [xlo, xLH, xHL, xHH] = wfb2dec(x, h, g);
        xhi_dir = {xLH, xHL, xHH};
    end
    
    % 递归调用
    ylo = pdfbdec(xlo, pfilt, dfilt, nlevs(1:end-1));

    y = {ylo{:}, xhi_dir};
end