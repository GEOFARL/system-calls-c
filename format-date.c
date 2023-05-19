#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    printf("Usage: %s <date>\n", argv[0]);
    exit(1);
  }

  char *input_date = argv[1];
  char output_date[11]; // Format: yyyy-mm-dd

  // Convert the month abbreviation to numerical value
  if (strncmp(input_date + 3, "Jan", 3) == 0)
  {
    strncpy(output_date + 5, "01", 2);
  }
  else if (strncmp(input_date + 3, "Feb", 3) == 0)
  {
    strncpy(output_date + 5, "02", 2);
  }
  else if (strncmp(input_date + 3, "Mar", 3) == 0)
  {
    strncpy(output_date + 5, "03", 2);
  }
  else if (strncmp(input_date + 3, "Apr", 3) == 0)
  {
    strncpy(output_date + 5, "04", 2);
  }
  else if (strncmp(input_date + 3, "May", 3) == 0)
  {
    strncpy(output_date + 5, "05", 2);
  }
  else if (strncmp(input_date + 3, "Jun", 3) == 0)
  {
    strncpy(output_date + 5, "06", 2);
  }
  else if (strncmp(input_date + 3, "Jul", 3) == 0)
  {
    strncpy(output_date + 5, "07", 2);
  }
  else if (strncmp(input_date + 3, "Aug", 3) == 0)
  {
    strncpy(output_date + 5, "08", 2);
  }
  else if (strncmp(input_date + 3, "Sep", 3) == 0)
  {
    strncpy(output_date + 5, "09", 2);
  }
  else if (strncmp(input_date + 3, "Oct", 3) == 0)
  {
    strncpy(output_date + 5, "10", 2);
  }
  else if (strncmp(input_date + 3, "Nov", 3) == 0)
  {
    strncpy(output_date + 5, "11", 2);
  }
  else if (strncmp(input_date + 3, "Dec", 3) == 0)
  {
    strncpy(output_date + 5, "12", 2);
  }
  else
  {
    printf("Invalid month abbreviation\n");
    exit(1);
  }

  // Rearrange the date components in the desired order
  strncpy(output_date, input_date + 7, 4);
  strncpy(output_date + 8, input_date + 0, 2);
  output_date[4] = '-';
  output_date[7] = '-';

  printf("%s\n", output_date);

  return 0;
}
