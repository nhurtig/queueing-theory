import os
import pandas as pd
import matplotlib.pyplot as plt
import json

RESULTS = "../soap/results/log"

gittins_lists = {}
fcfs_lists = {}

for f in sorted(os.listdir(RESULTS)):
    mypath = f"{RESULTS}/{f}"
    if not os.path.isfile(mypath):
        continue
    df = pd.read_csv(mypath)
    df['Turnaround'] = df.FinishTime - df.ArrivalTime
    _,n,load,i = f.split("_")
    i = i.replace(".csv", "")
    print(n, load, i)
    # plt.plot(df.Turnaround)
    # plt.savefig(f.replace("csv", "png"))
    # plt.clf()
    T = df.Turnaround.mean()
    print(f"{f}: mean turnaround {T}")

    mydict = None
    if 'gittins' in f:
        mydict = gittins_lists
    if 'fcfs' in f:
        mydict = fcfs_lists
    
    if n in mydict:
        mydict[n].append(T)
    else:
        mydict[n] = [T]

f = open("gittins_lists.json", 'w')
json.dump(gittins_lists, f)
f.close()

f = open("fcfs_lists.json", 'w')
json.dump(fcfs_lists, f)
f.close()
