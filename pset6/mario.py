import cs50

def main():
    while True:
        print('Height: ', end='')
        height = int(input())
        if height <= 23 and height >= 0:
            break
    blocks = 2
    spaces = height - 1
    for i in range(height):
        for j in range(spaces):
            print(" ", end="")
        spaces = spaces - 1
        for k in range(blocks):
            print("#", end="")
        blocks = blocks + 1
        print("")

if __name__ == "__main__":
    main()