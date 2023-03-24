import numpy as np
from enum import IntEnum


class Score(IntEnum):
    MATCH = 5       # match
    MISMATCH = -3   # substitution
    GAP = -4        # deletion/insertion


class Traceback(IntEnum):
    STOP = 0
    LEFT = 1
    UP = 2
    DIAGONAL = 3


def smith_waterman(seq1, seq2):
    # calculate the row and column values of the matrices
    row = len(seq1) + 1
    column = len(seq2) + 1

    # initialize the matrices according to the lengths of the sequences
    matrix = np.zeros((row, column), dtype=int)
    traceback_matrix = np.zeros((row, column), dtype=int)

    # initialize max score and max score index
    max_score = -1
    max_score_index = (-1, -1)

    for i in range(1, row):
        for j in range(1, column):
            # calculate the insertion/deletion, substitution and match scores
            vertical_score = matrix[i - 1, j] + Score.GAP
            horizontal_score = matrix[i, j - 1] + Score.GAP
            diagonal_score = matrix[i - 1, j - 1] + (Score.MATCH if seq1[i - 1] == seq2[j - 1] else Score.MISMATCH)

            # take the max of the calculated scores
            matrix[i, j] = max(0, vertical_score, horizontal_score, diagonal_score)

            # set up the traceback as each cell being iterated
            traceback_matrix[i, j] = Traceback.STOP if matrix[i, j] == 0 else \
                                     Traceback.UP if matrix[i, j] == vertical_score else \
                                     Traceback.LEFT if matrix[i, j] == horizontal_score else \
                                     Traceback.DIAGONAL

            # find the max score and its index
            if matrix[i, j] >= max_score:
                max_score = matrix[i, j]
                max_score_index = (i, j)

    # initialize the result sequences
    res_seq1 = []
    res_seq2 = []
    max_row, max_col = max_score_index

    # traceback the sequence
    while traceback_matrix[max_row, max_col] != Traceback.STOP:
        if traceback_matrix[max_row, max_col] == Traceback.DIAGONAL:
            res_seq1.append(seq1[max_row - 1])
            res_seq2.append(seq2[max_col - 1])
            max_row -= 1
            max_col -= 1
        elif traceback_matrix[max_row, max_col] == Traceback.LEFT:
            res_seq1.append("-")
            res_seq2.append(seq2[max_col - 1])
            max_col -= 1
        else:
            res_seq1.append(seq1[max_row - 1])
            res_seq2.append("-")
            max_row -= 1

    res_seq1.reverse()
    res_seq2.reverse()
    return res_seq1, res_seq2, matrix, max_score


with open("./txts/4_line_added.txt", "r", encoding="utf8") as file:
    sequence1 = file.read()

with open("./txts/19_line_added.txt", "r", encoding="utf8") as file:
    sequence2 = file.read()

# FOR TESTING PURPOSES
sequence1 = "Ali topu attı"
sequence2 = "Ali topu Ahmete attı"

sequence1 = sequence1.split()
sequence2 = sequence2.split()

result_seq1, result_seq2, result_matrix, result_max_score = smith_waterman(sequence1, sequence2)
print("-----Sequence 1-----\n" + " ".join(result_seq1) + "\n")
print("-----Sequence 2-----\n" + " ".join(result_seq2) + "\n")
print("-------Matrix-------")
print(result_matrix )
print("\n-----Max Score------\n", result_max_score)

