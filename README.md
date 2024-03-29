This project comes from https://github.com/axenhook/object-file-system

# compile on linux

* run "make -f make_ofs_tools" to generate the demo program  
  
* run "make -f make_ofs_server" to generate the demo program with libevent, you must install libevent first


# compile on windows

* run vs2008 IDE, open win_proj/ofs_tools.sln to generate the demo program
  
* run vs2008 IDE, open unit_test/ofs_test.sln to generate the unit test program
  

# demo program with libevent usage(only support linux now)

1. run ofs_server
  
2. run "telnet IP_ADDRESS 9999" command to connect to the ofs server, if it is on the local machine, the IP_ADDRESS may be "127.0.0.1"  (to quit the telnet, please press "ctrl+]" keys, and then input "quit")
  
3. input the demo program command like the following section
  
# demo program usage example

|description|command example|
|-----------|---------------|
|create container named ct0       | create -ct ct0 |
|create object with objid 300 | create -ct ct0 -o 300|
|insert kv in object 300      | insert -ct ct0 -o 300 -k axen -v abcdjkjkj |
|dump all kv in object 300    | dump   -ct ct0 -o 300 |
|remove kv in object 300      | remove -ct ct0 -o 300 -k axen |

# feature
1. support multi fs
2. support multi object(table) in a single fs
3. support multi type object(table) in a single fs
4. support variant length key or value
5. support NULL value or zero length value

# TODO
1. using btree manager the space instead of bitmap
2. support both b tree and b+ tree
3. support snapshot management
4. support the data on disk is consistent at any time
5. support object read/write
6. support big value
7. support file system operation


