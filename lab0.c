#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

void sigFoo(int signum)
{
	perror("Caught segmentation fault. Errno:");
	exit(3);
}

int main(int argc, char *argv[ ])
{
    int c;

  	static struct option long_options[] =
    {
      {"segfault",     no_argument,       0, 's'},
      {"catch",  	   no_argument,       0, 'c'},
      {"input",  	   required_argument, 0, 'i'},
      {"output",  	   required_argument, 0, 'o'},
      {0, 0, 0, 0}
    };

	int option_index = 0;

	char *input = NULL;
	char *output = NULL;
	int seg = 0;
	int catch = 0;

    while((c = getopt_long (argc, argv, "sci:o:", long_options, &option_index)) != -1)
    {
        switch(c)
        {
        	case 's':
        		seg = 1;
        		break;
        	case 'c':
        		catch = 1;
        		break;
        	case 'i':
        		input = optarg;
        		break;
        	case 'o':
        		output = optarg;
        		break;
        	default:
        		break;
        }
    }


    if (input)
    {
    	int i_fd = open(input, O_RDONLY);
		if (i_fd >= 0) 
		{
			close(0);
			dup(i_fd);
			close(i_fd);
		}
		else
		{
			perror("Could not open file. Errno: ");
			exit(1);
		}
    }

    if (output)
    {
    	int o_fd = creat(output, 0666);
		if (o_fd >= 0) 
		{
			close(1);
			dup(o_fd);
			close(o_fd);
		}
		else
		{
			perror("Could not create file. Errno: ");
			exit(2);
		}
    }

    if (catch)
    {
    	signal(SIGSEGV, sigFoo);
    }

	char* buf;
    if (seg)
    {
    	buf = NULL;
    	char ch = *buf;
    }
    else
    {
  		buf = (char*) malloc(sizeof(char));
    }

	int errCheck = read(0, buf, 1);

	while(errCheck > 0) 
	{
		write(1, buf, 1);
		errCheck = read(0, buf, 1);
	}

	free(buf);

	if (errCheck < 0)
	{
		perror("Read/Write Error. Errno: ");
		exit(-1);
	}

	exit(0);

}
