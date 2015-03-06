import csv
import numpy as np
import matplotlib.pyplot as plt

t=[]
x=[]
y=[]
i=0

with open('Output.csv', 'rb') as f:
    reader = csv.reader(f)
    for row in reader:
        i = i+1
        if i == 1:
            continue
        t.append(float(row[0]))
        x.append(float(row[1]))
        y.append(float(row[2]))

plt.plot(t,x,t,y)
plt.xlim([0,20])
plt.show()
