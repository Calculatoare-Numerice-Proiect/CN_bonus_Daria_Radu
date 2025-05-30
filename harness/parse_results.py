import pandas as pd

files = [
    ("int","results/raw/int.csv"),
    ("flt","results/raw/flt.csv"),
    ("lat","results/raw/lat.csv"),
    ("bw","results/raw/bw.csv"),
    ("sort","results/raw/sort.csv"),
    ("sql","results/raw/sql.csv"),
    ("ml","results/raw/ml.csv"),
]

df_list = []
for name, path in files:
    df = pd.read_csv(path)
    df["benchmark"] = name
    df_list.append(df)
all_df = pd.concat(df_list, ignore_index=True)
all_df.to_csv("results/processed/all_results.csv", index=False)
print("Merged into results/processed/all_results.csv")