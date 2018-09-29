from cs50 import get_string
import sys

def translate(char, cipher_char):
    if char.isupper():
        return chr(((ord(char) - ord('A') + ord(cipher_char.lower()) - ord('a')) % 26) + ord('A'))
    elif char.islower():
        return chr(((ord(char) - ord('a') + ord(cipher_char.lower()) - ord('a')) % 26) + ord('a'))
    else:
        return char


def main():

    if len(sys.argv) != 2:
        print("Correct usage includes one command-line argument.")
        exit(1)
    if not sys.argv[1].isalpha():
        print("Please provide a valid keyword.")

    cipher = sys.argv[1]

    message_out = ""
    message_in = get_string("What is the message you'd like to encode: ")

    i = 0
    for c in message_in:
        if c.isalpha():
            message_out += translate(c, cipher[i % len(cipher)])
            i += 1
        else:
            message_out += translate(c, cipher[i % len(cipher)])

    print("{}".format(message_out))

if __name__ == "__main__":
    main()