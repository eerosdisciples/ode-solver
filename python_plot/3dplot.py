import matplotlib.pyplot as plt
import numpy as np
import csv
from mpl_toolkits.mplot3d import Axes3D

results = np.genfromtxt('../particle.csv', delimiter=',', skip_header=1)

x = results[:,1]
y = results[:,2]
z = results[:,3]

fig = plt.figure()
ax = fig.gca(projection='3d')
ax.plot(x,y,z)

ax.set_xlabel('x')
ax.set_ylabel('y')
ax.set_zlabel('z')

ax.invert_xaxis()
ax.invert_yaxis()

plt.show()
