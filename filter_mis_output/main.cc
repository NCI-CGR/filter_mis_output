#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <utility>
#include <stdexcept>

#include "filter_mis_output/cargs.h"
#include "filter_mis_output/utilities.h"

int main(int argc, char **argv) {
  try {
    filter_mis_output::cargs ap(argc, argv);
    if (ap.help() || argc == 1) {
      ap.print_help(std::cout);
      return 0;
    }
    std::cout << "hi there! hope you're having a nice day" << std::endl;
    std::string input_data_directory = ap.get_input_data_directory();
    std::string output_data_directory = ap.get_output_data_directory();
    std::string inclusion_list = ap.get_inclusion_list();
    unsigned chromosome = ap.get_chromosome();
    unsigned chromosome_lower_bound = ap.get_chromosome_lower_bound();
    unsigned chromosome_upper_bound = ap.get_chromosome_upper_bound();
    std::vector<unsigned> chromosome_list = ap.get_chromosome_list();
    std::map<unsigned, bool> target_chromosomes;
    std::map<std::string, bool> inclusion_snps;
    std::cout << "reconciling requested chromosomes" << std::endl;
    if (chromosome) target_chromosomes[chromosome] = true;
    if (chromosome_lower_bound || chromosome_upper_bound) {
      for (unsigned i = (chromosome_lower_bound ? chromosome_lower_bound : 1);
	   i <= (chromosome_upper_bound ? chromosome_upper_bound : 22);
	   ++i) {
	target_chromosomes[i] = true;
      }
    }
    for (std::vector<unsigned>::const_iterator iter = chromosome_list.begin();
	 iter != chromosome_list.end();
	 ++iter) {
      if (*iter)
	target_chromosomes[*iter] = true;
    }
    std::cout << "reading variant inclusion data from file \"" << inclusion_list << "\"" << std::endl;
    filter_mis_output::load_mapped_variants(inclusion_list, inclusion_snps);
    for (std::map<unsigned, bool>::const_iterator iter = target_chromosomes.begin();
	 iter != target_chromosomes.end();
	 ++iter) {
      if (iter->first < 1 || iter->first > 22) continue;
      std::cout << "processing chromosome " << iter->first << std::endl;
      std::string chr = std::to_string(iter->first);
      filter_mis_output::process_imputed_files(input_data_directory + "/chr" + chr + ".dose.vcf.gz",
					       input_data_directory + "/chr" + chr + ".info.gz",
					       inclusion_snps,
					       ap.permit_file_desync(),
					       output_data_directory + "/chr" + chr + "-filtered.dose.vcf.gz",
					       output_data_directory + "/chr" + chr + "-filtered.info.gz");
      std::cout << "\tcompleted chromosome " << iter->first << std::endl;
    }
    std::cout << "all done hooray!" << std::endl;
    return 0;
  } catch (const std::domain_error &e) {
    std::cerr << "error: " << e.what() << std::endl;
    return 1;
  } catch (...) {
    std::cerr << "error: unhandled exception" << std::endl;
    return 2;
  }
}
