function Euler(m,c,g,t0,v0,tn,n)
% print headings and initial conditions
fprintf('values of t approximations v(t)\n')
fprintf('%8.3f',t0),fprintf('%19.4f\n',v0)
% compute step size h
h=(tn-t0)/n;
% set t,v to the initial values
t=t0;
v=v0;
% compute v(t) over n time steps using Euler's method
for i=1:n
    v=v+(g-c/m*v)*h;
    t=t+h;
    fprintf('%8.3f',t),fprintf('%19.4f\n',v)
end

