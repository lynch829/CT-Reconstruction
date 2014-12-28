function x = GSVT(Y, lmb)
% GSVT: Generalized singular value thresholding
% ---------------------------------------------------
% x = GSVT(Y, lmb)
% x = argmin_x \| Y - x \|_2^2 + lmb * \| x \|_*
% Y: r*c * D, r*c : iamge size, D: dimensions

% Huayu Zhang, Dec 2014
Y_star = sqrt(sum(Y.^2,3));

if(numel(Y_star(isnan(Y_star))) > 0)
    disp('Nan in image');
    Y_star(isnan(Y_star)) = 0;
end
[U,S,V] = svd(Y_star);
S = S - lmb; S(S<0) = 0;

x = Y .* repmat((U * S * V') ./ Y_star,[1,1,size(Y,3)]); 