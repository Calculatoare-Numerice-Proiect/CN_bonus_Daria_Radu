import sqlite3, time, sys, csv

# simple in-memory join test
if __name__ == "__main__":
    conn = sqlite3.connect(":memory:")
    c = conn.cursor()
    c.execute("CREATE TABLE A (id INTEGER, val TEXT);")
    c.execute("CREATE TABLE B (id INTEGER, val TEXT);")
    # populate
    N = 100000
    for i in range(N):
        c.execute("INSERT INTO A VALUES (?, ?)", (i, str(i)))
        c.execute("INSERT INTO B VALUES (?, ?)", (i, str(i)))
    conn.commit()
    t0 = time.time()
    c.execute("SELECT A.id, A.val, B.val FROM A JOIN B ON A.id = B.id;")
    c.fetchall()
    t1 = time.time()
    elapsed = t1 - t0
    with open(sys.argv[1], 'w', newline='') as f:
        w = csv.writer(f)
        w.writerow(["rows","time_s"])
        w.writerow([N, f"{elapsed:.6f}"])
    print(f"rows={N}, time={elapsed:.6f}")