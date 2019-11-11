# /usr/bin/expect -c 'spawn scp -P 14601 raw_sock.c   tc@localhost:~/; expect "password:"; send "user@123\n"; interact'

/usr/bin/expect -c 'spawn scp -P 14601 q1_sniffer.cpp   tc@localhost:~/; expect "password:"; send "user@123\n"; interact'
/usr/bin/expect -c 'spawn scp -P 14601 q2_sniffer_modified.cpp   tc@localhost:~/; expect "password:"; send "user@123\n"; interact'
/usr/bin/expect -c 'spawn scp -P 14502 packet_sender.cpp   tc@localhost:~/; expect "password:"; send "user@123\n"; interact'

/usr/bin/expect -c 'spawn scp -P 14601 makefile   tc@localhost:~/; expect "password:"; send "user@123\n"; interact'
# /usr/bin/expect -c 'spawn scp -P 14502 makefile   tc@localhost:~/; expect "password:"; send "user@123\n"; interact'