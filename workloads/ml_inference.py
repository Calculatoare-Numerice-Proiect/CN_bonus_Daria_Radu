    # instead of a single size:
    # X, y = make_classification(n_samples=100000, n_features=20)

    sizes = [10_000, 50_000, 100_000, 200_000]
    with open(sys.argv[1], 'w', newline='') as f:
        w = csv.writer(f)
        w.writerow(["samples","time_s"])
        for n in sizes:
            X, y = make_classification(n_samples=n, n_features=20)
            clf = RandomForestClassifier(n_estimators=10)
            clf.fit(X, y)
            t0 = time.time()
            clf.predict(X)
            t1 = time.time()
            w.writerow([n, f"{(t1-t0):.6f}"])
            print(f"{n},{(t1-t0):.6f}")
