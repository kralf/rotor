#!/usr/bin/python

import rotorc
import sys

while True:
  time = rotorc.seconds()
  rotorc.Thread.sleep( 10000 )
  print rotorc.seconds() - time
