#!/usr/bin/python
import sys
import socket
#import androidhelper
import time
#from time import gmtime, strftime
from sys import stdout
BufSize =332768
def RSHash ():
    import time
    str = time.strftime("%M%H%w")
    #print str

    b    = 378551
    a    = 63689
    hash = 0

    for x in str:
        hash = hash * a + int(x)+48
        a    = a * b

    return hash & 0x7FFFFFFF

main_loop =False
my_bytes = " "
#droid = androidhelper.Android()
#print("parametry: %s" % sys.argv)
# create a socket object
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# get local machine name
if len(sys.argv) >= 3 :
    host = sys.argv[1]
    port = int(sys.argv[2])
    #print("podales dobre argumenty")
else:
    print("adres i port domyslny ")
    #host = "192.168.1.144"
    host = "192.168.1.144"
    #host = "cyniu88.no-ip.pl"
    port = int(8833)
if len(sys.argv) >= 4 :
     my_bytes = " ".join(sys.argv[3:])
    #print "MOJE PODANE:", my_bytes


# connection to hostname on the port.

s.connect((host, port))

s.send(str(RSHash ()))
tm = s.recv(BufSize)
#print "pierwsze odebranie", tm
s.send("OK")
tm = s.recv(BufSize)
#print "drugie odebranie", tm

if (tm[1]  == "F") and (tm[2] == "A") and (tm[3]== "I"):
    print ("AUTHENTICATION FAILED!")
    exit()
if (tm[0] == "O" and tm[1] == "K" ):
        main_loop=True
######################## user level ###########################
#print "user level"
s.send("ROOT")
tm = s.recv(BufSize)
#print "drugie odebranie", tm
s.send("OK")
tm = s.recv(BufSize)
#print tm

######################## main loop  ###########################
while main_loop:
    if len(sys.argv) >=4:
        main_loop=False
    else:
        t = time.strftime("%H:%M")
       # droid.vibrate(1500)
        my_bytes = raw_input(" %s iDom: " % t)
        print("> %s" % my_bytes.decode('ascii'))
    if my_bytes == "exit" :
        s.send(my_bytes)
        tm = s.recv(BufSize)
        s.send("OK")
        tm = s.recv(BufSize)
        print( tm)
        break

    s.send(my_bytes)
    #s.send()
    tm = s.recv(BufSize)
    s.send("OK")
    #print " pierwsze odbieram wielkosc" ,tm
    len_to_recv = int(tm)
    tm = ""
    while True :



        tm = tm + s.recv(len_to_recv)
        #len_to_recv = len_to_recv - len(tm)
        #print "len ma " ,len_to_recv
        if len_to_recv - len(tm)<= 0 :
            print tm
            #print len(tm)
            print "\nEND.\n"
            if (tm[1]  == "C") and (tm[2] == "L") and (tm[3]== "O"):
                exit()
            break


    # Receive no more than 1024 bytes
s.close()


