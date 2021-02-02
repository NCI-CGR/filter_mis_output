/*!
  \file utilities.cc
  \brief implementations of nontemplated global utility functions
 */

#include "filter_mis_output/utilities.h"

bool filter_mis_output::cicompare(const std::string &s1,
				      const std::string &s2) {
  if (s1.size() == s2.size()) {
    for (unsigned i = 0; i < s1.size(); ++i) {
      if (tolower(s1.at(i)) != tolower(s2.at(i)))
	return false;
    }
    return true;
  }
  return false;
}
	
void filter_mis_output::splitline(const std::string &s,
				      std::vector<std::string> &vec,
				      const std::string &sep) {
  std::string::size_type loc = 0, cur = 0;
  vec.clear();
  while (true) {
    loc = s.find(sep, cur);
    if (loc == std::string::npos) {
      vec.push_back(s.substr(cur));
      break;
    }
    vec.push_back(s.substr(cur, loc - cur));
    cur = loc + sep.size();
  }
  return;
}

void filter_mis_output::load_mapped_variants(const std::string &filename,
						 std::map<std::string, bool> &target) {
  if (filename.empty()) return;
  finter::finter_reader *input = 0;
  std::string line = "", catcher = "";
  try {
    input = finter::reconcile_reader(filename);
    while (input->getline(&line)) {
      std::istringstream strm1(line);
      if (!(strm1 >> catcher)) continue;
      target[catcher] = true;
    }
    input->close();
    delete input;
    input = 0;
  } catch (...) {
    if (input) delete input;
    throw;
  }
}

std::string filter_mis_output::get_token(const std::string &s,
					 unsigned index,
					 const std::string &sep) {
  std::string::size_type loc = 0, cur = 0;
  for (unsigned i = 0; i < index; ++i) {
    loc = s.find(sep, cur);
    cur = loc + sep.size();
  }
  return s.substr(cur, s.find(sep, cur) - cur);
}

void filter_mis_output::process_imputed_files(const std::string &input_dose_vcf_filename,
					      const std::string &input_info_filename,
					      const std::map<std::string, bool> &inclusion_snps,
					      bool permit_file_desync,
					      const std::string &output_dose_vcf_filename,
					      const std::string &output_info_filename) {
  finter::finter_reader *input_dose_vcf = 0, *input_info = 0;
  finter::finter_writer *output_dose_vcf = 0, *output_info = 0;
  std::string input_dose_line = "", input_info_line = "";
  std::string dose_variant_id = "", info_variant_id = "";
  unsigned total_variants = 0, total_included = 0, total_excluded = 0;
  try {
    output_dose_vcf = finter::reconcile_writer(output_dose_vcf_filename);
    output_info = finter::reconcile_writer(output_info_filename);
    try {
      input_dose_vcf = finter::reconcile_reader(input_dose_vcf_filename);
    } catch (...) {
      if (input_dose_vcf_filename.rfind(".gz") == input_dose_vcf_filename.size() - 3) {
	input_dose_vcf = finter::reconcile_reader(input_dose_vcf_filename.substr(0, input_dose_vcf_filename.size() - 3));
      } else throw;
    }
    do {
      input_dose_vcf->getline(&input_dose_line);
      output_dose_vcf->writeline(input_dose_line);
    } while (input_dose_line.find("##") != std::string::npos);
    try {
      input_info = finter::reconcile_reader(input_info_filename);
    } catch (...) {
      if (input_info_filename.rfind(".gz") == input_info_filename.size() - 3) {
	input_info = finter::reconcile_reader(input_info_filename.substr(0, input_info_filename.size() - 3));
      } else throw;
    }
    if (!input_info->getline(&input_info_line))
      throw std::domain_error("missing header data for info file \"" + input_info_filename + "\"");
    output_info->writeline(input_info_line);

    //while (input_dose_vcf->getline(&input_dose_line) &&
    //	   input_info->getline(&input_info_line)) {
    while (true) {
      if (input_dose_line.empty()) {
	if (!input_dose_vcf->getline(&input_dose_line)) break;
	dose_variant_id = get_token(input_dose_line, 2, "\t");
      }
      if (input_info_line.empty()) {
	if (!input_info->getline(&input_info_line)) break;
	info_variant_id = get_token(input_info_line, 0, "\t");
      }
      if (dose_variant_id.compare(info_variant_id)) {
	// only including the following override because upstream software is dropping info lines so dose lines can be dropped to resync
	if (permit_file_desync) {
	  input_dose_line = "";
	  continue;
	}
	throw std::domain_error("line mismatch for dose variant \"" + dose_variant_id + "\"/info variant \"" + info_variant_id + "\"");
      }
      ++total_variants;
      if (inclusion_snps.find(dose_variant_id) != inclusion_snps.end()) {
	output_dose_vcf->writeline(input_dose_line);
	output_info->writeline(input_info_line);
	++total_included;
      } else {
	++total_excluded;
      }
      if (total_variants % 10000 == 0) {
	std::cout << "\r\tprocessed " << total_variants << " variants...";
	std::cout.flush();
      }
      input_dose_line = input_info_line = "";
    }
    std::cout << "." << std::endl;
    input_dose_vcf->close();
    delete input_dose_vcf;
    input_dose_vcf = 0;
    input_info->close();
    delete input_info;
    input_info = 0;
    output_dose_vcf->close();
    delete output_dose_vcf;
    output_dose_vcf = 0;
    output_info->close();
    delete output_info;
    output_info = 0;
    std::cout << "\tencountered " << total_variants << " variants, kept " << total_included
	      << ", removed " << total_excluded << " ("
	      << (static_cast<double>(total_included) / static_cast<double>(total_variants)) << ")" << std::endl;
  } catch (...) {
    if (input_dose_vcf) delete input_dose_vcf;
    if (input_info) delete input_info;
    if (output_dose_vcf) delete output_dose_vcf;
    if (output_info) delete output_info;
    throw;
  }
}
