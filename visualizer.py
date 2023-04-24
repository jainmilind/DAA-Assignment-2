import matplotlib.pyplot as plt
x_coord_final = list()
y_coord_final = list()
minx = 9e18
miny = 9e18
maxx = -9e18
maxy = -9e18
with open('input_visualizer.txt') as f:
    n = int(next(f))
    x_coord_1 = list()
    y_coord_1 = list()
    for i in range(n):
        x, y = [float(l) for l in next(f).split()]
        x_coord_1.append(x)
        y_coord_1.append(y)
        minx = min(minx, x)
        maxx = max(maxx, x)
        miny = min(miny, y)
        maxy = max(maxy, y)
    x_coord_final.append(x_coord_1)
    y_coord_final.append(y_coord_1)
    n = int(next(f))
    for i in range(n):
        x_coord_2 = list()
        y_coord_2 = list()
        x1, y1,x2,y2 = [float(l) for l in next(f).split()]
        x_coord_2.append(x1)
        y_coord_2.append(y1)
        x_coord_2.append(x2)
        y_coord_2.append(y2)
        minx = min(minx, x1,x2)
        maxx = max(maxx, x1,x2)
        miny = min(miny, y1,y2)
        maxy = max(maxy, y1,y2)
        x_coord_final.append(x_coord_2)
        y_coord_final.append(y_coord_2)
fig, ax = plt.subplots()
ax.set_facecolor('xkcd:grey')
ax.set_xlim(minx - 10, maxx + 10)
ax.set_ylim(miny - 10, maxy + 10)
ax.spines['bottom'].set_position('zero')
ax.spines['left'].set_position('zero')
plt.plot(x_coord_final[0], y_coord_final[0],'o')
for i in range(1,len(x_coord_final)):
    plt.plot(x_coord_final[i], y_coord_final[i])
plt.show()