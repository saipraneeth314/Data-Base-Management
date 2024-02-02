# Data-Base-Management
Python Code for Query Parsing in SQL querying on CSV files,<br>
C++ code for Lock-manager of Databases Management
---------------------------------------------------------------------------------------------------------
## Lock Manager

The `LockManager.cpp` supports the following capabilities:

1. Lock a resource in either shared or exclusive mode.
2. Unlock a resource held by a transaction.
3. Support for lock upgrade.
4. Support for lock downgrade.

A resource will be identified by a 10 character string.

Typical queries would look like this:

    ret = lock("AAA", 1235, lockType::SHARED);
    ret = lock("AAA", 1243, lockType::EXCLUSIVE);
    upgrade("AAD", 1236);
    downgrade("AAD", 1236);
    unlock("AAD", 4567);
    unlock("AAE", 1237);

---------------------------------------------------------------------------------------------------------
## Query Processor

In `QueryParsing.ipynb`, It takes an SQL statement as input and gives the result of the query as output.<br>

It supports queries like:
1) SELECT
2) FROM
3) WHERE
4) ORDER BY
5) LIMIT
6) DELETE

Typical queries would look like this:

```sql
-- Example 1
SELECT * FROM Data.csv;

-- Example 2
SELECT name, Age FROM Data.csv WHERE name = 'sai';

-- Example 3
SELECT Data.name, student.subject
FROM Data.csv NATURAL JOIN student.csv
WHERE Data.Age >= 25
ORDER BY Data.balance DESC;

-- Example 4
DELETE FROM Data.csv, student.csv
WHERE Data.Age >= 25
ORDER BY Data.name ASC;

-- Example 5
SELECT Data.name, student.class
FROM Data.csv NATURAL JOIN student.csv;

-- Example 6
SELECT * FROM Data.csv LIMIT 5;

-- Example 7
SELECT * FROM Data.csv ORDER BY Data.name;

-- Example 8
DELETE FROM Data.csv WHERE Data.Age >= 50;


-- Example 7
SELECT * FROM Data.csv ORDER BY Data.name;

-- Example 8
DELETE FROM Data.csv WHERE Data.Age >= 50;

