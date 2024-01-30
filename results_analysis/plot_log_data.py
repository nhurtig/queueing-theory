import json
import numpy as np
import matplotlib.pyplot as plt

def lists_to_quartiles(dict_lists):
    dict_lists.pop("0", None)
    domain = sorted([1 - pow(10,-float(x)) for x in dict_lists.keys()])
    names = sorted(dict_lists.keys(), key=(lambda x: 1 - pow(10, -float(x))))
    qs = [np.quantile(dict_lists[x], [0.25,0.5,0.75]) for x in names]
    qs = np.transpose(np.array(qs))
    first = qs[0]
    median = qs[1]
    third = qs[2]
    return domain, names, first, median, third

f = open("gittins_lists.json", "r")
gittins_lists = json.load(f)
f.close()

f = open("fcfs_lists.json", "r")
fcfs_lists = json.load(f)
f.close()

g_domain, g_names, g_first, g_median, g_third = lists_to_quartiles(gittins_lists)
f_domain, f_names, f_first, f_median, f_third = lists_to_quartiles(fcfs_lists)

f_names = [float(x) for x in f_names]
g_names = [float(x) for x in g_names]


g_color = "#8337be"
f_color = "#444444"
plt.plot(f_domain, f_median, color=f_color)
# plt.plot(g_domain, g_median, color=g_color)
# plt.legend(["FCFS", "Gittins"])
plt.plot(f_domain, f_first, '--', color=f_color)
# plt.plot(g_domain, g_first, '--', color=g_color)
plt.plot(f_domain, f_third, '--', color=f_color)
# plt.plot(g_domain, g_third, '--', color=g_color)

plt.xlabel("Load")
plt.ylabel("Average waiting time (seconds)")

# print(g_median)
# print(g_domain)
# print(f_median)

plt.savefig("log_results.png", dpi=1200)
plt.cla()

f_domain = np.array(f_names)
g_domain = np.array(g_names)

plt.plot(f_domain, f_median, color=f_color)
# plt.plot(g_domain, g_median, color=g_color)
# plt.legend(["FCFS", "Gittins"])
plt.plot(f_domain, f_first, '--', color=f_color)
# plt.plot(g_domain, g_first, '--', color=g_color)
plt.plot(f_domain, f_third, '--', color=f_color)
# plt.plot(g_domain, g_third, '--', color=g_color)


plt.ylabel("Average waiting time (seconds)")

# plt.xscale('log')

def inverse(name):
    # y = 1 - 10^{-x}
    # 1 - y = 10^{-x}
    # log_10{1-y} = - x
    return -np.log10(1-name)

# # Adjust x-axis ticks and tick labels
minor_ticks = [inverse(x) for x in np.arange(0.1,0.9,0.1)] + [inverse(x) for x in np.arange(0.91,0.99,0.01)] + [inverse(x) for x in np.arange(0.991,0.999,0.001)]
minor_tick_names = ["" for x in minor_ticks]
major_ticks_orig = [0,0.9,0.99,0.999]
major_ticks = [inverse(x) for x in major_ticks_orig]
major_tick_names = [str(x) for x in major_ticks_orig]

plt.xticks(minor_ticks + major_ticks, minor_tick_names + major_tick_names)

# plt.gca().xaxis.set_major_formatter(lambda x, pos: 1 - 10 ** (-x))

plt.xlabel("Load (logarithmic)")
plt.savefig("log_results_logx.png", dpi=1200)

plt.cla()

plt.plot(g_domain, g_median, color=g_color)
plt.legend(["Gittins"])
plt.plot(g_domain, g_first, '--', color=g_color)
plt.plot(g_domain, g_third, '--', color=g_color)


plt.ylabel("Average waiting time (seconds)")

# plt.xscale('log')

def inverse(name):
    # y = 1 - 10^{-x}
    # 1 - y = 10^{-x}
    # log_10{1-y} = - x
    return -np.log10(1-name)

# # Adjust x-axis ticks and tick labels
minor_ticks = [inverse(x) for x in np.arange(0.1,0.9,0.1)] + [inverse(x) for x in np.arange(0.91,0.99,0.01)] + [inverse(x) for x in np.arange(0.991,0.999,0.001)]
minor_tick_names = ["" for x in minor_ticks]
major_ticks_orig = [0,0.9,0.99,0.999]
major_ticks = [inverse(x) for x in major_ticks_orig]
major_tick_names = [str(x) for x in major_ticks_orig]

plt.xticks(minor_ticks + major_ticks, minor_tick_names + major_tick_names)

# plt.gca().xaxis.set_major_formatter(lambda x, pos: 1 - 10 ** (-x))

plt.xlabel("Load (logarithmic)")
plt.savefig("log_results_logx_gittins.png", dpi=1200)

plt.cla()

compare_color="#aa8030"

# [np.quantile(np.array(fcfs_lists[i])/np.array(gittins_lists[i]),0.5) for i in gittins_lists.keys()]

mykeys = [str(x) for x in sorted(g_names)]
mykeys[mykeys.index("1.0")] = "1"
mykeys[mykeys.index("2.0")] = "2"

plt.plot(g_domain, [np.quantile(np.array(fcfs_lists[i])/np.array(gittins_lists[i]),0.5) for i in mykeys], color=compare_color)
plt.plot(g_domain, [np.quantile(np.array(fcfs_lists[i])/np.array(gittins_lists[i]),0.25) for i in mykeys], '--', color=compare_color)
plt.plot(g_domain, [np.quantile(np.array(fcfs_lists[i])/np.array(gittins_lists[i]),0.75) for i in mykeys], '--', color=compare_color)


plt.ylabel("Ratio in average waiting time")

# # Adjust x-axis ticks and tick labels
minor_ticks = [inverse(x) for x in np.arange(0.1,0.9,0.1)] + [inverse(x) for x in np.arange(0.91,0.99,0.01)] + [inverse(x) for x in np.arange(0.991,0.999,0.001)]
minor_tick_names = ["" for x in minor_ticks]
major_ticks_orig = [0,0.9,0.99,0.999]
major_ticks = [inverse(x) for x in major_ticks_orig]
major_tick_names = [str(x) for x in major_ticks_orig]

plt.xticks(minor_ticks + major_ticks, minor_tick_names + major_tick_names)

# plt.gca().xaxis.set_major_formatter(lambda x, pos: 1 - 10 ** (-x))

plt.xlabel("Load (logarithmic)")
plt.savefig("log_results_logx_compare.png", dpi=1200)
