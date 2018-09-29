from cs50 import get_string
import sys

def translate(c, cipher):
    if c.isupper():
        return chr(((ord(c) - ord('A') + cipher) % 26) + ord('A'))
    elif c.islower():
        return chr(((ord(c) - ord('a') + cipher) % 26) + ord('a'))
    elif not c.isalpha():
        return c


def main():

    if len(sys.argv) != 2:
        print("Correct usage includes one command-line argument.")
        exit(1)

    cipher = int(sys.argv[1])

    message_in = get_string("Enter the message you'd like to cipher: ")
    message_out = ""

    for c in message_in:
        message_out += translate(c, cipher)

    print("{}".format(message_out))

if __name__ == "__main__":
    main()