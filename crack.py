import crypt
import cs50
import sys
def main():

# Declare strings from user input (the hash to match,
# the salt, and the 15-char word to run through crypt)

    paesh = sys.argv[1]
    word = sys.argv[1][2:len(sys.argv[1])]
    salt = sys.argv[1][0:2]


    #Check for valid input.
    if len(sys.argv) != 2:
        print("Please provide a valid hash at command-line.")

    #Declare list of characters to cycle through. (Only alpha)
    letters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"


    # Cycle through all 1-letter passwords
    for i in letters:
        check = i
        if crypt.crypt(check, salt) == paesh:
            print("Cracked! Your password is {}.".format(check))
            return 0

        #Cycle through all 2-letter passwords.
        for j in letters:
            check = i + j
            if crypt.crypt(check, salt) == paesh:
                print("Cracked! Your password is {}.".format(check))
                return 0

            #Cycle through all 3-letter passwords.
            for k in letters:
                check = i + j + k
                if crypt.crypt(check, salt) == paesh:
                    print("Cracked! Your password is {}.".format(check))
                    return 0

                #Cycle through all 4-letter passwords.
                for l in letters:
                    check = i + j + k + l
                    if crypt.crypt(check, salt) == paesh:
                        print("Cracked! Your password is {}.".format(check))
                        return 0

    #IF password not found, return error message.
    print("Whoops, something went horribly wrong.")
    return 1

if __name__ == "__main__":
    main()