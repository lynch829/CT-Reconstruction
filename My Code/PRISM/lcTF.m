function Y = lcTF(varargin)
% LCTF : Linear combination of TF
% ---------------------------------------------
% Y = lbTF(a1, Y1, a2, Y2, ...)
% Y1,Y2,...: data in Tight Frame domain
% a1,a2,...: coefficient
% Y = a1 * Y1 + a2 * Y2 + ...;
% format: {Wx, xL, L}, Wx = cell(nf-1,L)

% Huayu Zhang, Dec 2014

[M,L] = size(varargin{2}.Wx);
for i = 1:M
    for j = 1:L
        % construct 3D matrix
        m = numel(varargin) / 2;
        Mat = zeros([size(varargin{2}.Wx{i,j}),m]);

        for k = 1:m
            Mat(:,:,k) = varargin{k*2}.Wx{i,j} * varargin{k*2-1};
        end
        Y.Wx{i,j} = sum(Mat,3);
    end
end


% construct 3D matrix
m = numel(varargin) / 2;
Mat = zeros([size(varargin{2}.xL),m]);

for k = 1:m
    Mat(:,:,k) = varargin{k*2}.xL * varargin{k*2-1};
end
Y.xL = sum(Mat,3);
Y.L = L;