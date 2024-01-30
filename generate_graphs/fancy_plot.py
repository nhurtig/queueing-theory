import matplotlib.pyplot as plt
import numpy as np

# y = [10/17,20/17,30/17]
y = [20/27, 40/27, 60/27]
p = [80/100,5/100,15/100]
p = np.array(p).astype(float)
p /= sum(p)
samples = 1000
n = len(y)

def gittins_expr_slowdown(istar, j, a, Pprime):
    num = (1/Pprime)*sum([p[i]*y[i] for i in range(istar,istar+j+1)])
    print(f"num={num}")
    denom = sum([p[i]*y[i] for i in range(istar,istar+j)])
    denom += y[istar+j]*sum([p[i] for i in range(istar+j,n)])
    denom /= Pprime
    denom -= a
    print(f"denom={denom}")
    return denom/num

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

def G_slowdown(a):
    possible_gittins = []
    istar = 0
    yistar = y[istar]
    while a >= yistar:
        istar += 1
        yistar = y[istar]
    Pprime = sum([p[i] for i in range(istar,n)])
    for j in range(n-istar):
        print(f"j={j}")
        possible_gittins.append(gittins_expr_slowdown(istar, j, a, Pprime))
    return min(possible_gittins)

# print(G(1.5))

A = np.linspace(0,max(y),samples)[:-1]
Y = np.vectorize(G)(A)

plt.plot(A,Y,color="#8337be")

A = np.linspace(0,max(y),samples)[:-1]
Y = np.vectorize(G_slowdown)(A)

plt.plot(A,Y,color="#4285f4")
plt.ylim(0,3)
plt.xlabel("Age")
plt.ylabel("Rank")

plt.legend(["Unmodified", "Holding costs"])

pts = y[:-1]
pts.append(0)
yGittins = np.vectorize(G)(pts)
plt.scatter(pts,yGittins,color="#8337be")

pts = y[:-1]
pts.append(0)
yGittins = np.vectorize(G_slowdown)(pts)
plt.scatter(pts,yGittins,color="#4285f4")
# plt.title("Gittins indices for discrete distribution")

plt.savefig('discrete_gittins_compare.png',dpi=1200)
# plt.savefig('discrete_gittins_unmodified.png',dpi=1200)
