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
  -x <pos>              position to become new start [0]
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
