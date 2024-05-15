import pandas as pd
import numpy as np
import os

def main():
    folder = "results"
    for experiment in os.listdir(folder):
        print(f"analyzing {experiment}")
        analyze_experiment(experiment, folder + os.sep + experiment, get_N(experiment))

def analyze_experiment(name, path, N):
    files = os.listdir(path)
    coordinates = [extract_coordinates(f) for f in files]

    coordinates.sort()

    # Determine array shape
    ses = []
    sizes = []
    for i in range(4):
        s = sorted(set(coord[i] for coord in coordinates))
        sizes.append(len(s))
        ses.append(s)

    # Initialize numpy array
    array = np.empty(sizes, dtype=object)

    # Fill numpy array
    for w, d1 in enumerate(ses[0]):
        for x, d2 in enumerate(ses[1]):
            for y, d3 in enumerate(ses[2]):
                for z, d4 in enumerate(ses[3]):
                    array[w, x, y, z] = f"{path}{os.sep}{d1}_{d2}_{d3}_{d4}_.csv"

    print(f"analyzing {array.size} runs...")
    ETs = np.vectorize(curry2(get_ET)(N))(array)

    np.save(f"analyzed/{name}", ETs)

# Assuming the format of the filenames is 'w_x_y_z.csv'
def extract_coordinates(filename):
    parts = filename.split('_')
    w = float(parts[0])
    x = float(parts[1])
    y = float(parts[2])
    z = float(parts[3])
    return w, x, y, z

def curry2(fn2):
    return lambda a1: lambda a2: fn2(a1, a2)

def get_ET(N, fname):
    fname = fname.replace(".0_", "_")
    df = pd.read_csv(fname)
    for float_col in ["FinishTime", "ServiceTime", "ArrivalTime"]:
        df[float_col] = df[float_col].astype('float')

    # drop filler
    df = df[df["ArrivalTime"] < N/2]
    if df["FinishTime"].max() > N: # something didn't finish
        return np.inf # likely an unstable queue

    val = (df["FinishTime"] - df["ArrivalTime"]).mean()
    return val

def get_N(experiment):
    if "explore" in experiment and "compare" not in experiment:
        return 10000
    elif "compare" in experiment:
        return 10000
    else:
        print(f"unrecognized experiment {experiment}")
        return 0

if __name__ == "__main__":
    main()
