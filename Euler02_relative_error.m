function Euler02_relative_error(m,k,g,t)
fprintf('The true (exact) solution (consider as P)at time t = 18 is -->')
temp_1 = sqrt((g*m)/k);
temp_2 = sqrt((g*k)/m);
v = temp_1*tanh(temp_2*t);
fprintf('\t%8.3f\n',v)
fprintf(['Since we know at approximation to p (consider as P*)\nat time t = 18 ' ...
    'from 2.b which is ----> 55.3583 \n'])
relative_error = abs(1 - 55.3583/v);
fprintf(['values of relative error in the computed ' ...
    'approximation at time t = 18 is \n'])
fprintf('-->%8.3f\n',relative_error)
end