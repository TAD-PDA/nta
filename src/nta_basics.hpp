#ifndef NTABASICS_HPP_ /* Include guard */
#define NTABASICS_HPP_

#include <string>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <string>
#include <sstream>
#include <iostream>
#include <signal.h>
#include <fcntl.h>
#include <pwd.h>
#include <sys/stat.h>
#include <sys/types.h>

using namespace std;

#define NTA_HELPTEXT "Usage: nta [OPTION]\n"                                          \
					 "\n"                                                             \
					 "Application options :\n"                                        \
					 "	 -l --log		FILE	Redirect stdout to FILE"                       \
					 "   -i 			FILE	Use FILE as input\n"                              \
					 "   -f 			FILE	Use keymap from FILE\n"                           \
					 "   -t	--test-mode 		Print commands instead of executing them\n" \
					 "   -h --help				Show this text\n"                               \
					 "\n"                                                             \
					 "Reporting options :\n"                                          \
					 "   --report-warn\n"                                             \
					 "   --report-info\n"                                             \
					 "   --report-expand\n"                                           \
					 "   --report-error \n"                                           \
					 "   --report-debug\n"                                            \
					 "   --report-verbose\n"                                          \
					 "   --report-noparse\n"                                          \
					 "   --report-noexpand\n";

#define NTAREP_WARN 0b1L
#define NTAREP_ERROR 0b10L
#define NTAREP_INFO 0b100L
#define NTAREP_EXPAND 0b1000L
#define NTAREP_DEBUG 0b10000L
#define NTAREP_PARSE 0b100000L

#define COMMAND_QUIT string("quit")
#define COMMAND_LIST string("list")
#define COMMAND_REDIRECT string("@NTAREDIR@")

#define REDIRECT_FILE "/tmp/nta_redirect"
#define LOG_FILE "./nta.log"

#define NTAREP_DEFAULT (NTAREP_ERROR | NTAREP_PARSE)
#define NTAREP_VERBOSE (NTAREP_WARN | NTAREP_ERROR | NTAREP_INFO | NTAREP_EXPAND | NTAREP_DEBUG | NTAREP_PARSE)

void nta_print_help();

void nta_report_determine_levels(int argc, char *argv[], string &keymap_file);

void nta_report(const long int type, const string report);

void nta_report_source(const string source, const long int type, const string report);

int nta_open_file_for_redirect(string filename);

int split_sring_to_int_vector(string s, vector<int> &v);

#endif
