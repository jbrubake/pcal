/*
 * convert Pcal data files: 8-bit character => \xxx octal escape
 *
 *	Usage: cvt8to7 <infile> <outfile>
 *
 *	(where outfile may be the same as infile)
 */

#include <stdio.h>
#include <ctype.h>

#define MAXLINE 256

main(argc, argv)
	char **argv;
{
	FILE *ifp, *ofp;
	char buf[MAXLINE], obuf[MAXLINE], *ifile, *ofile, tmpfile[30];
	int c;

	/* check for correct number of arguments */
	if (argc != 3) {
		fprintf(stderr, "Usage: %s <infile> <outfile>\n", argv[0]);
		exit(1);

	}

	/* set up names for input, output, and temp files */
	ifile = argv[1];
	ofile = argv[2];
	sprintf(tmpfile, "/tmp/%d.xxx", getpid());

	/* open input file; create temp file */
	if ((ifp = fopen(ifile, "r")) == NULL) {
		fprintf(stderr, "can't open %s\n", ifile);
		exit(1);
	}
	if ((ofp = fopen(tmpfile, "w")) == NULL) {
		fprintf(stderr, "can't open temp file %s\n", tmpfile);
		fclose(ifp);
		exit(1);
	}

	/* copy input file to temp file, escaping nonprinting characters */
	while ((c = getc(ifp)) != EOF) {
		if (isascii(c) && isprint(c))
			putc(c, ofp);
		else
			fprintf(ofp, "\\%03o", c & 0377);
	}
	fclose(ifp);
	fclose(ofp);

	/* copy temp file to output file */
	ifp = fopen(tmpfile, "r");	/* assumed OK */
	if ((ofp = fopen(ofile, "w")) == NULL) {
		fprintf(stderr, "can't open output file %s\n", ofile);
		fprintf(stderr, "temporary file is in %s\n", tmpfile);
		fclose(ifp);
		exit(1);
	}

	while ((c = getc(ifp)) != EOF)
		putc(c, ofp);

	/* clean up and exit */
	unlink(tmpfile);
	exit(0);
}
