import numpy as np
import scipy as sc
import matplotlib.pyplot as plt

x, y1, y2, y3, y4 = np.loadtxt("/Users/victor/projects/cs190i/Problem-1/fileP1.csv", 
                               unpack = True,
                               delimiter = ",")

plt.plot(x, y1, color = 'blue', label = 'Monte Carlo with Uniform PDF', linewidth = .5)
plt.plot(x, y2, color = 'orange', label = 'Monte Carlo Estimation with PDF proprtional to y = pi/2 - x', linewidth = .5)

plt.axhline(y = 1, color = 'green', linestyle = 'solid', linewidth = 2, label = '| = 1')
plt.plot(x, y4, color = 'purple', label = 'Monte Carlo Estimation with PDF proportional to y = cos(x) using rejection method', linewidth = 1)
plt.plot(x, y3, color = 'red', label = 'Monte Carlo Estimation with PDF proportional to y = cos(x) using inversion method', linewidth = .8)

plt.xlim(0, 8000)
plt.ylim(0, 2)



plt.title("Monte Carlo Estimation")
plt.ylabel("Estimated <I> Value")
plt.xlabel("Number of Samples")

plt.legend(loc = "upper right", fontsize = "small")
plt.show()