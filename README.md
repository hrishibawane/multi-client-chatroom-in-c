## Multi-client chatroom using sockets and threads in C
Linux-based simple multi-client chatroom using socket communication and threads for handling multiple client connections.

### Execution

1. Clone repository

2. Compile options:
    1. ```make all``` => to compile both server and client
    2. ```make server``` => to compile server
    3. ```make client``` => to compile client
    4. ```make clean``` => to clear all executables

3. Execute options:
    - run ```./server``` to run server in one terminal
    - run ```./client``` in another terminal window/tab

4. Type "exit" or press ctrl-c to quit client.

Note: Change the IP address on line 16 of client.c source file to your local network IP address if you wish to run client on another linux machine connected to local network.
