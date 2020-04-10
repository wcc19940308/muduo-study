#/usr/bin/python3
import errno
import socket
import sys
import time

if len(sys.argv) < 2:
    print ("Usage: %s port" % sys.argv[0])
    print ("port shuold in net.ipv4.ip_local_port_range")
else:
    port = int(sys.argv[1]) # specify port number
    for i in range(65536):
        try:
            sock = socket.create_connection(('localhost'), port)
            print ("connected", sock.getsockname(), sock.getpeername())
            time.sleep(60 * 60)
        except socket.error as e:
            if e.errno != errno.ECONNREFUSED:
                break