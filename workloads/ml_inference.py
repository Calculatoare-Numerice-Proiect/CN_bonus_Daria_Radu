import time, sys, csv
from sklearn.ensemble import RandomForestClassifier
from sklearn.datasets import make_classification

if __name__ == "__main__":
    X, y = make_classification(n_samples=100000, n_features=20)
    clf = RandomForestClassifier(n_estimators=10)
    clf.fit(X, y)
    t0 = time.time()
    clf.predict(X)
    t1 = time.time()
    elapsed = t1 - t0
    with open(sys.argv[1], 'w', newline='') as f:
        w = csv.writer(f)
        w.writerow(["samples","time_s"])
        w.writerow([len(X), f"{elapsed:.6f}"])
    print(f"samples={len(X)}, time={elapsed:.6f}")