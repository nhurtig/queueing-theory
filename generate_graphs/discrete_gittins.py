import matplotlib.pyplot as plt
import numpy as np

y = [20/17,40/17,60/17]
p = [8/10,1/2/10,3/2/10]
p = np.array(p).astype(float)
p /= sum(p)
samples = 1000
n = len(y)

def gittins_expr(istar, j, a, Pprime):
    num = (1/Pprime)*sum([p[i] for i in range(istar,istar+j+1)])
    print(f"num={num}")
    denom = sum([p[i]*y[i] for i in range(istar,istar+j)])
    denom += y[istar+j]*sum([p[i] for i in range(istar+j,n)])
    denom /= Pprime
    denom -= a
    print(f"denom={denom}")
    return denom/num

def G(a):
    possible_gittins = []
    istar = 0
    yistar = y[istar]
    while a >= yistar:
        istar += 1
        yistar = y[istar]
    Pprime = sum([p[i] for i in range(istar,n)])
    for j in range(n-istar):
        print(f"j={j}")
        possible_gittins.append(gittins_expr(istar, j, a, Pprime))
    return min(possible_gittins)

# print(G(1.5))

A = np.linspace(0,max(y),samples)[:-1]
Y = np.vectorize(G)(A)

plt.figure(figsize=(4,3))
plt.plot(A,Y, color="#32006e")
plt.ylim(0,3)
# plt.title(f"y={y},p={p.round(2)}")
plt.xlabel("Job age $a$")
plt.ylabel("Gittins index $r(a)$")
pts = y[:-1]
pts.append(0)
yGittins = np.vectorize(G)(pts)
plt.scatter(pts,yGittins, color="#32006e")
# for i in range(len(pts)):
#     plt.text(pts[i], yGittins[i]+0.3, f'({pts[i]}, {yGittins[i]:.2f})')
plt.tight_layout()
plt.savefig('discrete_gittins.pdf')
