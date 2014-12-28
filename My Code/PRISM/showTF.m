function showTF(fh, y)
% showTF(y) : display all the decomposition of TF
%----------------------------------------------------
% y : signal in TF domain y = {Wx, xL, L}
% fh : figure handle id

% Huayu Zhang, Dec 2014

[M,L] = size(y.Wx);

for l = 1:L
    figure(fh+l-1);
    n = ceil(sqrt(M+1));
    for i = 1:M
        subplot(n,n,i); 
        imagesc(y.Wx{i,l}); colormap(gray); axis equal; axis off;
    end
end

subplot(n,n,M+1);
imagesc(y.xL); colormap(gray); axis equal; axis off;