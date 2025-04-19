# Data-Base-Management

## Transaction Lock Manager

### Overview
Lock Manager for Concurrent Transaction Coordination is a systems-level project built to manage synchronized access to shared resources in environments with multiple concurrent transactions. It ensures data consistency and system integrity by implementing a robust locking mechanism that supports both shared (read) and exclusive (write) locks.

Designed with a focus on concurrency control, this lock manager is ideal for use in database systems and multi-threaded applications. It handles complex scenarios such as lock acquisition, release, upgrade, and downgrade, effectively preventing conflicts and minimizing contention. The system guarantees safe, deadlock-free resource handling through precise coordination and conflict resolution strategies, showcasing a deep understanding of operating system design and synchronization techniques.

---

The `LockManager.cpp` supports the following capabilities:

1. Lock a resource in either shared or exclusive mode.
2. Unlock a resource held by a transaction.
3. Support for lock upgrade.
4. Support for lock downgrade.

A resource will be identified by a 10-character string.

Typical queries would look like this:

    ret = lock("AAA", 1235, lockType::SHARED);
    ret = lock("AAA", 1243, lockType::EXCLUSIVE);
    upgrade("AAD", 1236);
    downgrade("AAD", 1236);
    unlock("AAD", 4567);
    unlock("AAE", 1237);

---------------------------------------------------------------------------------------------------------
## SQL Query Processor

### Overview
Custom SQL Query Processor is a lightweight engine designed to execute standard SQL operations on CSV-formatted data, mimicking the behavior of relational database systems. This project showcases a deep understanding of SQL query parsing, execution planning, and data handling without relying on traditional database backends.

The processor supports essential SQL clauses such as SELECT, WHERE, ORDER BY, LIMIT, and DELETE, along with efficient execution of natural and Cartesian joins. Its modular design ensures smooth parsing and execution, enabling flexible query processing over structured CSV datasets.

This project highlights strong proficiency in database internals, query parsing, and algorithmic logic—bridging the gap between raw data storage and relational querying.

---

`QueryParsing.ipynb` takes an SQL statement as input and gives the result of the query as output.<br>

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

