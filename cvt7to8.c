/*
 * convert Pcal data files: \xxx octal escape => true 8-bit character
 *
 *	Usage: cvt7to8 <infile> <outfile>
 *
 *	(where outfile may be the same as infile)
 */

#include <stdio.h>
#include <sys/stat.h>

#define MAXLINE 256
#define isodigit(c)	((c) >= '0' && (c) <= '7')

main(argc, argv)
	char **argv;
{
	FILE *ifp, *ofp;
	char *buf, *bufend, *p, *ifile, *ofile, tmpfile[30];
	struct stat sbuf;
	int c, nbytes;

	/* check for correct number of arguments */
	if (argc != 3) {
		fprintf(stderr, "Usage: %s <infile> <outfile>\n", argv[0]);
		exit(1);
	}

	/* set up names for input, output, and temp files */
	ifile = argv[1];
	ofile = argv[2];
	sprintf(tmpfile, "/tmp/%d.xxx", getpid());

	/* open input file and copy into malloc'd buffer */
	if ((ifp = fopen(ifile, "r")) == NULL) {
		fprintf(stderr, "can't open %s\n", ifile);
		exit(1);
	}
	stat(ifile, &sbuf);
	if ((buf = malloc(nbytes = sbuf.st_size)) == NULL) {
		fprintf(stderr, "can't malloc %d bytes for file %s\n", nbytes,
			ifile);
		fclose(ifp);
		exit(1);
	}
	if (fread(buf, 1, nbytes, ifp) != nbytes) {
		fprintf(stderr, "can't read file %s\n", ifile);
		fclose(ifp);
		exit(1);
	}

	/* create temp file */
	if ((ofp = fopen(tmpfile, "w")) == NULL) {
		fprintf(stderr, "can't open temp file %s\n", tmpfile);
		fclose(ifp);
		exit(1);
	}

	/* copy input file to temp file, converting octal escapes */
	bufend = (p = buf) + nbytes;

	while (p < bufend) {
		if (p[0] == '\\' && isodigit(p[1])) {
			int nc;
			for (c = 0, nc = 1; nc <= 3 && isodigit(p[nc]); nc++)
				c = c * 8 + (p[nc] - '0');
			putc(c, ofp);
			p += nc;
		} else
			putc(*p++, ofp);
	}

	free(buf);
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
