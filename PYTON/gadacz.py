
# -*- coding: utf-8 -*-
from gtts import gTTS
from pygame import mixer # Load the required library
import os
import sys
print(sys.argv[1:])
filePath = "/mnt/ramdisk/tts.mp3"
text = sys.argv[1] 
tts = gTTS(text, "pl")
tts.save(filePath)
#mixer.init()
#mixer.music.load("/mnt/ramdisk/tts.mp3")
#mixer.music.play()

os.system('mplayer '+filePath +' &')
