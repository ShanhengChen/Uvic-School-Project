function root = Newton( x0, eps , imax )
i = 1;
fprintf ( ' iteration approximation \n');
while i <= imax
    root = x0 - f(x0)/fp(x0);
    fprintf ( ' %6.0f %18.8f \n', i, root );
    if abs(1-(x0/root)) < eps
        return
    end
    i = i + 1;
    x0 = root;
end
    fprintf ( ' failed to converge in %g iterations\n', imax )
end