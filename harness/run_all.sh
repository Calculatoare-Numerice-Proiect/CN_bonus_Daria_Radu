#!/usr/bin/env bash
set -e

mkdir -p ../results/raw ../results/processed ../plots

# Compile microbenchmarks
gcc -O3 ../microbenchmarks/integer_throughput.c -o ../microbenchmarks/integer_throughput
gcc -O3 ../microbenchmarks/float_throughput.c -o ../microbenchmarks/float_throughput -lm
gcc -O3 ../microbenchmarks/mem_latency.c -o ../microbenchmarks/mem_latency
gcc -O3 ../microbenchmarks/mem_bandwidth.c -o ../microbenchmarks/mem_bandwidth
# gcc -O3 -fopenmp ../microbenchmarks/thread_scalability.c -o ../microbenchmarks/thread_scalability
# gcc -O3 ../microbenchmarks/cache_latency.c -o ../microbenchmarks/cache_latency
clang -O3 -Xpreprocessor -fopenmp \
      -I/opt/homebrew/opt/libomp/include \
      ../microbenchmarks/thread_scalability.c \
      -L/opt/homebrew/opt/libomp/lib -lomp \
      -o ../microbenchmarks/thread_scalability

gcc -Wall -o ../microbenchmarks/cache_latency ../microbenchmarks/cache_latency.c -lm

# Run microbenchmarks
./../microbenchmarks/integer_throughput > ../results/raw/int.csv & pid=$!
./monitor_sys.sh 1 ../results/raw/int_sys.csv $pid &
mon_pid=$!
wait $pid
kill $mon_pid
./../microbenchmarks/float_throughput  > ../results/raw/flt.csv & pid=$!
./monitor_sys.sh 1 ../results/raw/flt_sys.csv $pid &
mon_pid=$!
wait $pid
kill $mon_pid
./../microbenchmarks/mem_latency       > ../results/raw/lat.csv & pid=$!
./monitor_sys.sh 1 ../results/raw/lat_sys.csv $pid &
mon_pid=$!
wait $pid
kill $mon_pid
./../microbenchmarks/mem_bandwidth     > ../results/raw/bw.csv & pid=$!
./monitor_sys.sh 1 ../results/raw/bw_sys.csv $pid &
mon_pid=$!
wait $pid
kill $mon_pid
./../microbenchmarks/cache_latency > ../results/raw/cache_latency.csv & pid=$!
./monitor_sys.sh 1 ../results/raw/cache_sys.csv $pid &
mon_pid=$!
wait $pid
kill $mon_pid
./../microbenchmarks/thread_scalability > ../results/raw/thread_scalability.csv & pid=$!
./monitor_sys.sh 1 ../results/raw/thread_sys.csv $pid &
mon_pid=$!
wait $pid
kill $mon_pid

# Run workloads
/Library/Frameworks/Python.framework/Versions/3.11/bin/python3.11 ../workloads/sort_benchmark.py ../results/raw/sort.csv
/Library/Frameworks/Python.framework/Versions/3.11/bin/python3.11 ../workloads/sql_benchmark.py  ../results/raw/sql.csv
/Library/Frameworks/Python.framework/Versions/3.11/bin/python3.11 ../workloads/ml_inference.py  ../results/raw/ml.csv

# Merge results
/Library/Frameworks/Python.framework/Versions/3.11/bin/python3.11 ../harness/parse_results.py

#Generate plots
/Library/Frameworks/Python.framework/Versions/3.11/bin/python3.11 ../plots/generate_plots.py


echo "All benchmarks done."