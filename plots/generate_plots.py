import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("results/processed/all_results.csv")
for bm in df.benchmark.unique():
    sub = df[df.benchmark == bm]
    xcol, ycol = sub.columns[0], sub.columns[1]
    plt.figure()
    plt.plot(sub[xcol], sub[ycol], marker='o')
    plt.xlabel(xcol)
    plt.ylabel(ycol)
    plt.title(f"{bm} performance")
    plt.savefig(f"plots/{bm}.png")
    plt.close()