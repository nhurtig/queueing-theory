import os
import pandas as pd
import matplotlib.pyplot as plt

RESULTS = "../soap/results"

for f in sorted(os.listdir(RESULTS)):
    df = pd.read_csv(f"{RESULTS}/{f}")
    df['Turnaround'] = df.FinishTime - df.ArrivalTime
    plt.plot(df.Turnaround)
    plt.savefig(f.replace("csv", "png"))
    plt.clf()
    print(f"{f}: mean turnaround {df.Turnaround.mean()}")
    for s in range(1, 4):
        print(f"{f}, s={s}: {df[df.ServiceTime == s].Turnaround.mean()}")
