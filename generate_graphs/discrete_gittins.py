import matplotlib.pyplot as plt
import numpy as np

y = [1,2,3]
p = [5,1,1]
p = np.array(p).astype(float)
p /= sum(p)
samples = 1000
n = len(y)

def gittins_expr(istar, j, a, Pprime):
    num = (1/Pprime)*sum([p[i] for i in range(istar,istar+j+1)])
    denom = sum([p[i]*y[i] for i in range(istar,istar+j)])
    denom += y[istar+j]*sum([p[i] for i in range(istar+j,n)])
    denom /= Pprime
    denom -= a
    return num/denom

def G(a):
    possible_gittins = []
    istar = 0
    yistar = y[istar]
    while a >= yistar:
        istar += 1
        yistar = y[istar]
    Pprime = sum([p[i] for i in range(istar,n)])
    for j in range(n-istar):
        possible_gittins.append(gittins_expr(istar, j, a, Pprime))
    return max(possible_gittins)

A = np.linspace(0,max(y),samples)[:-1]
Y = np.vectorize(G)(A)

plt.plot(A,Y)
plt.ylim(0,10)
plt.title(f"y={y},p={p.round(2)}")
plt.xlabel("a")
plt.ylabel("G(a)")
pts = y[:-1]
pts.append(0)
yGittins = np.vectorize(G)(pts)
plt.scatter(pts,yGittins)
for i in range(len(pts)):
    plt.text(pts[i], yGittins[i]+0.3, f'({pts[i]}, {yGittins[i]:.2f})')
plt.savefig('discrete_gittins.png')
