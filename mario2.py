from cs50 import get_int

while True:
    n = get_int("Enter an integer between 1 and 20, please: ")
    if n >= 1 and n <= 20:
        break

height = n

for i in range(height):
    print(" " * n, end = "")
    print("#" * (height - n), end = "")
    print("  ", end = "")
    print("#" * (height - n), end = "")
    n = n-1
    print()
