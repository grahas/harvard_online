from nltk.tokenize import TweetTokenizer
import sys

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""
        # TODO
        #__init__ loads positive and negative words into memory in such a way that analyze can access them.
        self.positive = []
        self.negative = []
        with open(positives, "r") as f:
            for line in f:
                if f.readline().startswith(';') == False:
                    self.positive.append(line.strip())
        print(self.positive)
        with open(negatives, "r") as f:
            for line in f:
                if f.readline().startswith(';') == False:
                    self.negative.append(line.strip())
                    #print(line.strip())
        print(self.negative)
                
    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""

        # TODO
        #analyze analyzes the sentiment of text, returning a positive score if text is more positive than negative, a negative score
        #if text is more negative than positive, and 0 otherwise, whereby that score is computed as follows:
        #assign each word in text a value: 1 if the word is in positives, -1 if the word is in negatives, and 0 otherwise
        #consider the sum of those values to be the entire text’s score
        score = 0
        tknzr = TweetTokenizer()
        text = text.lower()
        print(self.negative)
        for each_word in tknzr.tokenize(text):
            if each_word in self.positive:
                score += 1

        for each_word in self.negative:
            if each_word in tknzr.tokenize(text):
                score += -1

        if score > 0:
            return 1
        elif score < 0:
            return -1
        else:
            return 0
            
if __name__ == "__main__":
    word = str(sys.argv[1:])
    bleh = Analyzer("positive-words.txt", "negative-words.txt").analyze(word)
    print(bleh)
    
