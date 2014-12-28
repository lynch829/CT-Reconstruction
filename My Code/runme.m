%% CT Reconstruction
% Huayu Zhang, 2014

close all; clear; clc;

add_paths % Add paths

%% Add paths
addpath('./SystemMatrix');
addpath('./Recon');
addpath('./ART_TV');

%% Load Projection Data
P1 = readPrj(360, 72, 'cylinder.prj', 'data');   % 360 angles and 72 detectors, mu = 1
P2 = readPrj(360, 72, 'cylinder_E2.prj', 'data');% 360 angles and 72 detectors, mu = 0.7


%% System Parameters
dos = 600.0;            % distance between source and object
dod = 600.0;            % distance between the detector array and object
ld = 1440.0;            % the length of the detector array
Ndet = 72;              % number of detectors
Nang = 360;             % number of angles
N = 64;                 % image size
lm = 600.0;             % length of model
theta = linspace(0,2 * pi - pi/180,360);      % angle rings
angleIndex = 1:6:216;      % acquisited angles 
detIndex = 1:Ndet;         % detectors 

%% calculate coordinates
s0 = [-dos;0];
d0 = [dod*ones(1,Ndet);linspace(-(ld-ld/Ndet)/2,(ld-ld/Ndet)/2,Ndet)];


%% Construct a CT system
CTS1 = CTSystem(N, lm, s0, d0, theta(angleIndex));
CTS1.genSM();
%% Reconstruction

% conjugate gradient
Pu1 = P1(:,angleIndex); Pu2 = P2(:,angleIndex);
ATA = sparse(CTS1.SM' * CTS1.SM);

f1 = cgs(ATA,CTS1.SM'* Pu1(:));  I1 = reshape(f1,[N,N]);
figure(1); imagesc(I1); colormap(gray); axis equal; axis off; colorbar;

% ART 
f0 = zeros(N*N,1);
[I2, cv2] = ART(Pu1(:), CTS1.SM, f0, 200, 1);
I2 = reshape(I2,[N,N]); 
figure(2); imagesc(I2); colormap(gray); axis equal; axis off; colorbar;

% ART_TV_PRIORI
load('./data/IP64.mat');  %I_prior
[I3, cv] = ARTTV_priori(Pu1, CTS1.SM, I_prior, 20, 1, 20);
figure(3); imagesc(I3); colormap(gray); axis equal; axis off; colorbar;

% PRISM
w = cell(2,2);
w{1,1} = [1,1;1,1] / 4; w{1,2} = [1,-1;1,-1] / 4;
w{2,1} = [1,1;-1,-1] / 4; w{2,2} = [1,-1;-1,1] / 4;
lambda1 = 300;

[XL, XS] = prism(CTS1.SM, Pu1(:), w, 5, lambda1, 10);
I4 = XL + XS; I4(I4 < 0) = 0;
figure(4); imagesc(I4); colormap(gray); axis equal; axis off; colorbar;

%% Compare
figure(5); imagesc([I1,I2,I3,I4]); colormap(gray); axis equal; axis off; colorbar;
title('ART, Conjugate gradient, Total Variation with a priorier image, PRISM');

