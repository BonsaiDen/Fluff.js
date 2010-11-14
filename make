#!/usr/bin/python
import os
import sys

options = '-Wall -Wextra -pedantic '
options += '-lefence ' if '-f' in sys.argv else ''
options += '-O2 -s -ffast-math -momit-leaf-frame-pointer ' if '-r' in sys.argv else ''
options += '-g ' if '-d' in sys.argv else ''
options += '-pg ' if '-p' in sys.argv else ''

os.system('g++ src/fluff.cc %s -o fluff -lv8 -lsfml-system -lsfml-window' % options)

