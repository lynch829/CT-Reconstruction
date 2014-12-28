%% CT Reconstruction for play toy
% a 300mm*300mm square, 8*8 size
% Huayu Zhang, 2014

close all; clear; clc;
%% Add paths
addpath('./SystemMatrix');
addpath('./Recon');
addpath('./ART_TV');

%% Load Projection Data
P1 = readPrj(360, 10, 'cube300mm.prj', 'data');   % 360 angles and 10 detectors, mu = 1
%P2 = readPrj(360, 10, 'cylinder_E2.prj', 'data');% 360 angles and 72 detectors, mu = 0.7

%% System Parameters
dos = 1200.0;            % distance between source and object
dod = 1200.0;            % distance between the detector array and object
ld = 1600.0;            % the length of the detector array
Ndet = 10;              % number of detectors
N = 8;                 % image size
lm = 800.0;             % length of model
theta = linspace(0,2 * pi - pi/180,360);      % angle rings
angleIndex = 1:45:360;      % acquisited angles 

% calculate coordinates
s0 = [-dos;0];
d0 = [dod*ones(1,Ndet);linspace(-(ld-ld/Ndet)/2,(ld-ld/Ndet)/2,Ndet)];

%% Construct a CT system
CTS1 = CTSystem(N, lm, s0, d0, theta(angleIndex));
CTS1.genSM();
%% Reconstruction
Pu1 = P1(:,angleIndex); %Pu2 = P2(:,angleIndex);

% conjugate gradient
% ATA = sparse(CTS1.SM' * CTS1.SM);
% f1 = cgs(ATA,CTS1.SM'* Pu1(:));  I1 = reshape(f1,[N,N]);
% figure(1); imagesc(I1); colormap(gray); axis equal; axis off; colorbar;

% ART 
f0 = zeros(N*N,1);
[I2, cv2] = ART(Pu1(:), CTS1.SM, f0, 10, 1);
I2 = reshape(I2,[N,N]);
figure(2); imagesc(I2); colormap(gray); axis equal; axis off; colorbar;

% print SM
CTS1.printSysMat('system_matrix.txt');
% % ART_TV_PRIORI
% load('./data/IP64.mat');  %I_prior
% [I3, cv] = ARTTV_priori(Pu1, CTS1.SM, I_prior, 20, 1, 20);
% figure(3); imagesc(I3); colormap(gray); axis equal; axis off; colorbar;

% PRISM
