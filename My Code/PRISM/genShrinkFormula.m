function x = genShrinkFormula(Y, lmb)
% GENSHRINKFORMULA : generalized shrinkage formula
% ---------------------------------------------------
% x = genShrinkFormula(Y, lmb)
% x = argmin_x (\|x - Y\|_2^2 + \lambda \| x \|_2^2
% Y: N * d, N : number of elements, d : dimensions

% Huayu Zhang, Dec 2014

Y_star = sqrt(sum(Y.^2,2)); % norm of rows of matrix

x = Y ./ repmat(Y_star,[1,size(Y,2)]) .* repmat(max(Y_star - lmb, 0),[1,size(Y,2)]);

