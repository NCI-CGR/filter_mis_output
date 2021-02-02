/*!
  \file utilities.h
  \brief general global utility functions in namespace
  \author Cameron Palmer
*/

#ifndef __FILTER_MIS_OUTPUT_UTILITIES_H__
#define __FILTER_MIS_OUTPUT_UTILITIES_H__

#include <stdexcept>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <map>
#include <cmath>
#include <cfloat>

#include "finter/finter.h"

namespace filter_mis_output {
  /*!
    \brief convert string to arbitrary class representation
    @tparam value_type output class for conversion
    @param s input string to be converted to new class
    \return new class representation of input string
    \warning requires support for object class with stringstream
  */
  template <class value_type>
  value_type from_string(const std::string &s) {
    std::istringstream strm1(s);
    value_type res;
    if (!(strm1 >> res))
      throw std::domain_error("from_string: cannot convert string to object: \"" + s + "\"");
    return res;
  }
	
  /*!
    \brief perform case-insensitive comparison for identity of two strings
    @param s1 first string for comparison
    @param s2 second string for comparison
    \return whether the two input strings are case-insensitive identical
  */
  bool cicompare(const std::string &s1,
		 const std::string &s2);
  /*!
    \brief split a string into tokens based on arbitrary delimiter
    @param s string in need of parsing
    @param vec where the tokenized result will be stored
    @param sep arbitrary delimiter used to denote tokens
    \warning data present in vec will be purged before adding results
  */
  void splitline(const std::string &s,
		 std::vector<std::string> &vec,
		 const std::string &sep);

  /*!
    \brief take a file, load the first entry on each line into a map
    @param filename name of file to load
    @param target where to store the loaded data

    Used for a plink-style variant inclusion list
   */
  void load_mapped_variants(const std::string &filename,
			    std::map<std::string, bool> &target);


  std::string get_token(const std::string &s,
			unsigned index,
			const std::string &sep);
  
  void process_imputed_files(const std::string &input_dose_vcf_filename,
			     const std::string &input_info_filename,
			     const std::map<std::string, bool> &inclusion_snps,
			     bool permit_file_desync,
			     const std::string &output_dose_vcf_filename,
			     const std::string &output_info_filename);
}

#endif //__FILTER_MIS_OUTPUT_UTILITIES_H__
