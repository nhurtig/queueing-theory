import numpy as np
import matplotlib.pyplot as plt
import matplotlib.colors as mcolors
import matplotlib.patches as patches

figsize = (6,4.5)
primary_color = '#32006e'
primary_light = '#D9CEEE'
secondary_color = '#e93cac'

def main():
    explore_plot("analyzed/explore_gorg.npy", "plots/explore_gorg", True)
    explore_plot("analyzed/explore_ours.npy", "plots/explore_ours", False)
    print("thru exp")

    dat_ratio = compare("analyzed/compare_ours.npy", "analyzed/compare_gorg.npy")

    
    compare_plot(dat_ratio, "plots/compare")
    box_whisker_plot(dat_ratio, "plots/box_whisker")

def explore_plot(dat_name, out, is_gorg):
    dat = np.load(dat_name)
    rhos = [0.7, 0.75, 0.8, 0.85, 0.87, 0.89]

    dat = np.median(dat, axis=0)[0] # dims are hyperparam, rho

    hyperparams = None
    if is_gorg:
        hyperparamlogs = np.arange(-2, 3.02, 0.1)
        hyperparams = np.power(10, hyperparamlogs)
    else:
        kappastep = 0.02
        hyperparams = np.arange(-1+kappastep, 1, kappastep)
        best_kappa_indices = np.argmin(dat[:,rhos.index(0.85)])
        print(f"best kappa is {hyperparams[best_kappa_indices]}")

    # set up colors
    # Define the starting and ending colors
    start_color = primary_color
    end_color = secondary_color

    # Number of intermediate colors
    num_colors = len(rhos)-2

    # Convert colors to RGB
    start_rgb = mcolors.hex2color(start_color)
    end_rgb = mcolors.hex2color(end_color)

    # Convert RGB tuples to numpy arrays
    start_rgb = np.array(start_rgb)
    end_rgb = np.array(end_rgb)

    # Create a gradient of colors
    colors = [mcolors.rgb2hex(start_rgb + (i+1) * (end_rgb - start_rgb) / (num_colors + 1)) for i in range(num_colors)]

    # Insert the start and end colors into the list
    colors.insert(0, start_color)
    colors.append(end_color)

    plt.figure(figsize=figsize)
    # for each rho...
    for i in range(dat.shape[1]):
        plt.plot(hyperparams, dat[:,i], color=colors[i])

    plt.legend(rhos, title=r'Load $\rho$')
    if is_gorg:
        plt.xlabel(r'Preemption gap $\alpha$')
        plt.xscale('log')
        plt.xlim(10**-2,10**3)
    else:
        plt.xlabel(r'Preemption tendency $\kappa$')
        plt.xlim(-1,1)
    
    plt.yscale('log')
    plt.ylim(1, 100)

    plt.tight_layout()
    plt.savefig(out+".pdf")
    plt.savefig(out+".png", dpi=800)

def compare(base_name, comp_name):
    dat_base = np.load(base_name)
    dat_comp = np.load(comp_name)

    dat_base = dat_base[:,0,:,0] # kappa, rho don't change
    dat_comp = dat_comp[:,0,:,0] # rho doesn't change

    dat_ratio = dat_comp / dat_base # dims are run #, alpha
    if np.isnan(dat_ratio).sum() > 0:
        print("WARNING: found unstable queue in compared data. Results are likely invalid!")
        dat_ratio[np.isnan(dat_ratio)] = 1.0
    
    med_min = np.min(np.median(dat_ratio, axis=0))
    mean_min = np.min(np.mean(dat_ratio, axis=0))
    wins_min = np.min(np.mean(dat_ratio > 1, axis=0))

    print("Against best alpha for each:")
    print(f"    median gap {med_min}")
    print(f"    mean gap   {mean_min}")
    print(f"    won prop   {wins_min}")
    print(f"    lost seeds {(1-wins_min)*dat_ratio.shape[0]} of {dat_ratio.shape[0]}")
    print(f"    worst diff {dat_ratio.min()}")
    
    return dat_ratio

def compare_plot(dat_ratio, out):
    alphalogs = np.arange(-2, 3.02, 0.1)
    alphas = np.power(10, alphalogs)

    plt.figure(figsize=figsize)
    # plot median ratio
    plt.plot(alphas, np.median(dat_ratio, axis=0), color=primary_color)
    
    plt.xscale('log')
    plt.xlim(10**-2,10**3)

    plt.xlabel(r'Preemption gap $\alpha$')
    plt.ylabel(r'Görg 1986: $\mathbf{E}[T]$ over $\kappa=0.58$')

    plt.yscale('log')
    plt.ylim(1)

    plt.tight_layout()
    plt.savefig(out+".pdf")
    plt.savefig(out+".png", dpi=800)

def box_whisker_plot(dat_ratio, out):
    alphalogs = np.arange(-2, 3.02, 0.1)
    alphas = np.power(10, alphalogs)

    plt.figure(figsize=figsize)

    best_alpha_index = np.argmin(np.median(dat_ratio, axis=0))

    bplot = plt.boxplot(dat_ratio[:,best_alpha_index], notch=True, vert=False, labels=[""])
    for box in bplot['boxes']:
        box.set_color(primary_color)
    for patch in bplot['whiskers']:
        patch.set_color(primary_color)
    for patch in bplot['fliers']:
        patch.set_color('r')
    for patch in bplot['caps']:
        patch.set_color(primary_color)
    for patch in bplot['means']:
        patch.set_color('r')
    for patch in bplot['medians']:
        patch.set_color(primary_color)

    for box, color in zip(bplot['boxes'], [primary_light]):
        box_x = []
        box_y = []
        for j in range(len(box.get_xdata())):
            box_x.append(box.get_xdata()[j])
            box_y.append(box.get_ydata()[j])
        box_coords = list(zip(box_x, box_y))
        box_polygon = patches.Polygon(box_coords, closed=True, facecolor=color)
        plt.gca().add_patch(box_polygon)

    plt.tick_params(
        axis='y',          # changes apply to the x-axis
        which='both',      # both major and minor ticks are affected
        left=False,      # ticks along the bottom edge are off
        right=False,         # ticks along the top edge are off
        labelleft=False) # labels along the bottom edge are off

    plt.xlabel(r'Görg 1986 $\alpha=???$: $\mathbf{E}[T]$ over $\kappa=0.58$'.replace("???", f"{alphas[best_alpha_index]:.2f}"))

    plt.tight_layout()
    plt.savefig(out+".pdf")
    plt.savefig(out+".png", dpi=800)


if __name__ == "__main__":
    main()
