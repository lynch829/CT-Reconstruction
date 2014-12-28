classdef CTSystem < handle
    % Huayu Zhang, 2014
    properties
        N;  % image size
        L;  % model length:mm
        sp0; % initial source position
        dp0; % initial detectors position
        angle; % acquisition angles
    end
    
    properties(SetAccess = protected)
        SM; % system matrix, sparse
    end
    
    methods
        function CTS = CTSystem(n, lm, s0, d0, angle)
            % CTSYSTEM: CREATE A CT SYSTEM
            % ------------------------------------------
            % n : image size
            % lm : model length
            % s0 : initial position of source
            % d0 : initial position of detectors
            % angle : acquisition angle
            
            CTS.N = n;
            CTS.L = lm;
            CTS.sp0 = s0;
            CTS.dp0 = d0;
            CTS.angle = angle;
        end
        
        function genSM(CTS)
            % GENSM: GENERATE SYSTEM MATRIX
            % ----------------------------------
            disp('Calculating System Matrix...');
            grid.x=linspace(-0.5 * CTS.L, 0.5 * CTS.L, CTS.N + 1);
            grid.y=linspace(-0.5 * CTS.L, 0.5 * CTS.L, CTS.N + 1);
            CTS.SM = sparse(length(CTS.angle)*size(CTS.dp0,2), CTS.N*CTS.N);
            for i=1:length(CTS.angle)%角度循环
                if(mod(i,10)==0)
                    disp(strcat('angle:',num2str(i)));
                end
            %     theta=i*2*pi/scan.Nangle;
%                 szb.x=CTS.sp0(1) * cos(CTS.angle(i)) + CTS.sp0(2) * sin(CTS.angle(i));%源坐标
%                 szb.y=-CTS.sp0(1) * sin(CTS.angle(i)) + CTS.sp0(2) * cos(CTS.angle(i));
                szb.x=CTS.sp0(1) * cos(CTS.angle(i)) - CTS.sp0(2) * sin(CTS.angle(i));%源坐标
                szb.y=CTS.sp0(1) * sin(CTS.angle(i)) + CTS.sp0(2) * cos(CTS.angle(i));
                for k=1:size(CTS.dp0,2)%探测器循环
                    xx0 = CTS.dp0(1,k);
                    yy0 = CTS.dp0(2,k);
%                     dzb.x= xx0*cos(CTS.angle(i))+yy0*sin(CTS.angle(i));%探测器坐标
%                     dzb.y= yy0*cos(CTS.angle(i))-xx0*sin(CTS.angle(i));
                    dzb.x= xx0*cos(CTS.angle(i))-yy0*sin(CTS.angle(i));%探测器坐标
                    dzb.y= yy0*cos(CTS.angle(i))+xx0*sin(CTS.angle(i));
                    sects = get_sect(szb,dzb,grid);%获得交点
                    [id, l] = get_li(sects,grid);
                    if(~isempty(l))
                        CTS.SM((i - 1) * size(CTS.dp0,2) + k, id) = l;%获得交线及位置
                    end
                end
            end
            disp('Calculating System Matrix...end')
        end
        
        function printSysMat(CTS, filename)
            fid = fopen(filename,'w');
            ndet = numel(CTS.dp0) / 2;

            for i = 1:size(CTS.SM,1)
                jdet = mod(i-1,ndet);
                iang = idivide(i-1,int32(ndet),'floor');
                fprintf(fid, 'angle:%d, detector:%d\n', iang, jdet); 
                for j = 1:size(CTS.SM,2)
                    fi = mod(j-1,CTS.N);
                    fj = idivide(j-1,int32(CTS.N));
                    value = full(CTS.SM(i,j));
                    if(abs(value) > 1e-15)
                        fprintf(fid, '(%d,%d): %f\n', fi, fj, full(CTS.SM(i,j)));
                    end
                end
            end
            fclose(fid);
        end
       
    end  % method
    
end
    