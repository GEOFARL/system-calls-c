#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define MAX_LINE_LENGTH 256

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    printf("Usage: %s <log filename>\n", argv[0]);
    exit(1);
  }

  char *filename = argv[1];

  // Create a pipe to pass data between processes
  int pipefd[2];
  if (pipe(pipefd) == -1)
  {
    perror("pipe");
    exit(1);
  }

  // Fork a child process to execute shell commands
  pid_t pid = fork();
  if (pid == -1)
  {
    perror("fork");
    exit(1);
  }

  if (pid == 0) // Child process
  {
    // Open the output file for writing
    FILE *output_file = fopen("unique_hosts.txt", "w");
    if (output_file == NULL)
    {
      perror("fopen");
      exit(1);
    }

    // Create a command to execute in a pipe
    char command[256];
    snprintf(command, sizeof(command),
             "cat %s | awk '{print substr($4, 2), $1}' | awk '{print substr($1, 0, 12), $2}' | sort -u | awk '{print substr($1, 1, length($1)-1)}' | uniq -c | sort -rn",
             filename);

    // Open a pipe and execute the command
    FILE *pipe = popen(command, "r");
    if (pipe == NULL)
    {
      perror("popen");
      exit(1);
    }

    // Read from the pipe and write to the output file
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), pipe) != NULL)
    {
      if (fputs(line, output_file) == EOF)
      {
        perror("fputs");
        exit(1);
      }
    }

    // Close the pipe and the output file
    pclose(pipe);
    fclose(output_file);
  }
  else // Parent process
  {
    // // Wait for the child process to exit
    int status;
    waitpid(pid, &status, 0);

    // Calculate the total number of hosts for all dates
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
      perror("open");
      exit(1);
    }
    int total = 0;
    int ch;
    while ((ch = fgetc(file)) != EOF)
    {
      if (ch == '\n')
      {
        total += 1;
      }
    }

    fclose(file);

    // Open the input file for reading
    int input_fd = open("unique_hosts.txt", O_RDONLY);
    if (input_fd == -1)
    {
      perror("open");
      exit(1);
    }

    // Duplicate the input file descriptor to the standard input
    dup2(input_fd, STDIN_FILENO);

    // Close the input file descriptor
    close(input_fd);

    // Calculate the percentage of unique hosts for each date and output the result
    char line[MAX_LINE_LENGTH];
    int count = 0;
    while (fgets(line, sizeof(line), stdin) != NULL)
    {
      char date[MAX_LINE_LENGTH];
      int unique;
      sscanf(line, "%d %s", &unique, date);

      char command[256];
      snprintf(command, sizeof(command),
               "./format-date %s",
               date);
      FILE *program_output = popen(command, "r");
      if (program_output == NULL)
      {
        perror("popen");
        exit(1);
      }

      char output_date[MAX_LINE_LENGTH];
      if (fgets(output_date, sizeof(output_date), program_output))
      {
        // Remove the trailing new line character
        output_date[strcspn(output_date, "\n")] = '\0';

        // Calculate the percentage
        float percentage = (float)unique / total * 100.0;
        // Output the result
        printf("%s - %d - %.2f%%\n", output_date, unique, percentage);
      }

      pclose(program_output);

      count += 1;
      if (count == 10)
      {
        break;
      }
    }
    // Remove the temporary file
    unlink("unique_hosts.txt");
  }
  return 0;
}