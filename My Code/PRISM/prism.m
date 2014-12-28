function [XL, XS] = prism(SM, P, filter, L, lambda1, epoch)
% PRISM : A PRIOR RANK, INTENSITY AND SPARSITY MODEL
% -----------------------------------------------------
% [XL, XS] = prism(SM, P, filter)
% SM : system matrix
% P : projection
% filter : image filter
% L : decomposition level
% lambda1 : regular parameter
% epoch : iteration times
% XL : intensity part
% XS : sparsity part

% Huayu Zhang, Dec 2014
% cite: Hao Gao, Multi-energy CT based on a prior rank, intensity and
% sparsity model(PRISM), IOP, 2011

%% Solve
% $(X_L,X_S) = argmin_{X_L,X_S} \frac{1}{2} \| A(X_L + X_S) - Y \|_2^2 + 
% \lambda_{*} \| WX_L \|_{*} + \lambda_1 \| WX_S \|_1 $ 

% calculate regular parameter
[sp,si] = size(SM);
m = sqrt(si); n = sqrt(si);
r = sqrt(max(m,n));
mu_star = lambda1;
mu1 = mu_star;
lambda_star = r * lambda1;

%initialization
f_k = zeros(sp,1); % projection
XL_k = zeros(m,n); XS_k = zeros(m,n); % image
dL_k = TF(XL_k, filter, L); dS_k = TF(XS_k, filter, L);   % dL = W(XL), dS = W(XS)
vL_k = dL_k; vS_k = dS_k;
AT = SM'; ATA = SM' * SM;


for i = 1:epoch
    % step 1 : L2 normalization problem
    b1 = AT * (P - f_k);
    XL_k = pcg(ATA + mu_star * eye(size(ATA)), b1 + mu_star * ...
        reshape(ITF(lcTF(1, dL_k, -1, vL_k), filter),[numel(b1),1]) - ATA * XS_k(:), [], 50);
    XS_k = pcg(ATA + mu1 * eye(size(ATA)), b1 + mu1 * ...
        reshape(ITF(lcTF(1, dS_k, -1, vS_k), filter),[numel(b1),1]) - ATA * XL_k(:), [] ,50);
    XL_k = reshape(XL_k,[m,n]); XS_k = reshape(XS_k,[m,n]);
    % step 2 :
    lambda = lambda1 / mu1; lambda_array = lambda ./ (2.^(1:L));
    Y = lcTF(1, TF(XS_k, filter, L), 1, vS_k);
    for l = 1 : L
        dS_k.Wx{3,l} = genShrinkFormula(Y.Wx{3,l}(:), lambda_array(l));
        Y1 = cat(2,Y.Wx{1,l}(:), Y.Wx{2,l}(:));
        dS1 = genShrinkFormula(Y1, lambda_array(l));
        dS_k.Wx{1,l} = reshape(dS1(:,1),[m,n]);
        dS_k.Wx{2,l} = reshape(dS1(:,2),[m,n]);
        dS_k.Wx{3,l} = reshape(dS_k.Wx{3,l},[m,n]);
    end
    dS_k.xL = genShrinkFormula(Y.xL, lambda_array(L));
    dS_k.xL = reshape(dS_k.xL,[m,n]);
    vS_k = lcTF(1, Y, -1, dS_k);
    
    % step 3:
    lambda = lambda_star / mu_star; lambda_array = lambda ./ (2.^(1:L));
    Y = lcTF(1, TF(XL_k, filter, L), 1, vL_k);
    for l = 1 : L
        dL_k.Wx{3,l} = GSVT(Y.Wx{3,l}, lambda_array(l));
        Y1 = cat(3,Y.Wx{1,l}, Y.Wx{2,l});
        dL1 = GSVT(Y1, lambda_array(l));
        dL_k.Wx{1,l} = reshape(dL1(:,:,1),[m,n]);
        dL_k.Wx{2,l} = reshape(dL1(:,:,2),[m,n]);
    end
    dL_k.xL = GSVT(Y.xL, lambda_array(L));
    vL_k = lcTF(1, Y, -1, dL_k);
    
    % to remove peper salt noise, I do not know why.
    XL_k = medfilt2(XL_k,[3,3]);
    XS_k = medfilt2(XS_k,[3,3]);
%     for debug
%     figure(i);
%     imagesc([XL_k,XS_k,XL_k+XS_k]); colormap(gray); axis off; axis equal;
%     if(numel(XL_k(isnan(XL_k))) > 0)
%         disp('Nan in image');
%     end
%     if(numel(XS_k(isnan(XS_k))) > 0)
%         disp('Nan in image');
%     end
end


XL = XL_k;
XS = XS_k;