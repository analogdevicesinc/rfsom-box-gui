#!/usr/bin/python
import sys

def rail(val, min, max):
	if val<min:
		return min
	if val>max:
		return max
	return val

x_res = 160
y_res = 128

if(len(sys.argv)!=4):
	sys.stderr.write("Usage: create_fb_data <r> <g> <b>")
	exit()
r=int(sys.argv[1])
g=int(sys.argv[2])
b=int(sys.argv[3])

# bit manipulation
r=rail(r,0,(1<<5)-1) << 11 
g=rail(g,0,(1<<6)-1) << 5
b=rail(b,0,(1<<5)-1) << 0
rgb=r | g | b
c1= rgb >> 8
c2= rgb & 0xff

# write data
for i in range(x_res*y_res):
	sys.stdout.write(chr(c2))
	sys.stdout.write(chr(c1))
sys.stdout.flush()
