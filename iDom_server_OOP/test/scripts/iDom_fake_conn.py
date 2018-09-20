#!/usr/bin/python
import sys
import socket
#import androidhelper
import time
#from time import gmtime, strftime
#from sys import stdout
counter = 0
last_counter = 0
BufSize =332768

def RSHash ():
    str_time = time.strftime("%M%H%w")
    #print str

    b = 378551
    a = 63689
    hash_t = 0
    
    for x in str_time:
        hash_t = hash_t * a + int(x)+48
        a    = a * b
        
    return hash_t & 0x7FFFFFFF
	
main_loop =False
my_bytes = ""
host = "192.168.1.144"
port = 8833

#droid = androidhelper.Android()
#print("parametry: %s" % sys.argv)
# create a socket object
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# get local machine name

m_sleep = 0

if len(sys.argv) >= 2 :
	m_sleep = sys.argv[1]

#connection to hostname on the port.

s.connect((host, port))

s.send(" ");
time.sleep(float(m_sleep))
tm = s.recv(BufSize)
print "pierwsze odebranie", tm
time.sleep(float(m_sleep))
s.send(" ");
tm = s.recv(BufSize)
print "pierwsze odebranie", tm
s.send("FAKE CONN");
exit()

#######################################   end program
print RSHash()
tm = s.recv(BufSize)
print "pierwsze odebranie", tm
s.send("OK")
tm = s.recv(BufSize)
print "drugie odebranie", tm

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
	last_couter = counter
        my_bytes = "RS232 send test:"+str(counter)+";"
        print("> %s" % my_bytes.decode('ascii'))
    if my_bytes == "exit" :
        s.send(my_bytes)
        tm = s.recv(BufSize)
        s.send("OK")
        tm = s.recv(BufSize)
        print( tm)
        break
     
    s.send(my_bytes)
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

	    dummy, counter = tm.split(":")
	    #counter = counter[:-1]
	    #print dummy
	    #print last_counter
	    #print  " nie jest rowny "
	    #print  counter

	    if int(last_counter)+2 != int(counter) :
		print last_counter
		print " nie jest rowny "
		print counter
		exit()
	    last_counter = counter
            #print len(tm)
            print "\nEND.\n"
            if (tm[1]  == "C") and (tm[2] == "L") and (tm[3]== "O"):
                exit()
            break
        

    # Receive no more than 1024 bytes
s.close()

