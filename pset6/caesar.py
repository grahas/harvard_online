import sys
import cs50


def shift_low(letter, key):
    return chr(((ord(letter) + key - ord('a')) % 26) + ord('a'))
    
    
def shift_up(letter, key):
    return chr(((ord(letter) + key - ord('A')) % 26) + ord('A'))
    
    
def do_cipher(letter, key):
    if not letter.isalpha():
        return letter
        
    # charachter by key, preserving case
    if letter.islower():
        return shift_low(letter, key)     
        
    #shift charachter by key, preserving case    
    if letter.isupper():
        return shift_up(letter, key)        


    
def main():
    if len(sys.argv) != 2:
        print("Error, inccorect amount of comand line arguments")
        return 1

    key          = int(sys.argv[1])
    cipher_txt   = cs50.get_string()
    decipher_txt = [do_cipher(i, key) for i in cipher_txt]
        
    print('key: {}, cipher text: {}, deciphered text: {}'.format(key, cipher_txt, decipher_txt))

if __name__ == "__main__":
    main()