function y = TF(x, filter, L)
% y = tightFrame(x, filter, L): Tight Frame transform
% ----------------------------------------------------
% x : image or signal
% filter : filter x' = imfilter(x,filter)
% e.g.
% filter = {w(0,0), w(0,1); w(1,0), w(1,1)}
% w(0,0) = 1/4 * [1 1; 1 1]; w(0,1) = 1/4 * [1 -1; 1 -1];
% w(1,0) = 1/4 * [1 1; -1 -1]; w(1,1) = 1/4 * [1 -1; -1 1];
% L : the level of decomposition
% y : signal in TF domain y = {Wx, xL, L}

% Huayu Zhang, Dec 2014

nf = numel(filter);  % number of element in filter
y.Wx = cell(nf - 1, L);
y.L = L;

% level-1
xl = imfilter(x, filter{1});
for i = 2:nf
    y.Wx{i-1,1} = circshift(imfilter(x, filter{i}), size(filter{i})-1); 
end

% level 2~L
for l = 2:L
    for i = 2:nf
        y.Wx{i-1,l} = circshift(imfilter(xl, filter{i}), size(filter{i})-1);
    end
    xl = circshift(imfilter(xl, filter{1}), (size(filter{1})-1) * mod(l,2)); 
    % circshift is a trick, because each time imfilter will shift the image
    % (-0.5,-0.5)pxl
end
y.xL = xl;