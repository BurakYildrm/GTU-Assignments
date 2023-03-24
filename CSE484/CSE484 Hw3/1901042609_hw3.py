import gensim
import os


FILE = "wiki_00"
LINE_NUMBER = 250000
TOP_N = 5


def is_vowel(char):
    """

    :param char: the character to check
    :return: bool depending if the char is a vowel
    """
    vowels = "aeiou"
    return char in vowels


def syllabicate(word):
    """
    :param word: The word to be syllabicated
    :return: The syllabicated list that contains syllabs
    """
    word = word.lower()
    syllabs = []
    syllab = ""
    last_was_vowel = False
    # we don't want the last character

    for pos, char in enumerate(word[:-1]):
        next_is_vowel = is_vowel(word[pos + 1])

        if next_is_vowel and syllab and not (last_was_vowel and is_vowel(char)):
            syllabs.append(syllab)
            syllab = ""

        if char in " .\n\r":
            if syllab:
                syllabs.append(syllab)

            syllabs.append(char)
            syllab = ""
            last_was_vowel = False
            continue

        syllab += char
        last_was_vowel = is_vowel(char)

    if word[-1] == ".":
        syllabs.append(syllab)
        syllabs.append(".")
    else:
        syllab += word[-1]
        syllabs.append(syllab)

    return syllabs


def n_grams(sequence, n):
    result = []
    sentence = []

    if n == 1:
        result.append(sequence)
        return result

    for i in range(len(sequence) - n + 1):
        newline = False
        n_gram = sequence[i]

        if sequence[i] in "\n\r" and sentence:
            result.append(sentence)
            sentence = []
            continue

        for j in range(1, n):
            if sequence[i + j] in "\n\r":
                newline = True

            if not newline:
                n_gram += sequence[i + j]

        if not newline:
            sentence.append(n_gram)

    if sentence:
        result.append(sentence)

    return result


def print_ngram_most_similar(ngram_model, ngram_examples, n):
    for example in ngram_examples:
        output = ngram_model.wv.most_similar(example, topn=TOP_N)
        print(f"{n}-Gram: {example}")
        print("Most similar syllables: ", end="")
        for i, (syllable, score) in enumerate(output):
            print("{}{}\t{}{}".format("\t\t\t\t\t\t" * bool(i != 0), "\' \'" if syllable == " " else syllable, score,
                                      "\n" * bool(i == TOP_N - 1)))


def print_ngram_similarity(ngram_model, ngram_examples):
    it = iter(ngram_examples)
    for example in it:
        next_example = next(it)
        output = ngram_model.wv.similarity(example, next_example)
        print(f"Word 1: {example} Word 2: {next_example} Similarity: {output}")


def analogy(word_a, word_b, word_c, w2v_model):
    result = w2v_model.wv.most_similar(negative=[word_a], positive=[word_b, word_c])
    return result[0][0]


file = open(FILE)
contents = ""

for i, line in enumerate(file):
    if i >= LINE_NUMBER:
        break
    contents += file.readline()


# convert all the letters to lowercase
contents = contents.lower()

# delete all the numbers
contents = ''.join([i for i in contents if not i.isdigit()])

# delete all the punctuation marks but dot
contents = contents.replace(",", "")
contents = contents.replace(";", "")
contents = contents.replace(":", "")
contents = contents.replace("...", "")
contents = contents.replace("?", "")
contents = contents.replace("!", "")
contents = contents.replace("-", "")
contents = contents.replace("/", "")
contents = contents.replace("\\", "")
contents = contents.replace("\"", "")
contents = contents.replace("\'", "")
contents = contents.replace("(", "")
contents = contents.replace(")", "")
contents = contents.replace("[", "")
contents = contents.replace("]", "")

# convert turkish specific characters to english
contents = contents.replace("ç", "c")
contents = contents.replace("ğ", "g")
contents = contents.replace("ı", "i")
contents = contents.replace("ö", "o")
contents = contents.replace("ş", "s")
contents = contents.replace("ü", "u")

syllables = syllabicate(contents)

one_gram = n_grams(syllables, 1)
two_gram = n_grams(syllables, 2)
three_gram = n_grams(syllables, 3)

if not os.path.isfile("./one_gram.model"):
    one_gram_model = gensim.models.Word2Vec(
        sentences=one_gram,
        vector_size=100,
        window=5,
        min_count=1,
        workers=4
    )
    one_gram_model.save("./one_gram.model")
else:
    one_gram_model = gensim.models.Word2Vec.load("./one_gram.model")

if not os.path.isfile("./two_gram.model"):
    two_gram_model = gensim.models.Word2Vec(
        sentences=two_gram,
        vector_size=100,
        window=5,
        min_count=1,
        workers=4
    )
    two_gram_model.save("./two_gram.model")
else:
    two_gram_model = gensim.models.Word2Vec.load("./two_gram.model")

if not os.path.isfile("./three_gram.model"):
    three_gram_model = gensim.models.Word2Vec(
        sentences=three_gram,
        vector_size=100,
        window=5,
        min_count=1,
        workers=4
    )
    three_gram_model.save("./three_gram.model")
else:
    three_gram_model = gensim.models.Word2Vec.load("./three_gram.model")

print("------MOST SIMILAR SYLLABLE------")
print("------1-GRAM------")
one_gram_examples = ["la", "mez", "cik"]
print_ngram_most_similar(one_gram_model, one_gram_examples, 1)

print("------2-GRAM------")
two_gram_examples = ["larim", "tikca", "mezler"]
print_ngram_most_similar(two_gram_model, two_gram_examples, 2)

print("------3-GRAM------")
three_gram_examples = ["madikca", "larini", "liyorken"]
print_ngram_most_similar(three_gram_model, three_gram_examples, 3)

print("\n------SIMILARITY SCORES------")
print("------1-GRAM------")
one_gram_examples = ["la", "le", "ta", "de", "dik", "cik"]
print_ngram_similarity(one_gram_model, one_gram_examples)

print("\n------2-GRAM------")
two_gram_examples = ["lari", "larim", "cakti", "dardi", "daki", "lara"]
print_ngram_similarity(two_gram_model, two_gram_examples)

print("\n------3-GRAM------")
three_gram_examples = ["larini", "lerini", "medigi", "madigi", "dakiler", "diklari"]
print_ngram_similarity(three_gram_model, three_gram_examples)
