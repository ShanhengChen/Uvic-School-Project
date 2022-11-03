-- Retrieve alphabetically the states that had
-- over 100 counties with unemployment rates above 6.0%
-- in 2008.
-- Hint: Unemployment rate = unemployed / labour force AND (`unemployed`/`labour_force`)*100 > 6
-- 1.1 marks: <8 operators
-- 1.0 marks: <9 operators
-- 0.9 marks: <11 operators
-- 0.8 marks: correct answer

SELECT `abbr`
FROM `CountyLabourStats` 
JOIN `County` ON (`fips`= `county`)
JOIN `State` ON (`state`= `id`)
WHERE year = 2008 and  (`unemployed`/`labour_force`)*100 > 6
GROUP BY `abbr`
HAVING SUM((`unemployed`/`labour_force`)*100 > 6) > 100
ORDER BY `abbr`; 