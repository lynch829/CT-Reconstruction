function [Ir, cv] = ART(P, SM, f0, N, lambda)
% ART: Algebraic Reconstruction Technology
% --------------------------------------------------
% P : Projection data
% SM : System matrix
% f0 : initial image
% N : iteration times
% lambda : relaxation factor
% Ir : reconstructed image
% cv : convergence

% Huayu Zhang, 2014

cv = zeros(N,1);
f = f0;

for i = 1:N
    lastf = f;
    f = ARTkernel(P, lastf, SM, lambda);
    f(f<0) = 0;
%     reshape(f,[sqrt(numel(f)),sqrt(numel(f))]) % for debug
    cv(i) = sum((lastf - f).^2);
    fprintf(1,'ART number:%d, MSE=%f\n',i,cv(i));
end

Ir = f;
