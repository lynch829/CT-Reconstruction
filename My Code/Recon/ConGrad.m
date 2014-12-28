function [x, it] = ConGrad(A, b, x0, k, epsilon)
% CONGRAD: CONJUGATE GRADIENT
% --------------------------------------------------
% Solve A*x = b
% x0 : initial x
% k : iteration times
% epsilon : precision
% x : solution
% it : iteration times

% Copyright 2014 (c) Huayu Zhang, all rights reserved

x = x0; it = 0;

if(iscell(A))
    r = b - sysMatProd(A, x);
    p = r;
    preci = sqrt(r'*sysMatProd(A, r));
    while(abs(preci) > epsilon && it < k)
        Ap = sysMatProd(A, p);
        alpha = (r'*r) / (p'*Ap);
        x = x + alpha * p;
        r_next = r - alpha*Ap;
        beta = (r_next'*r_next) / (r'*r);
        p = r_next + beta * p;
        r = r_next;
        preci = sqrt(r'*sysMatProd(A, r));
        it = it + 1;
        fprintf(1,'CG %d, alpha = %f, beta = %f, error = %f\n', it, alpha, beta, preci);
    end    
            
else
    r = b - A * x;
    p = r;
    preci = sqrt(r'*(A*r));
    while(abs(preci) > epsilon && it < k)
        alpha = (r'*r) / (p'*(A*p));
        x = x + alpha * p;
        r_next = r - alpha*A*p;
        beta = (r_next'*r_next) / (r'*r);
        p = r_next + beta * p;
        r = r_next;
        preci = sqrt(r'*(A*r));
        it = it + 1;
        fprintf(1,'CG %d, alpha = %f, beta = %f, error = %f\n', it, alpha, beta, preci);
    end
end
    


