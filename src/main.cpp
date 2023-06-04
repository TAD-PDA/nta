#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <signal.h>
#include "nta_key.hpp"
#include "nta_basics.hpp"
#include "nta_parser.hpp"
using namespace std;

#define DEFAULT_MAP_FILENAME "./test/ntakey.cfg"

int current_layer = 0;
int redirect_fifo_fd;

void run_nta_from_masterkey(ntakey masterkey)
{

	string userinput = "";
	bool redirected = false;

	while (getline(cin, userinput))
	{
		// Normal operation
		if (!redirected)
		{
			nta_report(NTAREP_DEBUG, "INPUT: " + userinput);
			try
			{

				stoi(userinput);
				vector<int> userinput_split;

				if (!split_sring_to_int_vector(userinput, userinput_split))
				{
					userinput_split.insert(userinput_split.begin(), current_layer);
					userinput.clear();
					int temp_layer = current_layer;

					if (!userinput_split.empty())
						masterkey.perform_recursively(userinput_split, userinput_split, temp_layer, 0);

					if (masterkey.has_child(temp_layer))
						current_layer = temp_layer;
					else
						nta_report(NTAREP_ERROR, "Layer switching failed: layer " + to_string(temp_layer) + " does not exist.");
				}
			}

			catch (invalid_argument const &)
			{
				size_t redirpos = 0;

				if (userinput == COMMAND_LIST)
				{
					masterkey.list_recursively();
					printf("\n");
				}
				else if (userinput == COMMAND_QUIT)
				{
					nta_report(NTAREP_INFO, "Quit by command");
					break;
				}
				else if ((redirpos = userinput.find(COMMAND_REDIRECT)) != string::npos)
				{
					sscanf(userinput.c_str() + redirpos + COMMAND_REDIRECT.length(), " %d ", &redirected);
				}
				else
				{
					nta_report(NTAREP_INFO, "Unknown input: " + userinput);
				}
			}
		}
		// Operation when input is being redirected to FIFO
		else
		{

			nta_report(NTAREP_DEBUG, "REDIR: " + userinput);
			// A line with the same toggle redirect command breaks out of
			// this mode.
			size_t redirpos = 0;
			if ((redirpos = userinput.find(COMMAND_REDIRECT)) != string::npos)
			{
				sscanf(userinput.c_str() + redirpos + COMMAND_REDIRECT.length(), " %d ", &redirected);
			}

			// Just echo input to the redirect
			write(redirect_fifo_fd, (userinput + '\n').c_str(), (userinput + '\n').length());
		}
	}
}

void term(int signum)
{
	nta_report(NTAREP_INFO, "Quit by signal " + to_string(signum));
	nta_keyserver_end();
	close(redirect_fifo_fd);
	exit(signum);
}

int main(int argc, char *argv[])
{

	signal(SIGTERM, term);
	signal(SIGINT, term);

	nta_keyserver_start();

	string nta_keymap_file = DEFAULT_MAP_FILENAME;

	nta_report_determine_levels(argc, argv, nta_keymap_file);

	ntakey masterkey(0);

	nta_read_config_to_key(nta_keymap_file, masterkey);

	redirect_fifo_fd = nta_open_file_for_redirect(REDIRECT_FILE);

	run_nta_from_masterkey(masterkey);

	nta_keyserver_end();
	close(redirect_fifo_fd);
}
