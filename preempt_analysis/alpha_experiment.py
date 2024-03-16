import numpy as np
import os
import matplotlib.pyplot as plt
import pandas as pd

directory = '../preempt/results/alpha_experiment/'

# Assuming the format of the filenames is 'w_x_y_z.csv'
def extract_coordinates(filename):
    parts = filename.split('_')
    w = float(parts[0])
    x = float(parts[1])
    y = float(parts[2])
    z = float(parts[3])
    return w, x, y, z

# Get list of CSV files
files = os.listdir(directory)
files = [f for f in files if f.endswith('.csv')]

# Get coordinates and sort
coordinates = [extract_coordinates(f) for f in files]
coordinates.sort()

# Determine array shape
ses = []
maxes = []
sizes = []
for i in range(4):
    s = sorted(set(coord[i] for coord in coordinates))
    maxes.append(max(s))
    sizes.append(len(s))
    ses.append(s)

max_w, max_x, max_y, max_z = maxes

# Initialize numpy array
array = np.empty(sizes, dtype=object)

# Fill numpy array
for w, d1 in enumerate(ses[0]):
    for x, d2 in enumerate(ses[1]):
        for y, d3 in enumerate(ses[2]):
            for z, d4 in enumerate(ses[3]):
                array[w, x, y, z] = f"{directory}{d1}_{d2}_{d3}_{d4}_.csv"

def get_ET(fname):
    fname = fname.replace(".0_", "_")
    df = pd.read_csv(fname)
    # if (sum(df["FinishTime"] < df["ArrivalTime"]) > 0):
    if (sum(df["ServiceTime"] <= 0) > 0):
        print(fname)
    val = (df["FinishTime"] - df["ArrivalTime"]).mean()
    dat = "_".join(fname.split("_")[-3:-1])
    return val
    # return f"{val:.2f}_{dat}"

ETs = np.vectorize(get_ET)(array)
# print(array)
print(ETs)
print(ETs.shape)
# print(ETs.mean(axis=0))
# print(ETs.mean(axis=0).shape)

# mything = ETs.mean(axis=0)[0,:,:]
names = array[0,0,:,:]
# print(names)
# print(mything)
print(ETs[0,0,:,:])

np.save('alpha_experiment', ETs)

