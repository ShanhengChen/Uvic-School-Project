function Euler2(m,k,g,t0,v0,tn,n)
fprintf('values of t approximations v(t)\n')
fprintf('%8.3f',t0),fprintf('%19.4f\n',v0)
h=(tn-t0)/n;
t=t0;
v=v0;
for i=1:n
    v=v+(g-k/m*v^2)*h;
    t=t+h;
    fprintf('%8.3f',t),fprintf('%19.4f\n',v)
end