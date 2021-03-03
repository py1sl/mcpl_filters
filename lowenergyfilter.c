
///////////////////////////////////////////////////////////////////////////////////
//                                                                               //
// A filter for MCPL files, that produces an output MCPL file of only the       //
// particles have energy below ethreshold.               //
//                                                                               //
// This file can be freely used as per the terms in the LICENSE file.            //
//                                                                               // 
// author: Steven Lilley steven.lilley@stfc.ac.uk                                //
// Based on rawExample_filtermcpl.c written 2015-2016 by                         //
// Thomas.Kittelmann@esss.se                                                     //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////

#include "mcpl.h"
#include <stdio.h>

int main(int argc,char**argv) {

  if (argc!=4) {
    printf("Please supply input and output filenames\n");
    return 1;
  }

  const char * infilename = argv[1];
  const char * outfilename = argv[2];
  const double * ethreshold = argv[3];
  

  // Initialisation, open existing file and create output file handle. Transfer
  // all meta-data from existing file, and add an extra comment in the output
  // file to document the process:

  mcpl_file_t fi = mcpl_open_file(infilename);
  mcpl_outfile_t fo = mcpl_create_outfile(outfilename);
  mcpl_transfer_metadata(fi, fo);
  mcpl_hdr_add_comment(fo,"Applied custom filter to select particles with ekin less than ethreshold");

  //Loop over particles from input, only adding the chosen particles to the output file:

  const mcpl_particle_t* particle;
  while ( ( particle = mcpl_read(fi) ) ) {
    if ( particle->pdgcode == 2112 && particle->ekin < ethreshold ) {
      mcpl_add_particle(fo,particle);
      //Note that a guaranteed non-lossy alternative to mcpl_add_particle(fo,particle)
      //would be mcpl_transfer_last_read_particle(fi,fo) which can work directly on
      //the serialised on-disk particle data.
    }
  }

  //Close up files:
  mcpl_closeandgzip_outfile(fo);
  mcpl_close_file(fi);
}

