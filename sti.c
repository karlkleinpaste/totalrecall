/*
 * Stuff Terminal Input.
 * Use the TIOCSTI to cram a few command bytes into the terminal,
 * in order to induce a shell to re-manufacture its own environment
 * with directives to find old state.
 */

#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>

main(int argc, char **argv)
{
  char *s;

  if (argc != 2)
    {
      fprintf (stderr, "%s: Usage `%s string-to-be-stuffed'\n", argv[0], argv[0]);
      exit(1);
    }
  for (s = argv[1] ; *s ; s++)
    {
      if (ioctl(fileno(stdin), TIOCSTI, s) < 0)
	{
	  fprintf(stderr, "char <%c>, distance %d\n", *s, s-argv[1]);
	  perror("TIOCSTI");
	  exit(2);
	}
    }
}
