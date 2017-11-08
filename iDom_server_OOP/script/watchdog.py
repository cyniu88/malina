import os
import subprocess

def is_running(pid):
    if os.path.isdir('/proc/{}'.format(pid)):
        return True
    return False

F = open("/mnt/ramdisk/pid-iDom.txt","r") 
pid = F.read()
print  pid
#print("pid istnieje?")
#print  is_running(pid)
cmd ="screen -S dom  -d -m sudo /home/pi/programowanie/iDom_server_OOP-build-clang-Release/iDom_server_OOP"


if (is_running(pid) == False):
    subprocess.Popen(cmd,shell=True)
    print ("uruchamiam proces")
else:
    print("iDom server pracuje")
