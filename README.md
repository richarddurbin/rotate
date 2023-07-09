[![DOI](https://zenodo.org/badge/664302121.svg)](https://zenodo.org/badge/latestdoi/664302121)

# rotate
simple program to rotate a circular sequence to start at a given position or string

To download and compile:
```
git clone https://github.com/richarddurbin/rotate.git ; cd rotate ; make
```
To run:
```
./rotate
rotate [options] <input fasta filename>
  -x <pos>          position to become new start [0]
  -rc               reverse complement after rotating [false]
  -s <string>       rotate to start of search string
  -m <n_mismatch>   maximum number of mismatches allowed in search [0]
  -o <outfilename>  output file [standard output]
use either -x and optionally -rc, or -s
use filename '-' for standard input
can read .fa.gz files as well as .fa files

./rotate -s tcta test.fa > output.fa
sequence        1       length  11
  forward       4       nerr    0
sequence        2       length  11
  forward       10      nerr    0
sequence        3       length  12
sequence        4       length  11
  reverse       10      nerr    0

cat output.fa
>1
TCTACGGAACG
>2
TCTACGGAAGG
>4
TCTACGGAAGG
```

# composition
You also get for free a handy utility to inspect a (optionally gzipped) fasta or fastq file and return the number of sequences and their lengths, with optional additional information.

```
./composition
Usage: composition [opts] <filename>
  will read fasta, fastq, optionally gzipped.  Use filename '-' for stdin.
  options:
    -b : show base counts
    -q : show quality counts
    -t : show time and memory used
    -l : show length distribution in up to 20 quadratic bins
    
./composition test.fa 
fasta file, 4 sequences >= 0, 45 total, 11.25 average, 11 min, 12 max

./composition -b -t -l test.fa
fasta file, 4 sequences >= 0, 45 total, 11.25 average, 11 min, 12 max
bases
  A 11 24.4 %
  C 12 26.7 %
  G 12 26.7 %
  T 10 22.2 %
approximate N50 11
length distribution (quadratic bins)
  10    3
  11    1
user    0.000028        system  0.000097        max_RSS 114688  memory  16902632
```
