#!/usr/bin/env bash
set -e

mkdir -p ../results/raw ../results/processed ../plots

# Compile microbenchmarks
gcc -O3 ../microbenchmarks/integer_throughput.c -o ../microbenchmarks/integer_throughput
gcc -O3 ../microbenchmarks/float_throughput.c -o ../microbenchmarks/float_throughput -lm
gcc -O3 ../microbenchmarks/mem_latency.c -o ../microbenchmarks/mem_latency
gcc -O3 ../microbenchmarks/mem_bandwidth.c -o ../microbenchmarks/mem_bandwidth

# Run microbenchmarks
./../microbenchmarks/integer_throughput > ../results/raw/int.csv
./../microbenchmarks/float_throughput  > ../results/raw/flt.csv
./../microbenchmarks/mem_latency       > ../results/raw/lat.csv
./../microbenchmarks/mem_bandwidth     > ../results/raw/bw.csv

# Run workloads
python3 ../workloads/sort_benchmark.py ../results/raw/sort.csv
python3 ../workloads/sql_benchmark.py  ../results/raw/sql.csv
python3 ../workloads/ml_inference.py  ../results/raw/ml.csv

# Merge results
python3 ../harness/parse_results.py

#Generate plots
python3 ../plots/generate_plots.py

echo "All benchmarks done."