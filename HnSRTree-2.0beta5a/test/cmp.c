/*

 * This program is a substitute for the command `fc' of Windows 98.

 * Its exit code does not report whether the given files are different

 * or not.

 */



#include <stdio.h>

#include <stdlib.h>



int

main(int argc, char *argv[])

{

    char *filename1, *filename2;

    FILE *fp1, *fp2;

    int eof1, eof2;



    if ( argc != 3 ) {

	fprintf(stderr, "Usage: %s input1 input2\n", argv[0]);

	exit(2);

    }

    filename1 = argv[1];

    filename2 = argv[2];



    if ( (fp1 = fopen(filename1, "r")) == NULL ) {

	perror(filename1);

	exit(2);

    }



    if ( (fp2 = fopen(filename2, "r")) == NULL ) {

	perror(filename2);

	exit(2);

    }



    eof1 = eof2 = 0;



    for ( ;; ) {

	char buffer1[16384], buffer2[16384];

	int len1, len2;



	if ( fgets(buffer1, sizeof(buffer1), fp1) == NULL ) {

	    if ( ferror(fp1) ) {

		perror(filename1);

		exit(2);

	    }

	    eof1 = 1;

	}

	if ( fgets(buffer2, sizeof(buffer2), fp2) == NULL ) {

	    if ( ferror(fp2) ) {

		perror(filename2);

		exit(2);

	    }

	    eof2 = 1;

	}



	if ( eof1 || eof2 ) {

	    break;

	}



	len1 = strlen(buffer1);

	if ( buffer1[len1 - 1] == '\n' ) {

	    buffer1[len1 - 1] = 0;

	    len1 --;

	}



	len2 = strlen(buffer2);

	if ( buffer2[len2 - 1] == '\n' ) {

	    buffer2[len2 - 1] = 0;

	    len2 --;

	}



	if ( strcmp(buffer1, buffer2) != 0 ) {

	    fclose(fp1);

	    fclose(fp2);

	    printf("***** %s\n%s\n", filename1, buffer1);

	    printf("***** %s\n%s\n", filename2, buffer2);

	    printf("*****\n");

	    return 1;

	}

    }



    fclose(fp1);

    fclose(fp2);



    if ( eof1 && eof2 ) {

	return 0;

    }

    else {

	if ( eof2 ) {

	    printf("%s is longer than %s\n", filename1, filename2);

	}

	else {

	    printf("%s is longer than %s\n", filename2, filename1);

	}

	return 1;

    }

}

