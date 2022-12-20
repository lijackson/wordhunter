

def main():
    boards = []
    with open("resources/boards.txt","r") as f:
        boards = f.read().split('\n')
    
    freq = {}
    for board in boards:
        for char in board:
            if char not in freq:
                freq[char] = 0
            freq[char] += 1
    
    letters = list("ABCDEFGHIJKLMNOPQRSTUVWXYZ")

    def cmp(c):
        return 0 if c not in freq else freq[c]

    letters.sort(key=cmp)

    for c in letters:
        if c not in freq:
            print(f"{c} : 0")
        else:
            print(f"{c} : {freq[c]/(0.16*len(boards))}")


if __name__ == "__main__":
    main()