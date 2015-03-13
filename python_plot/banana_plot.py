import matplotlib.pyplot as plt
import numpy as np
import csv
import matplotlib.cbook as cbook

contour = np.genfromtxt('iter.wall_2d', delimiter='', skip_header=5)

r_contour = contour[:,0]
z_contour = contour[:,1]

fig = plt.figure()

results = np.genfromtxt('../particle.csv', delimiter=',', skip_header=1)

x = results[:,1]
y = results[:,2]
z = results[:,3]

r = np.sqrt(x**2 + y**2)

plt.plot(r_contour,z_contour, color='r')
plt.plot(r,z)

plt.xlabel('r')
plt.ylabel('z')

plt.grid()
plt.show()

