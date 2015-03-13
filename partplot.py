import matplotlib.pyplot as plt
import numpy as np
import csv
import math
from mpl_toolkits.mplot3d import Axes3D

x=[]
y=[]
z=[]
r=[]
vx=[]
vy=[]
vz=[]
t=[]
i=0;
with open('particle.csv', 'rt') as f:
    reader = csv.reader(f)
    for row in reader:
        i = i+1
        if i == 1:
            continue
        t.append(float(row[0]))
        x.append(float(row[1]))
        y.append(float(row[2]))
        z.append(float(row[3]))
		
#fig = plt.figure()
#print square(x) + square(y)
r = np.sqrt(list(np.array(x)**2+np.array(y)**2))

plt.plot(r,z)
#ax = fig.add_subplot(111, projection='3d')
#ax.plot(x,y,z)
plt.show()
