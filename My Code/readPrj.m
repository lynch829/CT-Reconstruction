function data = readPrj(Nang, Ndet, filename, dir)
% READPRJ : READ PROJECTION DATA
% -------------------------------------------------------
% Nang : number of scan angles
% Ndet : number of detectors
% filename : file name
% dir : direction

% Huayu Zhang, 2014

if(nargin == 3)
    path = filename;
elseif(nargin == 4)
    path = strcat(dir,'\',filename);
end

fid = fopen(path,'rb');
data = fread(fid, [Ndet, Nang], 'single');
fclose(fid);
