-- Retrieve the fifteen counties with the largest 2016 vote imbalance,
-- with their vote counts and states, restricted to counties with at least 10000 votes
-- Hint: Use pq to measure variance/imbalance in this question,
-- where p is the probability of voting democrat and q, republican.
-- 1.1 marks: <11 operators
-- 1.0 marks: <12 operators
-- 0.9 marks: <15 operators
-- 0.8 marks: correct answer

SELECT name, `abbr`,`dem`,`gop`,`total_votes`
    FROM `ElectionResult` 
    JOIN `County` ON(`county`= `fips`)
    JOIN `State` ON (`state`=`id`)
    WHERE `ElectionResult`.year = 2016 AND `ElectionResult`.total_votes > 10000 
    ORDER BY (`dem`*`gop`)/(`total_votes`*`total_votes`)
    limit 15;