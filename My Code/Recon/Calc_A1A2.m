function [A1,A2] = Calc_A1A2(PL,PH)
%Ë«Ð§Ó¦²éÕÒ
%load lut_ls_gos;
fid = fopen('lut.dat','rb');
zi1 = fread(fid,[2001,2001],'double');
zi2 = fread(fid,[2001,2001],'double');
fclose(fid);

[xi,yi] = meshgrid(0:.0005:1,0:.0005:1);
A1 = interp2(xi,yi,zi1,PL,PH);
A2 = interp2(xi,yi,zi2,PL,PH);
