import socket

addr = ( socket.INADDR_BROADCAST, 60709 )

UDPSock = socket.socket( socket.AF_INET, socket.SOCK_DGRAM )
if not hasattr( UDPSock, 'SO_BROADCAST' ):
  UDPSock.setsockopt( socket.SOL_SOCKET, socket.SO_BROADCAST, 1 )


print 'Enter your message:'
print '- Empty message to stop this client.'
print '- "stop" to stop all servers.'

while True:
  data = raw_input('>> ')
  if len(data) == 0:
    break
  else:
    if UDPSock.sendto( data, ( "255.255.255.255", 60709 ) ):
      print "Sending message '%s'..." % data

UDPSock.close()             
print 'Client stopped.'
