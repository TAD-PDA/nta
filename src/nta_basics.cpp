#include "nta_basics.hpp"

long int nta_report_global_allowed_levels = NTAREP_DEFAULT;
bool nta_global_noexecute = 0;

void print_timestamp()
{
	time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	char buffer[128];
	std::strftime(buffer, 128 * sizeof(char), "%d.%m.%Y|%T", std::localtime(&time));
	printf("[%s]", buffer);
}

int nta_open_file_for_redirect(string filename)
{
	fclose(fopen(filename.c_str(), "w"));
	int fd = open(filename.c_str(), O_WRONLY | O_NONBLOCK);
	return fd;
};

void nta_load_logfile(string filename)
{
	freopen(filename.c_str(), "w", stdout);
}

void nta_print_help()
{
	const char *usagetext = NTA_HELPTEXT;
	printf("%s", usagetext);
}

void nta_report_determine_levels(int argc, char *argv[], string &keymap_file)
{
	const char *homedir;

	if ((homedir = getenv("HOME")) == NULL)
	{
		homedir = getpwuid(getuid())->pw_dir;
	}

	keymap_file = homedir + keymap_file;

	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "--report-warn") == 0)
		{
			nta_report_global_allowed_levels |= NTAREP_WARN;
		}
		else if (strcmp(argv[i], "--report-error") == 0)
		{
			nta_report_global_allowed_levels |= NTAREP_ERROR;
		}
		else if (strcmp(argv[i], "--report-info") == 0)
		{
			nta_report_global_allowed_levels |= NTAREP_INFO;
		}
		else if (strcmp(argv[i], "--report-expand") == 0)
		{
			nta_report_global_allowed_levels |= NTAREP_EXPAND;
		}
		else if (strcmp(argv[i], "--report-debug") == 0)
		{
			nta_report_global_allowed_levels |= NTAREP_DEBUG;
		}
		else if ((strcmp(argv[i], "-t") == 0) || (strcmp(argv[i], "--test-mode") == 0))
		{
			nta_global_noexecute = 1;
		}
		else if ((strcmp(argv[i], "-l") == 0) || (strcmp(argv[i], "--log") == 0))
		{
			if (i + 1 < argc)
			{
				nta_load_logfile(argv[i + 1]);
			}
			else
			{
				printf("No file passed.\n");
				exit(66);
			}
			i++;
		}
		else if (strcmp(argv[i], "-f") == 0)
		{
			if (i + 1 < argc)
			{
				keymap_file = argv[i + 1];
			}
			else
			{
				printf("No file passed.\n");
				exit(66);
			}
			i++;
		}
		else if (strcmp(argv[i], "-i") == 0)
		{
			if (i + 1 < argc)
			{
				freopen(argv[i + 1], "r", stdin);
			}
			else
			{
				printf("No file passed.\n");
				exit(66);
			}
			i++;
		}
		else if (strcmp(argv[i], "--report-verbose") == 0)
		{
			nta_report_global_allowed_levels |= NTAREP_VERBOSE;
		}
		else if (strcmp(argv[i], "--report-noexpand") == 0)
		{
			nta_report_global_allowed_levels &= !NTAREP_EXPAND;
		}
		else if (strcmp(argv[i], "--report-noparse") == 0)
		{
			nta_report_global_allowed_levels &= !NTAREP_PARSE;
		}
		else
		{
			nta_print_help();
			exit(64);
		}
	}
}

void nta_report(const long int type, const string report)
{
	
	if (nta_report_global_allowed_levels & type)
	{
		print_timestamp();
		switch (nta_report_global_allowed_levels & type)
		{
		case NTAREP_WARN:
			printf("[WARN] ");
			break;
		case NTAREP_ERROR:
			printf("[ERROR] ");
			break;
		case NTAREP_INFO:
			printf("[INFO] ");
			break;
		case NTAREP_EXPAND:
			printf("[EXPAND] ");
			break;
		case NTAREP_DEBUG:
			printf("[DEBUG] ");
			break;
		case NTAREP_PARSE:
			printf("[PARSE] ");
			break;
		default:
			break;
		}
		printf("%s\n", report.c_str());
	}
};

void nta_report_source(const string source, const long int type, const string report)
{
	
	if (nta_report_global_allowed_levels & type)
	{
		print_timestamp();
		switch (nta_report_global_allowed_levels & type)
		{
		case NTAREP_WARN:
			printf("[WARN|%s] ", source.c_str());
			break;
		case NTAREP_ERROR:
			printf("[ERROR|%s] ", source.c_str());
			break;
		case NTAREP_INFO:
			printf("[INFO|%s] ", source.c_str());
			break;
		case NTAREP_EXPAND:
			printf("[EXPAND|%s] ", source.c_str());
			break;
		case NTAREP_DEBUG:
			printf("[DEBUG|%s] ", source.c_str());
			break;
		case NTAREP_PARSE:
			printf("[PARSE|%s] ", source.c_str());
			break;
		default:
			break;
		}
		printf("%s\n", report.c_str());
	}
};

int split_sring_to_int_vector(string s, vector<int> &v)
{
	v.clear();
	string temp = "";
	for (unsigned int i = 0; i < s.length(); ++i)
	{

		if (s[i] == ' ')
		{
			try
			{
				v.push_back(stoi(temp));
			}
			catch (invalid_argument const &)
			{
				nta_report(NTAREP_WARN, "Input formatted incorrectly, aborting execution");
				v.clear();
				return 1;
			}
			temp = "";
		}
		else
		{
			temp.push_back(s[i]);
		}
	}
	try
	{
		v.push_back(stoi(temp));
	}
	catch (invalid_argument const &)
	{
		nta_report(NTAREP_WARN, "Input formatted incorrectly, aborting execution");
		v.clear();
		return 1;
	}
	return 0;
}
