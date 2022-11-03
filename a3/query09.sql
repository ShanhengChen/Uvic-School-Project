-- Show which industries in which states (except DC)
-- employed at least 7.5% of the state's 2019 population,
-- ordered by the total payroll for that industry
-- in that state.
-- 1.1 marks: <13 operators
-- 1.0 marks: <15 operators,sum(population)
-- 0.9 marks: <20 operators
-- 0.8 marks: correct answer
SELECT aa.abbr, `Industry`.name, SUM(payroll) `Total Payrolls`, (sum(employees)/ bb.xd)*100 `% of Population`
    FROM `CountyPopulation` 
    JOIN `County`ON (`CountyPopulation`.county = `fips`)
    JOIN `State` aa ON (`state` = `aa`.id)
    JOIN `CountyIndustries` ON( `CountyIndustries`.county = `fips`)
    JOIN `Industry` ON (`Industry`.id = `CountyIndustries`.industry)
    INNER JOIN (SELECT abbr,sum(population) xd
FROM `CountyPopulation` 
    JOIN `County`ON (`CountyPopulation`.county = `fips`)
    JOIN `State`ON (`state` = `State`.id)
    WHERE year = 2019 AND NOT abbr = 'DC'
    GROUP BY abbr
) AS bb ON aa.abbr = bb.abbr
    WHERE `CountyPopulation`.year = 2019 AND NOT aa.abbr = 'DC'
    GROUP BY aa.abbr,industry
    HAVING `% of Population` > 7.5
    ORDER BY `Total Payrolls` DESC;



