# Summary

Some DNA sequences, such as most viral, bacterial and organelle genomes,
are circular as opposed to linear. When these sequences are deposited
into public repositories, they are formatted as linear sequences by
assigning them starting positions and orientations. These starting
positions and orientations can be somewhat arbitrary and often differ
across individuals or taxa. genomes whose starting positions differ by
576 nucleotides . However, most multiple sequence alignment programs
(e.g. MAFFT , MUSCLE ) assume linearity of sequences, including the same
starting position. Performing accurate downstream analyses, then,
requires first standardizing both their orientation and starting
position.  
designated control region or D-loop; however, the chimpanzee mtDNA
sequence has position number one placed in tRNA phenylalanine, which
will be position 577 in human mtDNA. " from CSA paper.

Rotate is a command-line program written in C that takes as input an
(optionally gzipped) FASTA file of DNA sequences and either a new
starting position (offset in bp) or anchor string which uniquely defines
a new starting position, and outputs a FASTA file with the same
sequences appropriately rotated. If a string is given as input, it can
allow for any number of mismatches up to a user-defined threshold, and
will also search for and output reverse complements when necessary. The
program will fail if the specified input string is not unique in a
target sequence, allowing for mismatches, and return the locations
instead. It has no external dependencies. It is available at
<https://github.com/richarddurbin/rotate>.

# Statement of need

Multiple programs or software packages already exist with similar
functionality, but have various restrictions on input, extensive
dependencies, or do not allow user-defined starting positions or anchor
strings. For example, CSA is restricted to 32 total input sequences, and
rotates to an "optimal" rotation for multiple sequence alignment instead
of a user-defined starting position. Likewise, Circlator’s fixstart
function does not accept a user-defined start position or anchor string,
and instead tries to detect dnaA genes to guide the rotation, and the
software has many dependencies (including BWA , Prodigal , MUMmer , and
Canu or SPAdes ). Similarly to CSA, MARS uses a sophisticated algorithm
to compute the optimal rotation and can even integrate this into a
multiple alignment algorithm, but again does not allow for a
user-specified string or position. Though this optimal rotation is
desirable in many contexts, the ability to rotate to a user-defined
sequence or position is highly valuable, for example because it allows
for the iterative inclusion of new sequences without re-running the
algorithm on the entire dataset. Rotate is extremely fast and has no
dependencies, and its runtime scales linearly with the number of input
sequences, since every sequence is processed separately. Rotate is easy
to compile, and we tested its functionality on macOS 12.5.1 and on
Scientific Linux 7.9. Lastly, it is already being used in scientific
publications (e.g. ).

# Example

Here we give an example of how to use rotate in combination with a
multiple sequence alignment program of choice to rotate a large dataset
of sequences to any common start position. We downloaded two sets of
publicly available organelle assemblies from NCBI’s RefSeq database: (1)
all available complete mammalian mitochondrial genomes (n=1,546) and (2)
all available complete chloroplast genomes of the Rosaceae family
(n=465). For the mammalian mitochondria, we selected a conserved
mitochondrial sequence (using the 100 Vertebrate Cons track in the UCSC
Genome Browser ) as the anchor string, while for the chloroplasts we
used a common barcode primer . We first rotated the sequences with
rotate, specifying the anchor string (-s) and maximum number of
mismatches (-m), respectively. Execution of this first step on a Linux
machine using a single CPU with 6GB RAM on the took 3.165 and 3.422
seconds to rotate 1,546 mitochondrial and 465 chloroplast sequences,
respectively (wall clock time). Next we generate a multiple sequence
alignment for both rotated sets of sequences with Mafft and then perform
a second rotation by position (-x) to accomplish conventional
mitochondrial and chloroplast start positions in the multiple sequence
alignments.

    ## 1,546 mitochondria

    # rotate to anchor string, allowing for 4 mismatches
    ./rotate -s TACGACCTCGATGTTGGATCA -m 4 mammalia.fa > mammalia.rotated.fa

    # align with mafft
    ./mafft mammalia.rotated.fa > mammalia.aligned.fa

    # offset to common mitochondrial start position
    ./rotate -x 23859 mammalia.aligned.fa > mammalia.final.fa


    ## 465 chloroplasts

    # rotate to anchor string, allowing for 1 mismatch
    ./rotate -s CGAAATCGGTAGACGCTACG -m 1 rosaceae.fa > rosaceae.rotated.fa

    # align with mafft
    ./mafft rosaceae.rotated.fa > rosaceae.aligned.fa

    # offset to conventional chloroplast start position
    ./rotate -x 163249 rosaceae.aligned.fa > rosaceae.final.fa

# Acknowledgements

BDS acknowledges support from the Wellcome Trust programme in
Mathematical Genomics and Medicine (WT220023). MB acknowledges funding
from the Harding Distinguished Postgraduate Scholars Programme.

<div class="singlespace">

</div>
