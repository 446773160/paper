multiplicands = [
    2, -4, 6, -8, 12, -23, 45,
    -67, 123, -345, 567, -789, 1234,
    -3456, 5678, -7890, 987
]

multipliers = [
    3, 5, -7, -9, 15, 34, -56,
    -78, 234, 456, -678, -890, 2345,
    4567, -6789, -8901, -1234
]


for i in range(len(multiplicands)):
    print(f"{multiplicands[i]} × {multipliers[i]}")
