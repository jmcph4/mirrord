#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <unistd.h>

enum ip_mode {
    IP_MODE_IPV4 = 4,
    IP_MODE_IPV6 = 6,
    IP_MODE_ANY = 0
};

struct options {
    enum ip_mode ip_mode;
    bool no_daemon;
    char* log_path;
    char* address;
    int port;
    char* directory;
};

void usage(void) {
    fprintf(stderr, "usage: [-46d] [-a access.log] [-l address] [-p port]"
		    " directory\n");
    exit(EXIT_FAILURE);
}

struct options parseoptions(int argc, char** argv) {
    if(argc <= 1) {
        usage();
    }

    bool ipv4_only = false;
    bool ipv6_only = false;
    bool no_daemon = false;
    char* log_path = NULL;
    char* address = NULL;
    int port = 0;
    char* directory = NULL;

    int opt = 0;

    while((opt = getopt(argc, argv, "46da:l:p:")) != -1) {
        switch(opt) {
	    case '4':
		    if(ipv6_only) {
		    	usage();
		    }

		    ipv4_only = true;
		    break;
            case '6':
		    if(ipv4_only) {
		        usage();
		    }

		    ipv6_only = true;
		    break;
            case 'd':
		    no_daemon = true;
		    break;
	    case 'a':
		    log_path = optarg;
		    break;
	    case 'l':
		    address = optarg;
		    break;
	    case 'p':
		    port = atoi(optarg);
		    break;
	    default:
		    usage();
		    break;
	}

	if(optind > argc) {
	    usage();
	}
    }

    struct options opts;

	/* set IP address mode */
	if(ipv4_only) {
	    opts.ip_mode = IP_MODE_IPV4;
	} else if(ipv6_only) {
	    opts.ip_mode = IP_MODE_IPV6;
	} else {
	    opts.ip_mode = IP_MODE_ANY;
	}

        /* set daemon setting */
	opts.no_daemon = no_daemon;

	/* set path to access log */
	if(log_path != NULL) {
	    opts.log_path = calloc(strlen(log_path) + 1, sizeof(char));

	    if(opts.log_path == NULL) {
		perror("calloc");
	        exit(EXIT_FAILURE);
	    }

	    strlcpy(opts.log_path, log_path, strlen(log_path) + 1);
	} else {
	    opts.log_path = NULL;
	}


	/* set listening address */
        if(address != NULL) {
	    opts.address = calloc(strlen(address) + 1, sizeof(address));

            if(opts.address == NULL) {
		perror("calloc");
	        exit(EXIT_FAILURE);
	    }

            strlcpy(opts.address, address, strlen(address) + 1);

	} else {
	    opts.address = NULL;
	}

	/* set listening port */
	opts.port = port;
	
	/* set web root directory */
	if(optind >= argc) {
	    usage();
	}

	directory = argv[optind];

	opts.directory = calloc(strlen(directory) + 1, sizeof(char));

	if(opts.directory == NULL) {
	    perror("calloc");
	    exit(EXIT_FAILURE);
	}

	strlcpy(opts.directory, directory, strlen(directory) + 1);

	return opts;
}

int main(int argc, char** argv) {
    struct options opts = parseoptions(argc, argv);

    return EXIT_SUCCESS;
}

