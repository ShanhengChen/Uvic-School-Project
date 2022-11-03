-- Retrieve by increasing snowfall the number of employees
-- in 'Mining, quarrying, and oil and gas extraction' for all (id =19)
-- counties that have the words 'iron', 'coal', or 'mineral'
-- in their name.
-- 1.1 marks: <13 operators
-- 1.0 marks: <15 operators
-- 0.9 marks: <20 operators
-- 0.8 marks: correct answer

--IF(EXISTS(`CountyIndustries`.employees FROM `CountyIndustries` WHERE industry= 19), `CountyIndustries`.employees, ) 

SELECT DISTINCT `name`,`abbr`, employees 
    FROM `County`
    JOIN `State` ON (`state` = `id`)
    JOIN `CountyIndustries` ON (`county` = `fips` )
    WHERE (`name` like '%iron%' or `name` like '%coal%' or `name` like '%mineral%') AND (industry= 19)
     ;



