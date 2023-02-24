function v = falling_parachutist(m)
    c = 13.5;
    t = 10;
    g = 9.81;
    v = ((g*m)/c)*(1-exp(-(c*t)/m)) - 40;
end