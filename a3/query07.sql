-- Show which county has the largest relative population decrease
-- from 2010 to 2019.
-- 1.1 marks: <11 operators
-- 1.0 marks: <13 operators
-- 0.9 marks: <16 operators
-- 0.8 marks: correct answer


-- Replace this comment line with the actual query

SELECT `County`.name AS `name`,  `aa`.population AS `2010`, `bb`.population AS `2019`, `abbr`, ((`aa`.population - `bb`.population)/`aa`.population)*100 AS `Loss (%)`
    FROM `CountyPopulation` AS `aa`
    JOIN `CountyPopulation` AS `bb` ON (`aa`.county = `bb`.county)
    JOIN `County` ON(`aa`.county= `fips`)
    JOIN `State` ON (`state`=`id`)
    WHERE ( `bb`.year = 2019 ANd `aa`.year = 2010) 
    ORDER BY `Loss (%)` DESC
    limit 1;


