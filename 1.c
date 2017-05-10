/*For each command line argument, the main process will launch one subprocess.
Each such process will establish if the assigned argument is a director,
file or something else.
Based on the established type the subprocess will return
to the parent process the followings:
- if the argument is a file then it will return the
file size (see st_size from stat).
- if it is a directory then it will return the
number of text files (established using file command) found in this directory
or among his subdirectories. To establish the number of
text files the process will use popen to execute a shell script.
- for cases where the argument is neither a file or a directory it will
return a random generated number between 5 and 15.
The communication between the process and his sub processes will be done
using a private pipe channel.
Before each read or write pipe operation each process will print what is
written or was read from the pipe.*/

#include <dirent.h>
#include <time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	//Test if there are some arguments in the command line
	if (argc < 2)
	{
		printf("Not enough arguments !");
		exit(1);
	}

	for (int i = 1; i < argc; i++)
	{
		//Make a pipe
		int p[2];

		//Create the child
		int f = fork();

		//If the pipe couldn't have been created, we print an error
		if (pipe(p) < 0)
		{
			printf("Pipe error !");
			exit(2);
		}

		//If the fork couldn't have been performed, we prin an error
		if (f < 0)
		{
			printf("Fork error !");
			exit(3);
		}

		//If the child was created
		if (f == 0)
		{
			printf("Argument %d: ", i);
			//We try to open a directory, by this action we will
			//see if the argument it is a directory or not
			DIR *dir = opendir(argv[i]);

			//We try to open a file, by this action we will see
			// if the argument is a file or not
			FILE *aux = fopen(argv[i], "r");

			//In sb we will have the argument
			//We do this for the usage of S_ISDIR and S_ISREG
			struct stat sb;
			stat(argv[i], &sb);

			//If it is a directory
			if (dir)
			{
				//We check it if it directory
				if (S_ISDIR(sb.st_mode))
				{
					//In the file g we will have the name
					//of the directory
					FILE *g;
					//Open for writing
					g = fopen("Procese/Intermediar.txt", "w");
					//Write in the file, the name of directory
					const char *text = argv[i];
					fprintf(g, text);
					//Close the file
					fclose(g);

					//We open the shell program for reading
					FILE *file = popen("Procese/1.sh", "r");
					//Read the commands from the file
					char buffer[1000];
					fgets(buffer, 1000, file);
					int value = 0;
					//Convert string to integer
					sscanf(buffer, "%d", &value);
					//We check if there was any error in writing
					if (write(p[1], &value, sizeof(int)) < 0)
					{
						printf("Writing error !");
						exit(4);
					}
					//If there wasn't errprs, we write in pipe
					write(p[1], &value, sizeof(int));

					//The file it's closed
					pclose(file);
				}
				//The directory it's closed
				closedir(dir);
			}
			else if (aux)
			{
				//Check if it is a reglar file
				if (S_ISREG(sb.st_mode))
				{
					//Get the size of the file
					int size = sb.st_size;
					//Check if there was any errors in writing
					if (write(p[1], &size, sizeof(int)) < 0)
					{
						printf("Writing error !");
						exit(4);
					}
					//If not, we write in pipe
					write(p[1], &size, sizeof(int));
				}
				//Close the file
				fclose(aux);
			}
			else
			{
				//If the file does not exist, here will be
				//generated a random number
				srand(time(NULL));
				int nr = rand() % 100;
				//We take the modulo of the number
				while (nr < 5 || nr > 15)
					nr = rand() % 100;
				//Check if there were any errors in writing
				if (write(p[1], &nr, sizeof(int)) < 0)
				{
					printf("Writing error !");
					exit(4);
				}
				//If not, we will write in pipe
				write(p[1], &nr, sizeof(int));
			}
			close(p[1]);
		}
		//Declare the variable where it will be placed the result
		//from the child
		int x;
		//Check if there were any errors in reading
		if (read(p[0], &x, sizeof(int)) < 0)
		{
			printf("Reading error !");
			exit(5);
		}
		//If not, we read from pipe and print
		read(p[0], &x, sizeof(int));
		printf("%d\n", x);
		//We close the pipe
		close(p[0]);
		close(p[1]);
		wait(0);
	}
	printf("Program finished !");
	exit(0);
	return 0;
}
