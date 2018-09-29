// This program will search for jpeg files on a memcard after they've been deleted.

#include <stdio.h>
int main(int argc, char *argv[])
{
    // Check usage.
    if (argc != 2)
    {
        printf("Usage is ./recover <filename>");
        return 2;
    }
    // Declare buffer size to read from memcard in 512-byte chunks.
    const int BUFFER_SIZE = 512;
    unsigned char buffer[BUFFER_SIZE];

    // Open the memcard file.
    FILE *memcard = fopen(argv[1], "r");

    // Check to make sure memcard exists.
    if (memcard == NULL)
    {
        printf("File does not exist.\n");
        return 1;
    }
    // Declare filename for opening in following function.
    char filename[8];
    int counter = 0;
    FILE *img = NULL;

    // Begin reading from memcard.
    while (fread(buffer, BUFFER_SIZE, 1, memcard) == 1)
    {
        // Conditions to check for beginning of jpeg file.
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // If there's already a file, close it.
            if (img != NULL)
            {
                fclose(img);
                counter ++;
            }

            sprintf(filename, "%03i.jpg", counter);
            img = fopen(filename, "a");
        }

        // Then write the buffer to the file.
        if (img != NULL)
        {
            fwrite(buffer, BUFFER_SIZE, 1, img);
        }
    }

    // Once EOF is reached, close the last file and end.
    fclose(img);
    fclose(memcard);
    return 0;
}