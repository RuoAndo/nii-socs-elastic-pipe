import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import sys

args = sys.argv
 
d = pd.read_csv(args[1],encoding = 'UTF8')
print(d)

x = np.array(d["source_ip"])
y = np.array(d["count"])

fig = plt.figure()
#ax = Axes3D(fig)
ax = fig.add_subplot(1, 1, 1)

ax.barh(x, y)
ax.set_ylabel("source_ip")
ax.set_xlabel("count")

#ax.axes.yaxis.set_visible(False)
ax.axes.yaxis.set_ticks([])
plt.show()

