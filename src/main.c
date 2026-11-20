#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include <string.h>

#include "shared/config.h"
#include "commands/la.h"
#include "commands/analyse.h"
#include "commands/lamin.h"
#include "commands/gamme.h"

struct command {
    char* name;
    int (*fn)(const struct config*);
};

const struct command commands[] = {
    { "la"   , run_la_command    },
    { "analyse"   , run_analyse_command    },
    { "lamin"   , run_lamin_command    },
    { "gamme", run_gamme_command },
    { NULL , NULL }
};

const char* short_options = "o:h";
const struct option long_options[] = {
    { "output", required_argument, NULL, 'o'},
    { "help"  , no_argument      , NULL, 'h'},
    { NULL    , 0                , NULL, 0  }
};

void help() {
    printf("help.\n");
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
    if (optind >= argc || argv[optind] == NULL || want_help) {
        help();
        return want_help ? EXIT_SUCCESS : EXIT_FAILURE;
    }

    const char* command_name = argv[optind];
    size_t i = 0;
    while (commands[i].name != NULL && strcmp(commands[i].name, command_name) != 0) { i++; }
    if (commands[i].name != NULL && commands[i].fn != NULL) { return commands[i].fn(&cfg); }

    fprintf(stderr, "Error : unknown command '%s'.\n", command_name);
    help();
    return EXIT_FAILURE;
}
