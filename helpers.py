import cs50
from nltk.tokenize import sent_tokenize

# This function will split files into lines in a list to be compared.
def lines(a, b):
    left = a.sent_tokenize("\n")
    right = b.split("\n")
    # Common array to be used for lines in common.
    common = []
    for i in left:
        for j in right:
            # Compare lines, don't add to common if line exists.
            if left[i] == right[j] and left[i] not in common:
                common.append(left[i])

    print("The following lines were found to be identical between the two inputs:")
    for k in common:
        print(common[k])

# This function breaks files into sentences and compares each sentence.
def sentences(a, b):
    left = a.sent_tokenize
    right = b.sent_tokenize
    # Common array to be used for lines in common.
    common = []
    for i in left:
        for j in right:
            # Add common lines to common, do not add if line exists.
            if left[i] == right[j] and left[i] not in common:
                common.append(left[i])

# This funciton compares all substrings of size n.
def substrings(a, b, n):

    # Init lists to store substrings.
    left = []
    right = []

    # Initialize list of substrings to compare.
    for i in a - n:
        left[i] = a[i:(i+n)]
        right[i] = b[i:(i+n)]

    # Common array to be used for lines in common.
    common = []
    for i in left:
        for j in right:

            # Compare substrings, do not add if substring exists already.
            if left[i] == right[j] and left[i] not in common:
                common.append(left[i])