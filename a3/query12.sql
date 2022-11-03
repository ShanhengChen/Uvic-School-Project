-- Retrieve alphabetically the names of industries that
-- employ at least five million workers across
-- the US, excluding California.
-- 1.1 marks: <9 operators
-- 1.0 marks: <11 operators
-- 0.9 marks: <14 operators
-- 0.8 marks: correct answer

SELECT `Industry`.name
    FROM `Industry`
    JOIN `CountyIndustries` ON (`Industry`.id = `industry`)
    JOIN `County` ON (`fips`= `CountyIndustries`.county)
    JOIN `State` ON (`State`.id=`state`)
    WHERE `fips` NOT IN (SELECT `fips`
            FROM `County`
            JOIN `State` ON (`id`=`state`)
            WHERE `state` = 5)
    GROUP BY `name`
    HAVING SUM(`employees`) > 5000000
    ORDER BY `name`;
