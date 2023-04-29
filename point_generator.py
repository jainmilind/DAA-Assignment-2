from random import randint
import sys
n = int(sys.argv[1])
x = randint(-1e3, 1e3)
y = randint(-1e3, 1e3)
print(n)
for i in range(n):
    print(x, y)
    y += randint(-1e1, 1e1)
    x += randint(1, 1e1)
# this is Cost
print(sys.argv[2])