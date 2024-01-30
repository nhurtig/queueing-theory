import os
import pandas as pd
import json
# import matplotlib.pyplot as plt

RESULTS = "../soap/results/gittcomp_nice"

turn_lists = {}
slow_lists = {}
keys = ['1', '5', '20']
subkeys = ['t', 's']
for key in keys:
    turn_lists[key] = {}
    slow_lists[key] = {}
    for subkey in subkeys:
        turn_lists[key][subkey] = {}
        slow_lists[key][subkey] = {}


for f in sorted(os.listdir(RESULTS)):
    mypath = f"{RESULTS}/{f}"
    if not os.path.isfile(mypath):
        continue
    _,k,n,load,i = f.split("_")
    df = pd.read_csv(mypath)
    df['Turnaround'] = df.FinishTime - df.ArrivalTime
    df['Slowdown'] = df.Turnaround * df.ServiceTime
    # if k == "1" and n == "2.9":
    #     print(df)
    i = i.replace(".csv", "")
    # print(n, load, i)
    # if k == "1" and n == "2.1":
    #     plt.plot(df.Turnaround)
    #     plt.savefig(f.replace("csv", "png"))
    #     plt.clf()
    T = df.Turnaround.mean()
    S = df.Slowdown.mean()
    abandoned = -(df.shape[0] - (df.ID.max() - df.ID.min()))
    if k == "1" and n == "2.5":
        print(f"{f}: mean turnaround {T}, slowdown {S}, ab={abandoned}")

    mydict = None
    if 'turn' in f:
        mydict = turn_lists
    if 'slow' in f:
        mydict = slow_lists
    
    mydict = mydict[k]
    
    if n in mydict['t']:
        mydict['t'][n].append(T)
        mydict['s'][n].append(S)
    else:
        mydict['t'][n] = [T]
        mydict['s'][n] = [S]

f = open("slow_lists.json", 'w')
json.dump(slow_lists, f)
f.close()

f = open("turn_lists.json", 'w')
json.dump(turn_lists, f)
f.close()
