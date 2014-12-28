function [id, l] = get_li(sects,grid)
if length(sects)<2
    id = [];
    l = [];
else
    N = length(grid.x)-1;
    nsect = length(sects);
    midx = (sects(1,1:nsect-1)+sects(1,2:nsect))/2;
    midy = (sects(2,1:nsect-1)+sects(2,2:nsect))/2;
    l = sqrt((sects(2,1:nsect-1)-sects(2,2:nsect)).^2+(sects(1,1:nsect-1)-sects(1,2:nsect)).^2);
    ps = grid.x(2)-grid.x(1);%pixel size
    nx = floor((midx-grid.x(1))/ps)+1;
    ny = floor((midy-grid.y(1))/ps)+1;
    nx(nx<1) = 1; nx(nx>N) = N;
    ny(ny<1) = 1; ny(ny>N) = N;
%     id = sub2ind([N,N],N-ny+1,nx);%一维坐标
    id = sub2ind([N,N],ny,nx);%一维坐标
end