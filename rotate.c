/*  File: rotate.c
 *  Author: Richard Durbin (rd109@cam.ac.uk)
 *  Copyright (C) Richard Durbin, Cambridge University, 2021
 *-------------------------------------------------------------------
 * Description:
 * Exported functions:
 * HISTORY:
 * Last edited: May 18 22:53 2021 (rd109)
 * Created: Mon May 17 16:13:20 2021 (rd109)
 *-------------------------------------------------------------------
 */

#include "seqio.h"
#include <ctype.h>

void usage (void)
{ fprintf (stderr, "rotate [options] <input fasta filename>\n") ;
  fprintf (stderr, "  -x <pos>		position to become new start [0]\n") ;
  fprintf (stderr, "  -rc               reverse complement after rotating [false]\n") ;
  fprintf (stderr, "  -s <string>       rotate to start of search string\n") ;
  fprintf (stderr, "  -m <n_mismatch>   maximum number of mismatches allowed in search [0]\n") ;
  fprintf (stderr, "  -o <outfilename>  output file [standard output]\n") ;
  fprintf (stderr, "use either -x and optionally -rc, or -s\n") ;
  fprintf (stderr, "use filename '-' for standard input\n") ;
  fprintf (stderr, "can read .fa.gz files as well as .fa files\n") ;
  exit (0) ;
}

char complement[] = {
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, '-',   0,   0, 
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
   0, 'T',   0, 'G',   0,   0,   0, 'C',   0,   0,   0,   0,   0,   0, 'N',   0,
   0,   0,   0,   0, 'A',   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0, 't',   0, 'g',   0,   0,   0, 'c',   0,   0,   0,   0,   0,   0, 'n',   0,
   0,   0,   0,   0, 'a',   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
} ;

void reverseComplement (char *s, int len)
{ char u, *t = s+len-1 ;
  while (s <= t) { u = *s ; *s++ = complement[*t] ; *t-- = complement[u] ; }
}

int match (char* s, int sLen, char* q, int qLen, int searchErr, int *pos, char* dirn)
{
  int i, j ;
  int nMatch = 0 ;
  
  for (i = 0 ; i < sLen ; ++i)
    { int m = 0 ;
      for (j = 0 ; j < qLen ; ++j)
	if (tolower(s[(i+j)%sLen]) != tolower(q[j]))
	  if (++m > searchErr) break ;
      if (m <= searchErr)
	{ *pos = i ; ++nMatch ;
	  fprintf (stderr, "  %s\t%d\tnerr\t%d\n", dirn, i, m) ;
	}
    }
  return nMatch ;
}

int main (int argc, char *argv[])
{
  int searchErr = 0 ;
  char *searchSeq = 0 ;
  int ssLen = 0 ;
  int startPos = 0 ;
  bool isRC = false ;
  char *outFileName = "-" ;
  int i ;

  // read the command line
  
  --argc ; ++argv ;		/* swallows the command name */
  while (argc > 1)
    if (!strcmp (*argv, "-x") && argc > 2)
      { startPos = atoi (argv[1]) ;
	argc -= 2 ; argv += 2 ;
      }
    else if (!strcmp (*argv, "-rc"))
      { isRC = true ;
	argc -= 1 ; argv += 1 ;
      }
    else if (!strcmp (*argv, "-s") && argc > 2)
      { searchSeq = argv[1] ;
	ssLen = strlen (searchSeq) ;
      	argc -= 2 ; argv += 2 ;
      }
    else if (!strcmp (*argv, "-m") && argc > 2)
      { searchErr = atoi (argv[1]) ;
      	argc -= 2 ; argv += 2 ;
      }
    else if (!strcmp (*argv, "-o") && argc > 2)
      { outFileName = argv[1] ;
      	argc -= 2 ; argv += 2 ;
      }
    else usage() ;

  // open the files
	
  dna2textConv['-'] = '-' ; // allow gap characters in sequences
  
  SeqIO *si = seqIOopenRead (*argv, dna2textConv, false) ;
  if (!si) die ("failed to open input sequence file %s", *argv) ;
  if (si->type != FASTA) die ("can only process FASTA files for now - %s type is %s",
			      *argv, seqIOtypeName[si->type]) ;
  SeqIO *siOut = seqIOopenWrite (outFileName, FASTA, dna2textConv, 0) ;
  if (!siOut) die ("failed to open output sequence file %s", outFileName) ;

  // do the work
  
  while (seqIOread (si))
    { char *s = sqioSeq(si) ;
      int len = si->seqLen ;
      fprintf (stderr, "sequence\t%s\tlength\t%d\n", sqioId(si), len) ;
      if (searchSeq)
	{ int forwardMatch = match (s, len, searchSeq, ssLen, searchErr, &startPos, "forward") ;
	  reverseComplement (s, len) ;
	  int reverseMatch = match (s, len, searchSeq, ssLen, searchErr, &startPos, "reverse") ;
	  if (forwardMatch + reverseMatch != 1) continue ;
	  if (forwardMatch) reverseComplement (s, len) ; // turn back to original order
	}
      char *t = new (len, char) ;
      for (i = 0 ; i < len ; ++i) t[i] = s[(i+startPos)%len] ;
      if (isRC) reverseComplement (t, len) ;
      seqIOwrite (siOut, sqioId(si), 0, len, t, 0) ;
      seqIOflush (siOut) ;
      free (t) ;
    }
  seqIOclose (si) ;
  seqIOclose (siOut) ;
}
