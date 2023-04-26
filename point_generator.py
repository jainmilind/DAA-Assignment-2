import random
import sys
n = int(sys.argv[1])
m = int(sys.argv[2])
print(n)
arr1 = []
for i in range(n):
    arr2 = []
    arr2.append(random.randint(-m,m))
    arr2.append(random.randint(-m,m))
    arr1.append(arr2)
for x in arr1:
    print(*x)
print(m//3)