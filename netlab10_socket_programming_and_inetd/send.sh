#!/bin/bash
# scp -P 14501 -r h1_tcpclient.c tc@localhost:/home/tc
/usr/bin/expect -c 'spawn scp -P 14501 -r h1_tcpclient.c tc@localhost:/home/tc; expect "password:"; send "user@123\n"; interact'
/usr/bin/expect -c 'spawn scp -P 14502 -r h2_add_tcpserver.c tc@localhost:/home/tc; expect "password:"; send "user@123\n"; interact'
/usr/bin/expect -c 'spawn scp -P 14503 -r h3_udpserver.c tc@localhost:/home/tc; expect "password:"; send "user@123\n"; interact'

# /usr/bin/expect -c 'spawn scp -P 14501 -r makefile tc@localhost:/home/tc; expect "password:"; send "user@123\n"; interact'
# /usr/bin/expect -c 'spawn scp -P 14502 -r makefile tc@localhost:/home/tc; expect "password:"; send "user@123\n"; interact'
# /usr/bin/expect -c 'spawn scp -P 14503 -r makefile tc@localhost:/home/tc; expect "password:"; send "user@123\n"; interact'