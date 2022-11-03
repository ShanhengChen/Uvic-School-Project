-- Show the percentage of counties that have more
-- females than males.
-- 1.1 marks: <8 operators
-- 1.0 marks: <10 operators
-- 0.9 marks: <13 operators
-- 0.8 marks: correct answer


SELECT (COUNT(`aa`.county)/(SELECT COUNT(`county`) FROM GenderBreakdown))*2 Fraction
    FROM `GenderBreakdown` aa
    JOIN `GenderBreakdown` bb
    WHERE (`aa`.county = `bb`.county)  AND (`aa`.population > `bb`.population) AND (`aa`.gender = 'female');
