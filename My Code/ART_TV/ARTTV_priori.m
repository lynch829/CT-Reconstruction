function [Ir, cv] = ARTTV_priori(P, SM, I, Nart, lambda, Ntv)
% ARTTV_PRIORI: ART-TV reconstruction with priori information
% -------------------------------------------------------------
% P: Projection Data
% SM: system matrix
% I: priori image
% Nart: number of art iterations
% lambda: relaxation factor
% Ntv: number of total variation
% Ir: recon image
% cv: convergence

% Huayu Zhang, 2014
% Please cite ShenLe's master thesis:Spectral 
% Computed Tomography Imaging System Design and Reconstruction, 2013

if(~issparse(SM))
    SM = sparse(SM);
end

N = size(I,1);
grad = 1./(sqrt([zeros(1,N);diff(I,1,1).^2] + [zeros(N,1), diff(I,1,2).^2]) + 0.01);
TV = ones(N+1)*1./(0.01);
TV(1:N,1:N) = grad;
TV(TV>50) = 200;
cv = zeros(Nart,1);

disp('ART');
f = I;  % initialization
I(I<0) = 0;
I(I>0.05) = 1;
for i = 1:Nart
    lastf = f;
    f = medfilt2(f,[3 3]); % eliminate pepper noise
    fn = ARTkernel(P, f(:), SM, lambda);
    f = reshape(fn,[N,N]);
    f(f<0) = 0;
    f = f.*I;
    
    %TV
    a = 1*exp(-i/Nart);
    d = norm(f(:) - lastf(:));
    f = TVkernel(f, a, d, TV, Ntv);
    cv(i) = sum(sum((f-lastf).^2))/sum(sum(lastf.^2));
    fprintf(1,'ART-TV number:%d, MSE=%f\n',i,cv(i));
end

Ir = f;
        
