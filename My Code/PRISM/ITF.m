function x = ITF(y, filter)
% x = ITF(y, filter) : Inversion Iight Frame Transform
% ----------------------------------------------------
% y : signal in TF domain y = {Wx, xL, L}
% filter : image filter

% Huayu Zhang, Dec 2014

xl = y.xL;
nf = numel(filter);

for l = y.L:1
    xl = circshift(imfilter(xl, filter{1}),size(filter{1})-1 * mod(l,2));
    for i = 2:nf
        xl = xl + circshift(imfilter(y.Wx{i-1,l}),size(filter{i})-1 * mod(l,2));
    end
end
x = xl;