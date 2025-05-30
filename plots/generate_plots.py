import os
import pandas as pd
import matplotlib.pyplot as plt

# 1) Compute paths relative to this script file:
BASE = os.path.dirname(os.path.abspath(__file__))
CSV_PATH = os.path.normpath(os.path.join(BASE, os.pardir, 'results', 'processed', 'all_results.csv'))
OUT_DIR  = BASE  # write PNGs into the 'plots/' folder itself

# 2) Make sure the CSV exists
if not os.path.isfile(CSV_PATH):
    raise FileNotFoundError(f"Could not find data file at {CSV_PATH}")

# 3) Ensure output dir exists (it should, but just in case)
os.makedirs(OUT_DIR, exist_ok=True)

# 4) Load and plot
df = pd.read_csv(CSV_PATH)

for bm in df.benchmark.unique():
    sub = df[df.benchmark == bm]
    xcol, ycol = sub.columns[0], sub.columns[1]

    plt.figure()
    # if you only have one point, use scatter so you actually see something
    if len(sub) == 1:
        plt.scatter(sub[xcol], sub[ycol], s=100)
    else:
        plt.plot(sub[xcol], sub[ycol], marker='o')

    plt.xlabel(xcol)
    plt.ylabel(ycol)
    plt.title(f"{bm} performance")

    out_file = os.path.join(OUT_DIR, f"{bm}.png")
    plt.savefig(out_file)
    plt.close()

print(f"Plots written to {OUT_DIR}")
