-- Retrieve alphabetically all states
-- with at least one hundred counties.
-- 1.1 marks: <6 operators
-- 1.0 marks: <8 operators
-- 0.8 marks: correct answer

SELECT `abbr`
    FROM `County`
    JOIN `State` ON (`state`=`id`)
    GROUP BY `abbr`
    HAVING COUNT(`abbr`) > 99
    ORDER BY `abbr`;