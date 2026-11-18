#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include <string.h>

#include "shared/config.h"
#include "commands/la.h"

struct command {
    char* name;
    int (*fn)(const struct config*);
};

int run_all_command(const struct config *cfg) {
    int result = 0;
    result += run_la_command(cfg);
    return result;
}

const struct command commands[] = {
    { "la" , run_la_command  },
    { "all", run_all_command },
    { NULL , NULL }
};

const char* short_options = "h";
const struct option long_options[] = {
    { "output", required_argument, NULL, 'o'},
    { "help"  , no_argument      , NULL, 'h'},
    { NULL    , 0                , NULL, 0  }
};

void help() {
    printf("help.");
}

int main(int argc, char** argv) {
    struct config cfg = {
        .output_dir = "../out/out.wav",
        .sampling_freq = 44100,
        .channels_nb = 1,
        .millis = 6000,
    };
    bool want_help = false;

    int opt;
    int option_index = 0;

    while ((opt = getopt_long(argc, argv, short_options, long_options, &option_index)) != -1) {
        switch (opt) {
            case 'o': cfg.output_dir = optarg; break;
            case 'h': want_help = true;        break;
            case '?':                   // FALLTHOUGH
            default:  help();     return EXIT_FAILURE;
        }
    }

    // no command given or only --help
    if (optind >= argc) { help(); return want_help ? EXIT_SUCCESS : EXIT_FAILURE; }

    char* command_name = argv[optind];
    size_t i = 0;
    struct command command = commands[i];
    while (command.name != NULL || strcmp(command.name, command_name) != 0) { i++; }
    if (command.name != NULL) { return command.fn(&cfg); }

    fprintf(stderr, "Error : unknown command '%s'.\n", command_name);
    help();
    return EXIT_FAILURE;
}
