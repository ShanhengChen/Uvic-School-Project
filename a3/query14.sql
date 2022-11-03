-- Out of those counties with at least 25000 residents,
-- retrieve the pair from the same state
-- that had the absolute closest
-- population in 2018
-- 1.1 marks: <11 operators
-- 1.0 marks: <12 operators
-- 0.9 marks: <14 operators
-- 0.8 marks: correct answer

SELECT bb.name,dd.population, aa.name,cc.population
    FROM `County` aa
    JOIN `County` bb
    JOIN `State` 
    JOIN `CountyPopulation` cc ON (`aa`.fips = `cc`.county)
    JOIN `CountyPopulation` dd ON (`bb`.fips = `dd`.county)
    WHERE aa.name <> bb.name AND aa.state = bb.state AND (cc.year = 2018 AND dd.year = 2018) AND (cc.population >25000 AND dd.population >25000)
    ORDER BY ABS(cc.population - dd.population )
    limit 1;
