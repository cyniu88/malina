#!/usr/bin/python
import subprocess
import sys

print(sys.argv[1])
komenda2 = "omxplayer --align center  --no-ghost-box   -b -o local "
komenda2 += sys.argv[1] + " < /mnt/ramdisk/cmd"
print (komenda2)
subprocess.Popen(komenda2,shell=True)
