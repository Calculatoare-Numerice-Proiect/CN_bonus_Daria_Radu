#!/usr/bin/env bash
#
# monitor_sys.sh (macOS)
# Usage: monitor_sys.sh <interval_s> <out_csv> <pid_to_watch>
#
# Logs timestamp (epoch), cpu_pct (for that PID), and mem_mb (RSS in MB).

interval=$1
out=$2
pid=$3

# Write header
echo "timestamp,cpu_pct,mem_mb" > "$out"

# Loop until the process exits
while kill -0 "$pid" 2>/dev/null; do
  # Current time in seconds since epoch
  ts=$(date +%s)

  # Use ps to get CPU% and RSS (in kilobytes) for that PID
  #   -o %cpu,rss prints two columns; skip the header row with tail -n +2
  read cpu rss_kb <<<$(ps -p "$pid" -o %cpu= -o rss= | awk '{print $1, $2}')

  # Convert RSS (KB) to MB with one decimal
  mem_mb=$(awk -v r="$rss_kb" 'BEGIN { printf("%.1f", r/1024) }')

  echo "$ts,$cpu,$mem_mb" >> "$out"
  sleep "$interval"
done
