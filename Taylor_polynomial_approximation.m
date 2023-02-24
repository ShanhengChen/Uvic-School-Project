function Taylor_polynomial_approximation(x)
first = 2 + (x-1)/4;
display1 = round(first,5);
fprintf('values of first term approximations is: \t')
fprintf('%8.4f\n',display1)
second = first - (x-1)^2/64;
fprintf('values of second term approximations is:\t')
display2 = round(second,5);
fprintf('%8.4f\n',display2)
thrid = second + (x-1)^3/512;
display3 = round(thrid,5);
fprintf('values of thrid term approximations is: \t')
fprintf('%8.4f\n',display3)

