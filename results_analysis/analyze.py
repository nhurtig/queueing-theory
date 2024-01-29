import os
import pandas as pd
import matplotlib.pyplot as plt

RESULTS = "../soap/results/log"

for f in sorted(os.listdir(RESULTS)):
    mypath = f"{RESULTS}/{f}"
    if not os.path.isfile(mypath):
        continue
    df = pd.read_csv(mypath)
    df['Turnaround'] = df.FinishTime - df.ArrivalTime
    # plt.plot(df.Turnaround)
    # plt.savefig(f.replace("csv", "png"))
    # plt.clf()
    print(f"{f}: mean turnaround {df.Turnaround.mean()}")
    # for s in range(1, 4):
    #     print(f"{f}, s={s}: {df[df.ServiceTime == s].Turnaround.mean()}")
