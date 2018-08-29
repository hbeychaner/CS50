// Helper functions for music
#include <string.h>
#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <cs50.h>
#include <math.h>
#include "helpers.h"

// Converts a fraction formatted as X/Y to eighths
int duration(string fraction)
{
    const int BASE = (8);
    int numerator = atoi(&fraction[0]);
    int denominator = atoi(&fraction[2]);

	return (numerator * BASE) / denominator;
}

// Calculates frequency (in Hz) of a note
int frequency(string note)
{
    int accidental;
    int octave;
    int steps;
    const int MID_A = 440;

    switch(note[0])
        {
            case 'C'  :
            steps = -9;
            break;

            case 'D'  :
            steps = -7;
            break;

            case 'E'  :
            steps = -5;
            break;

            case 'F'  :
            steps = -4;
            break;

            case 'G'  :
            steps = -2;
            break;

            case 'A'  :
            steps = 0;
            break;

            case 'B'  :
            steps = 2;
            break;
        }

    if (note[1] == '#')
    {
        accidental = 1;
        octave = atoi(&note[2]);
    }
    else if (note[1] == 'b')
    {
        accidental = -1;
        octave = atoi(&note[2]);
    }
    else
    {
        accidental = 0;
        octave = atoi(&note[1]);
    }

    // Defines distance in semitones between A4 and the input.
    int diff = (accidental + ((octave - 4) * 12) + steps);

    // If 0 difference, return A4 frequency. Else, this calculates the frequency and returns it, rounded to the nearest int.
    int frequency = 0;
    float power = (float)diff / 12;
    if (octave >= 0)
    {
        frequency = round(MID_A * pow(2, power));
    }
    else
    {
        frequency = round(MID_A / pow(2, power));
    }

    return frequency;
}

// Determines whether a string represents a rest
bool is_rest(string s)
{
    // Tests whether string has an alpha character. If not, it's empty and therefore a rest.
    if (!isalpha(s[0]))
    {
		return 1;
	}
	// If first character is a letter, return 0 to denote a note.
    else
	{
        return 0;
	}
}