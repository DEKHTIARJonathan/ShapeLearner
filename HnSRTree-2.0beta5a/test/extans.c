#include <stdio.h>

#include <string.h>



int

main(int argc, char *argv[])

{

    char buffer[16384];

    int inProperties = 0, inTimesStats = 0, inSRTreeStats = 0;



    while ( fgets(buffer, sizeof(buffer), stdin) != NULL ) {

	if ( inProperties ) {

	    if ( strcmp(buffer, "]\n") == 0 ) {

		inProperties = 0;

	    }

	}

	else if ( inTimesStats ) {

	    if ( strcmp(buffer, "]\n") == 0 ) {

		inTimesStats = 0;

	    }

	}

	else if ( inSRTreeStats ) {

	    if ( strcmp(buffer, "]\n") == 0 ) {

		inSRTreeStats = 0;

	    }

	}

	else if ( strcmp(buffer, "HnPropertiesSt[\n") == 0 ) {

	    inProperties = 1;

	}

	else if ( strcmp(buffer, "HnTimesStatsSt[\n") == 0 ) {

	    inTimesStats = 1;

	}

	else if ( strcmp(buffer, "HnSRTreeStatsSt[\n") == 0 ) {

	    inSRTreeStats = 1;

	}

	else if ( strcmp(buffer, "**** Processing Cost ****\n") != 0 &&

		  strncmp(buffer, "Searching ", 10) != 0 ) {

	    char *ptr;



	    if ( (ptr = strrchr(buffer, ':')) != NULL ) {

		strcpy(ptr, "\n");

	    }



	    printf("%s", buffer);

	}

    }



    return 0;

}

