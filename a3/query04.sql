-- Retrieve alphabetically all states in which
-- every county has a name not found anywhere else
-- in the US
-- 1.1 marks: <8 operators
-- 1.0 marks: <9 operators
-- 0.8 marks: correct answer HAVING count(name) = 1
-- sub sub query 
SELECT `abbr` FROM `State` WHERE `abbr` 
    NOT IN (select DISTINCT `abbr`
        from `County`
        JOIN `State` ON (`id` = `state`)
            WHERE `name` 
            IN (SELECT `name`
                    FROM `County` 
                    GROUP BY `name`
                    HAVING count(`name`) > 1)) 
    ORDER BY `abbr`;

    
