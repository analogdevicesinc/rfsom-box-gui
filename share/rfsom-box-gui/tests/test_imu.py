#!/usr/bin/python

import iio
import time
import sys 

def main(): 
	ctx=iio.LocalContext()
	dev=ctx.find_device('adis16460')

	ch=dev.find_channel(sys.argv[1])
	sign=sys.argv[2][0]
	thresh=float(sys.argv[2][1:])
	start=time.time();
	while time.time()-start<10:
		raw=float(ch.attrs['raw'].value)
		scale=float(ch.attrs['scale'].value)
		val=str(raw*scale);
		toEval=val+sign+str(thresh)
#		print(toEval)
		if(eval(toEval)):
			exit(0)
			break;
		time.sleep(.05)
	exit(1)
try:
	timeout=int(sys.argv[3])
except IndexError:
	timeout=10
main()

