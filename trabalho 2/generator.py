from random import *

# Const variables that defines the number of strings generated
size = input("Size of 2d array:\n")

size = int(size)
path='./teste.txt'
file = open(path,'w')
file.write(str(size))
file.write("\n")

size -= 1
index = int(size)

for i in range(size):
    for j in range(index):
        file.write(str(randint(0,3)))
        file.write(" ")
    file.write("\n")
    index -= 1

file.close()