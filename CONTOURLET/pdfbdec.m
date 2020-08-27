function y = pdfbdec(x, pfilt, dfilt, nlevs)
% PDFBDEC   Pyramidal Directional Filter Bank (or Contourlet) Decomposition
%
%	y = pdfbdec(x, pfilt, dfilt, nlevs)
%
% Input:
%   x:      input image
%   pfilt: �������ֽ��˲���
%   dfilt: �����˲���
%   nlevs:  �ֽ���
% Output:
%   y:      һ����άcell����


if length(nlevs) == 0
    y = {x};
    
else
    [h, g] = pfilters(pfilt);
    
    if nlevs(end) ~= 0
        % ������˹�������ֽ�
        [xlo, xhi] = lpdec(x, h, g);
    
        % �����˲���
        switch dfilt        % ѡ���˲�������
            case {'pkva6', 'pkva8', 'pkva12', 'pkva'}   
 
                xhi_dir = dfbdec_l(xhi, dfilt, nlevs(end));
            
            otherwise       
         
                xhi_dir = dfbdec(xhi, dfilt, nlevs(end));                
        end
        
    else        
        % ����������ڲ����ķֽ���Ϊ0ʱ���ֽ�3��
        [xlo, xLH, xHL, xHH] = wfb2dec(x, h, g);
        xhi_dir = {xLH, xHL, xHH};
    end
    
    % �ݹ����
    ylo = pdfbdec(xlo, pfilt, dfilt, nlevs(1:end-1));

    y = {ylo{:}, xhi_dir};
end