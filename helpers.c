#include <string.h>
#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

// This function measures whether a note on a line is a rest or not. If it's a rest, return 1. Otherwise, return 0.
_Bool is_rest(char * line)
{
	if (!isalpha(line[0]))
    {
		return 1;
	}
    else
	{
        return 0;
	}
}

// This function returns an integer equal to the duration of a note in 16ths.
int duration(char * line)
{
        const float BASE = (1/8);
        int numerator = atoi(&line[0]);
        int denominator = atoi(&line[2]);

		return numerator / denominator / BASE;
}

// This function reads a note from a string and translates it into a frequency to be passed to the main function.
int frequency(char * line)
{
    int note;
    int accidental;
    int octave;
    const int MID_A = 440;

        switch(line[0])
        {
            case 'C'  :
            note = -9;
            break;

            case 'D'  :
            note = -7;
            break;

            case 'E'  :
            note = -5;
            break;

            case 'F'  :
            note = -4;
            break;

            case 'G'  :
            note = -2;
            break;

            case 'A'  :
            note = 0;
            break;

            case 'B'  :
            note = 2;
            break;
        }

    if (line[1] == '#')
    {
        accidental = 1;
        octave = line[2];
    }
    else if (line[1] == 'b')
    {
        accidental = -1;
        octave = line[2];
    }
    else
    {
        accidental = 0;
        octave = line[1];
    }

    int diff = (accidental + ((octave - 3) * 12) + note);

    if (diff > 0)
    {
        return MID_A * (2 ^ (diff / 12));
    }
    else if (diff < 0)
    {
        return MID_A / (2 ^ (diff / 12));
    }
    else return MID_A;
}