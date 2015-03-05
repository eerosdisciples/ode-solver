import matplotlib.pyplot as plt
import numpy as np
file=open("Output.csv","r")

sepFile=file.read().split('\n') #Separates file after newline #/#n  
file.close()


t=[]
x=[]

y=[]
tot=[]

for plotPair in sepFile: # PLotpair ar ett par i sepFile
	randz=plotPair.split('\t') # Vi delar upp det paret i tva delar,
	t.append(float(randz[0]))
	tot.append(str(randz[1]))
    
for plotPair in tot: # delar upp tot i 2 arrays
    	zandflag=plotPair.split()# Space som default 
    	x.append(float(zandflag[0]))
    	y.append(float(zandflag[1]))


print 'The coordinates are {0} and {1}'.format(r[0:1],z[0:1])

plt.plot(x,y);

plt.show()
		
        # ITER 2D wall contour
        # number of data points
    #    54

        # r-coordinate z-coordinate flag:
	

