function McLaurin_series()

%fprintf("e^-2 : %8.6f \n", exp(-2))
 %fprintf('%8.3f',y*x);
 fprintf("  first n term approximated by its McLaurin series expansion \n");
for z = 1 : 5
     fprintf(' n = %d \t',z);
     e = 1;
     y = 1;
     exponent = 2;
    for x = 1: z
        y = -y;
        fac = y*exponent^x/factorial(x);
         %fprintf('%8.3f \t',fac);
        e =  e + fac;
    end
        fprintf('first approximately equal to %8.6f \t',e);
        relative_error =abs( 1 - e/exp(-2));
        fprintf('relative error equal to %8.6f \n',relative_error);
end
fprintf('----------------------------------------------------------\n');
for z = 1 : 5
     fprintf(' n = %d \t',z);
     base = 1;
     exponent = 2;
    for x = 1: z
        fac = exponent^x/factorial(x);
         %fprintf('%8.3f \t',fac);
        base =  base + fac;
        result = 1/base;
    end
        fprintf('second approximately equal to %8.6f \t',result);
        relative_error =abs( 1 - result/exp(-2));
        fprintf('relative error equal to %8.6f \n',relative_error);
end
        fprintf(['since relative error of seond apprximation is relatively smaller \n' ...
            'then I would conclude that second approximation is better '])
end
    