%% Test TF transform
clear; close all; clc;
load('./data/IP64.mat');  %I_prior

add_paths
w = cell(2,2);
w{1,1} = [1,1;1,1] / 4;
w{1,2} = [1,-1;1,-1] / 4;
w{2,1} = [1,1;-1,-1] / 4;
w{2,2} = [1,-1;-1,1] / 4;

L = 5;

y = TF(I_prior, w, L);
showTF(1,y);
x = ITF(y, w);

figure;
imagesc([I_prior, x, x - I_prior]); colormap(gray); axis equal; axis off; colorbar;
xlabel('origin, ITF, error');
cd results;
saveas(gcf,'ITF','jpg'); close(gcf);

for i = L:-1:1
    saveas(gcf,['TF',num2str(i)],'jpg');
    close(gcf);
end
cd ..;