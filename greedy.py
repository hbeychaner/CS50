from cs50 import get_float

while True:
    payment = get_float("Please enter an amount of change: ")
    if payment >= 0.01 and payment <= 1:
        break

coins = 0
change = payment * 100

coins += change // 25
change %= 25
coins += change // 10
change %= 10
coins += change // 5
change %= 5
coins += change
print("You would need {} coins in change.".format((int(round(coins)))))