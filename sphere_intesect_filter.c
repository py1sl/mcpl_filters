
///////////////////////////////////////////////////////////////////////////////////
//                                                                               //
// A filter for MCPL files, that produces an output MCPL file of only the       //
// particles whose direction mean they would intersect the sphere.               //
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


double dot_product(double v[], double u[], int n)
{
    double result = 0.0;
    for (int i = 0; i < n; i++)
        result += v[i]*u[i];
    return result;
}

int main(int argc,char**argv) {

  if (argc!=3) {
    printf("Please supply input and output filenames\n");
    return 1;
  }

  const char * infilename = argv[1];
  const char * outfilename = argv[2];
  
  double sphere_x = 330;
  double sphere_y = -122;
  double sphere_z = -10;
  double sphere_rad = 5;

  // Initialisation, open existing file and create output file handle. Transfer
  // all meta-data from existing file, and add an extra comment in the output
  // file to document the process:

  mcpl_file_t fi = mcpl_open_file(infilename);
  mcpl_outfile_t fo = mcpl_create_outfile(outfilename);
  mcpl_transfer_metadata(fi, fo);
  mcpl_hdr_add_comment(fo,"Applied custom filter to select neutrons heading towards sphere");

  //Loop over particles from input, only adding the chosen particles to the output file:

  const mcpl_particle_t* particle;
  while ( ( particle = mcpl_read(fi) ) ) {
    // test if will intercept sphere
    double x = particle->position[0];
    double y = particle->position[1];
    double z = particle->position[2];
    double dpx = x - sphere_x;
    double dpy = y - sphere_y;
    double dpz = z - sphere_z;
    double dp2 = (dpx*dpx) + (dpy*dpy) + (dpz*dpz);
    
    double dplist[] = {dpx, dpy, dpz};
    
    double ddot = dot_product(dplist, particle->direction, 3); 
    
    if ( ((ddot*ddot)+(sphere_rad*sphere_rad))>(dp2) ) {
      mcpl_add_particle(fo,particle);

    }
  }

  //Close up files:
  mcpl_close_outfile(fo);
  mcpl_close_file(fi);
}

