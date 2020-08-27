function x = pdfbrec(y, pfilt, dfilt)
% PDFBREC   Pyramid Directional Filterbank Reconstruction
%
%	x = pdfbrec(y, pfilt, dfilt)
%
% Input:
%   pfilt:  �������˲���
%   dfilt:  �����˲���
%
% Output:
%   x:     �ع�ͼ��


n = length(y) - 1;
if n <= 0
    x = y{1};
    
else
    %�Ե�Ƶ�����ع�
    xlo = pdfbrec(y(1:end-1), pfilt, dfilt);
    
    %ѡ���Ӧ�ֽ��˲���
    [h, g] = pfilters(pfilt);
    
    % ����ϸ���Ӵ��ݹ����
    if length(y{end}) ~= 3
        switch dfilt        
            case {'pkva6', 'pkva8', 'pkva12', 'pkva'}	
                xhi = dfbrec_l(y{end}, dfilt);
            otherwise	
                xhi = dfbrec(y{end}, dfilt); 
        end
        
        x = lprec(xlo, xhi, h, g);
   
    else    
       %������������ֽ���Ϊ0ʱ
        x = wfb2rec(xlo, y{end}{1}, y{end}{2}, y{end}{3}, h, g);
    end
end