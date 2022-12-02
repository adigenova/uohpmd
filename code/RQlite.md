# RQlite example


## download precompiled binary

There are precompiled binaries for Linux, Mac and windows, here :

```
https://github.com/rqlite/rqlite/releases
```


We download the Mac release of RQlite:

```
curl -L https://github.com/rqlite/rqlite/releases/download/v7.11.0/rqlite-v7.11.0-darwin-amd64.tar.gz -o rqlite-v7.11.0-darwin-amd64.tar.gz
tar xvfz rqlite-v7.11.0-darwin-amd64.tar.gz
cd rqlite-v7.11.0-darwin-amd64
```


## start the first node
 
```
rqlite-v7.11.0-darwin-amd64/rqlited -node-id 1 $PWD/node.1
   
```


## start the second node
 
```

rqlite-v7.11.0-darwin-amd64/rqlited -node-id 2 -http-addr localhost:4003 -raft-addr localhost:4004 -join http://localhost:4001 $PWD/node.2 

```
  
## start the third node
 
```
rqlite-v7.11.0-darwin-amd64/rqlited -node-id 3 -http-addr localhost:4005 -raft-addr localhost:4006 -join http://localhost:4001 $PWD/node.3
```
 
## show open ports
 
 ```
 lsof -i -P -n | grep LISTEN
 ```
 
 
## Start a rqlite session
 
 
 ```
 rqlite-v7.11.0-darwin-amd64/rqlite
 ```
 
 Exec some commands to check the cluster status:
 
 
 ```
 .status
 .nodes
 ```
 
 or by url
 
 ```
 curl localhost:4001/status?pretty
 curl localhost:4001/nodes?pretty
 ```
 
 or even connect to a different node of the cluster
 
 ```
 rqlite-v7.11.0-darwin-amd64/rqlite -p 4003
 ```
 
 
# Add some data to the database
 
 We create a table
 
 ```
 CREATE TABLE IF NOT EXISTS alumno(name text, age number,  phone integer,   rut integer, email text);
 ```
 
 We insert some data in the table:
 
 ```
 INSERT INTO alumno VALUES('Roberto', 22, 977744557, 20456780,'roberto@gmail.com');
 INSERT INTO alumno VALUES('Pascale', 21, 677744557, 20476780,'pascale@gmail.com');
 INSERT INTO alumno VALUES('Dominga', 25, 677744557, 20486780,'dominga@gmail.com');
 INSERT INTO alumno VALUES('Ricardo', 19, 877744557, 20456980,'ricardo@gmail.com'); 
 ```
 
 
# Playing a bit with Rqlite
 
 1. delete a row in the table.
 2. add a new node to the cluster.
 3. kill the new node and made some changes in the table.
 4. kill the current leader.
 5. Alive the last leader.


optionals


 6. dump the database
 7. restore the database from the dump
