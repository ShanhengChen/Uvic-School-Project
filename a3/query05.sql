-- Retrieve in descending order of labour force size
-- all counties that had unemployment rates over 10%
-- in the 2008 census.
-- Hint: Unemployment rate = unemployment / labour force
-- 1.1 marks: <9 operators
-- 1.0 marks: <10 operators
-- 1.0 marks: <15 operators
-- 0.8 marks: correct answer

SELECT `name`, `abbr`, SUM(`labour_force`) AS `labour_force` ,((`CountyLabourStats`.unemployed / `labour_force`)*100) `Unemployment Rate`
    FROM `CountyLabourStats`
    JOIN `County` ON (`county` = `fips`)
    JOIN `State` ON (`id`=`state`)
    WHERE (`year`= 2008)
    Group by `county` ,`Unemployment rate`
    HAVING `Unemployment rate` > 10.000
    ORDER BY `labour_force` DESC
;

