/*!
  \file cargs.cc
  \brief method implementation for command line argument parser class
*/

#include "filter_mis_output/cargs.h"

void filter_mis_output::cargs::initialize_options() {
  _desc.add_options()
    ("help,h", "emit this help message")
    ("input-data-directory", boost::program_options::value<std::string>()->default_value("."), "directory containing MIS output files")
    ("output-data-directory", boost::program_options::value<std::string>()->default_value("."), "directory to which to write results")
    ("chromosome", boost::program_options::value<unsigned>(), "if desired, individual chromosome to process")
    ("chromosome-lower-bound", boost::program_options::value<unsigned>(), "if desired, lowest (number) chromosome to process")
    ("chromosome-upper-bound", boost::program_options::value<unsigned>(), "if desired, highest (number) chromosome to process")
    ("chromosome-list", boost::program_options::value<unsigned>()->multitoken(), "if desired, arbitrary list of chromosomes to process")
    ("inclusion-file", boost::program_options::value<std::string>(), "name of file containing variants to keep")
    ("permit-file-desync", "allow dose and info line mismatches by chomping dose line indiscriminately (don't use this unless you know why...)")
    ;
}
