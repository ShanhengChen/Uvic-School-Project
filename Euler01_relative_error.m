function Euler01_relative_error(m,c,g,t)
fprintf('The true (exact) solution (consider as P)at time t = 12 is -->')
temp = (g*m)/c;
exponent = -(c*t)/m;
v = temp* (1-exp(exponent));
fprintf('\t%8.3f\n',v)
fprintf(['Since we know at approximation to p (consider as P*)\nat time t = 12 ' ...
    'from 1.b which is ----> 57.0088 \n'])
relative_error = abs(1 - 57.0088/v);
fprintf(['values of relative error in the computed ' ...
    'approximation at time t = 12 is \n'])
fprintf('-->%8.3f\n',relative_error)

end