import socket

addr = ( '', 60709 )

UDPSock = socket.socket( socket.AF_INET, socket.SOCK_DGRAM )
if not hasattr( UDPSock, 'SO_BROADCAST' ):
  UDPSock.setsockopt( socket.SOL_SOCKET, socket.SO_BROADCAST, 1 )

UDPSock.bind( addr )

while True:
    data, addr = UDPSock.recvfrom( 1024 )
    print addr
    if data == 'stop':
        print 'Client wants me to stop.'
        break
    else:
        print "From addr: '%s', msg: '%s'" % (addr[0], data)
    UDPSock.sendto( data, len( data ), addr )

UDPSock.close()
print 'Server stopped.'