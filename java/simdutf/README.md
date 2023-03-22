# SIMD for UTF-8 Verify

This project is motivated by Lemire's [simdutf](https://github.com/lemire/simdutf) project.

## Data Preparation

```sh
./generate.sh
```

It will download or generate the following datasets:

- `financial.csv` (ASCII)
- `uuid.txt` (ASCII)

You can also copy the dataset into the `data` folder manually.

## Build and Run

It provides several profiles for testing by specifying `-Dtests=`. Except for `fast` profile, you can also specify a file to process using `-Dfile=`.

- `fast` (default): only unit tests
- `benchmark`: using JMH framework
- `ascii`: a quick (not-standard) benchmark test for ASCII verify

## ASCII Verify

```sh
mvn clean test -Dtests=ascii -Dfile=financial.csv -q
```

| Method                  | Speed         |
| ----------------------- | ------------- |
| `checkASCIIByException` | 1599.18 MB/s  |
| `checkASCIIByRange`     | 8151.27 MB/s  |
| `checkASCIIBySWAR`      | 21794.38 MB/s |
| `checkASCIIBySIMD`      | 46146.18 MB/s |
