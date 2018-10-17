#!/usr/bin/python
import subprocess
for r in range(0,32,3):
	for g in range(0,64,3):
		for b in range(0,32,3):
			cmd="python /usr/local/share/rfsom-box-gui/create_fb_data.py " + str(r) + " " + str(g) + " " + str(b)
			print(cmd)
			outfile = open("/dev/fb0","w")
			process = subprocess.call(cmd.split(),stdout=outfile)
			outfile.close()




