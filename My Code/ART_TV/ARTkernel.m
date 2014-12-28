function fn = ARTkernel(P, f, sSM, lambda)
% ARTKERNEL : one iteration of ART
% ------------------------------------------
% P : projection data
% f : current image
% fn : next image
% SM : system matrix(sparse)
% lambda: relaxation factor

% Huayu Zhang, 2014

if(nargin == 3)
    lambda = 1;
end
    
m = size(P,1);      % number of lines
randRIndex = randperm(m);

for i = 1:m
%     ri = randRIndex(i);
    ri = i;
    wi = sSM(ri,:);
    if(nnz(wi) ~= 0)
        k = (P(ri) - wi * f) / (wi * wi');
        f = f + lambda * k * wi';
    end
end

fn = f;

        