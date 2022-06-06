# chat_client.py

import sys, socket, select
import datetime
 
def chat_client():
    if(len(sys.argv) < 3) :
        print 'Usage : python chat_client.py hostname port'
        sys.exit()

    host = sys.argv[1]
    port = int(sys.argv[2])
     
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.settimeout(2)
     
    # connect to remote host
    try :
        s.connect((host, port))
    except :
        print 'Unable to connect'
        sys.exit()
     
    print 'Connected to remote host. You can start sending messages'
    sys.stdout.write('[Me] '); sys.stdout.flush()
     
    stdinWaitingTime = 0.0

    while 1:
        socket_list = [sys.stdin, s]
         
        # Get the list sockets which are readable
        t1 = datetime.datetime.now()
        read_sockets, write_sockets, error_sockets = select.select(socket_list , [], [], 30.0 - stdinWaitingTime)
        t2 = datetime.datetime.now()
        deltaT = t2 - t1

        if sys.stdin in read_sockets:
            stdinWaitingTime = 0;
        elif s in read_sockets:
            stdinWaitingTime = stdinWaitingTime + float(deltaT.seconds + deltaT.microseconds * 0.000001)

        for sock in read_sockets:            
            if sock == s:
                # incoming message from remote server, s
                data = sock.recv(4096)
                if not data :
                    print '\nDisconnected from chat server'
                    sys.exit()
                else :
                    #print data
                    sys.stdout.write(data)
                    sys.stdout.write('[Me] '); sys.stdout.flush()     
            
            elif sock == sys.stdin:
                # user entered a message
                msg = sys.stdin.readline()
                s.send(msg)
                sys.stdout.write('[Me] '); sys.stdout.flush() 

        if (sys.stdin not in read_sockets) and (s not in read_sockets):
            # idle mode
            s.send("timeout")
            print "timed out, go to idle mode\n"
            print '\nDisconnected from chat server'
            sys.exit()


if __name__ == "__main__":

    sys.exit(chat_client())


