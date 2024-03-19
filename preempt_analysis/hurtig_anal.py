import numpy as np
import os
import pandas as pd

N = 10000

file = 'compare_rho0.7'

directory = f'../preempt/results/{file}/'


def get_ET(fname):
    fname = fname.replace(".0_", "_")
    df = pd.read_csv(fname)
    for float_col in ["FinishTime", "ServiceTime", "ArrivalTime"]:
        df[float_col] = df[float_col].astype('float')

    # drop filler
    df = df[df["ArrivalTime"] < N/2]
    if df["FinishTime"].max() > N: # something didn't finish
        print(fname)
        return np.inf # unstable queue

    # if (sum(df["FinishTime"] < df["ArrivalTime"]) > 0):
    if (sum(df["ServiceTime"] <= 0) > 0):
        print(fname) # error check
        raise IndexError(fname)
    val = (df["FinishTime"] - df["ArrivalTime"]).mean()
    dat = "_".join(fname.split("_")[-3:-1])
    return val
    # return f"{val:.2f}_{dat}"

time_to_beat = 2.9218594123819557
hurtig_et = get_ET(directory + "0_0.1_hurtig_0.75_.csv")
# not preempting gets 3.95
# static gets 3.10445 (worse than alpha=0!!!)
if hurtig_et < time_to_beat:
    print("YOU DID IT!!!")
    print(hurtig_et, time_to_beat)
else:
    print(f"Nope! You got {hurtig_et:.5f}, wanted to beat {time_to_beat:.5f}")

# E[T] for various values of alpha, from swapping too much to swapping too little:
'''
[[3.10325309]
 [3.10307733]
 [3.10055241]
 [3.10032562]
 [3.09958061]
 [3.09829257]
 [3.09630883]
 [3.0845444 ]
 [3.08237237]
 [3.07639792]
 [3.07050259]
 [3.05758968]
 [3.05017266]
 [3.03002434]
 [3.01576384]
 [2.98530031]
 [2.96584383]
 [2.94526023]
 [2.92940293]
 [2.92428866]
 [2.92185941]
 [2.92939797]
 [2.94660676]
 [2.97903625]
 [3.02486337]
 [3.09882733]
 [3.21830251]
 [3.38472565]
 [3.50419155]
 [3.66554464]
 [3.78735603]
 [3.8815756 ]
 [3.93508557]
 [3.95291951]
 [3.95291951]
 [3.95291951]
 [3.95291951]
 [3.95291951]
 [3.95291951]
 [3.95291951]
 [3.95291951]
 [3.95291951]
 [3.95291951]
 [3.95291951]
 [3.95291951]
 [3.95291951]
 [3.95291951]
 [3.95291951]
 [3.95291951]
 [3.95291951]
 [3.95291951]]
 '''