function sects = get_sect(A,B,grid)
epsilong = 1e-6;
if abs(A.x-B.x)<epsilong%垂直于x轴
    xx0 = (A.x+B.x)/2;
    if xx0>=grid.x(1) && xx0<=grid.x(length(grid.x))
        sects = zeros(2,length(grid.y));
        sects(1,:) = xx0;
        sects(2,:) = grid.y;
    else
        sects = [];
    end
end
if abs(A.y-B.y)<epsilong%垂直于y轴
    yy0 = (A.y+B.y)/2;
    if yy0>=grid.y(1) && yy0<=grid.y(length(grid.y))
        sects = zeros(2,length(grid.y));
        sects(2,:) = yy0;
        sects(1,:) = grid.x;
    else
        sects = [];
    end
end
if abs(A.x-B.x)>=epsilong && abs(A.y-B.y)>=epsilong%正常
    yy0 = (grid.x-B.x)*(B.y-A.y)/(B.x-A.x)+B.y;
    secty = [grid.x(yy0>=grid.y(1)&yy0<=grid.y(length(grid.y)));yy0(yy0>=grid.y(1)&yy0<=grid.y(length(grid.y)))];%排除不在grid内的点
    xx0 = (grid.y-B.y)*(B.x-A.x)/(B.y-A.y)+B.x;
    sectx = [xx0(xx0>=grid.x(1) & xx0<=grid.x(length(grid.x)));grid.y(xx0>=grid.x(1) & xx0<=grid.x(length(grid.x)))];
    sects = [sectx,secty];
    [xx,ii] = unique(sects(1,:));
    yy = sects(2,ii);
    [xx,ii] = sort(xx);
    yy = yy(ii);
    sects = [xx;yy];
end