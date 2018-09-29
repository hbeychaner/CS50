README

This program opens a binary file (a "memory card" with deleted images for recovery), checks each byte sector for a jpg fileheader,
and, if a jpg fileheader is found, the jpg is encapsulated and exported as an image file after recovery.
Usage:
    whodunit <file_name>