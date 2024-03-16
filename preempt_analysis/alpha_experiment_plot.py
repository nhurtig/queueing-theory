import numpy as np
import matplotlib.pyplot as plt

ETs = np.load("alpha_experiment.npy")

plt.imshow(ETs[0,0,:,:], cmap='viridis')
plt.colorbar()  # Add color bar indicating the scale
plt.title('E[T]')
plt.xlabel('alpha')
plt.ylabel('rho')
plt.savefig('fig.png')
