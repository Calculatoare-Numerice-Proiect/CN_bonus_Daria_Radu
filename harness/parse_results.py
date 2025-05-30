# harness/parse_results.py
import pandas as pd

# list of (benchmark name, raw csv path, its two column names)
files = [
    ("int",  "../results/raw/int.csv",  ["ops",     "time_s"]),
    ("flt",  "../results/raw/flt.csv",  ["ops",     "time_s"]),
    ("lat",  "../results/raw/lat.csv",  ["size_mb", "time_ns"]),
    ("bw",   "../results/raw/bw.csv",   ["size_mb", "time_ns"]),
    ("sort", "../results/raw/sort.csv", ["n",       "time_s"]),
    ("sql",  "../results/raw/sql.csv",  ["rows",    "time_s"]),
    ("ml",   "../results/raw/ml.csv",   ["samples", "time_s"]),
    ("cache","../results/raw/cache_latency.csv",       ["size_kb",     "latency_ns"]),
    ("thread_scalability","../results/raw/thread_scalability.csv", ["threads",     "time_s"]),  # or ["threads,ops","time_s"] if you want to plot ops vs time

]

df_list = []
for name, path, cols in files:
    df = pd.read_csv(path)
    # rename its two columns to a standard x,y
    df = df.rename(columns={cols[0]: "x", cols[1]: "y"})
    df["benchmark"] = name
    df = df[["benchmark", "x", "y"]]
    df_list.append(df)

# concatenate and write
all_df = pd.concat(df_list, ignore_index=True)
all_df.to_csv("../results/processed/all_results.csv", index=False)
print("Merged tidy results to results/processed/all_results.csv")
