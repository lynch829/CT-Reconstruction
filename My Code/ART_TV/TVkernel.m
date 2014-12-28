function fn = TVkernel(f, a, d, TV, Ntv)
% TVKERNEL : ONE TOTAL VIRIATION INTERATION 
% ----------------------------------------------
% f : last image
% a : relaxation factor
% d : difference norm of current image and last image
% TV : TV gradient
% Ntv : number of iteration times
% fn : new image

% Huayu Zhang, 2014
% Please cite ShenLe's master thesis:Spectral 
% Computed Tomography Imaging System Design and Reconstruction, 2013

eps = 1e-8;     % in case zero division
for i = 1:Ntv
    fe = padarray(f,[1 1]);
    t1 = ( (f-fe(1:end-2,2:end-1)) + (f-fe(2:end-1,1:end-2)) ) ...
        ./ sqrt(eps + (f-fe(1:end-2,2:end-1)).^2 + (f-fe(2:end-1,1:end-2)).^2);
    t2 = (fe(3:end,2:end-1) - f)./sqrt(eps + (fe(3:end,2:end-1) - f).^2 + ...
        (fe(3:end,2:end-1) - fe(3:end,1:end-2)).^2);
    t3 = (fe(2:end-1,3:end) - f)./sqrt(eps + (fe(2:end-1,3:end) - f).^2 + ...
        (fe(2:end-1,3:end) - fe(1:end-2,3:end)).^2);
    v = t1.*TV(1:end-1,1:end-1) - t2.*TV(2:end,1:end-1) - t3.*TV(1:end-1,2:end);
    v = v / norm(v(:));
    f = f - a*d*v;
end

fn = f;