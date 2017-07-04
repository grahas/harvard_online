import cs50

def main():
    change = 0
    num_coins = 0
    
    while True:
        print("How much change is owed? ", end="")
        change = cs50.get_float()
        change = change * 100
        change = round(change)
        if change > 0:
            break
    while True:
        if change < 25:
            break        
        change -= 25
        num_coins += 1
    while True:
        if change <= 10:
            break        
        change -= 10
        num_coins += 1
    while True:
        if change <= 5:
            break
        change -= 5
        num_coins += 1
    while True:
        if change <= 0:
            break
        change -= 1
        num_coins += 1
    print(num_coins)
if __name__ == "__main__":
    main()