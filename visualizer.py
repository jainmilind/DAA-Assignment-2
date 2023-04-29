import matplotlib.pyplot as plt
## This is a list of all the x-coordinates that needs to be plotted
x_coord_final = list()
## This is a list of all the y-coordinates that needs to be plotted
y_coord_final = list()
## Minimum possible value of X coordinate
minx = 9e18
## Minimum possible value of Y coordinate
miny = 9e18
## Maximum possible value of X coordinate
maxx = -9e18
## Maximum possible value of Y coordinate
maxy = -9e18
with open('input_visualizer.txt') as f:
    ## Number of points in the input
    n = int(next(f))
    ## This is a list of all the x-coordinates of points taken as input
    x_coord_1 = list()
    ## This is a list of all the y-coordinates of points taken as input
    y_coord_1 = list()
    for i in range(n):
        ## x and y coordinate the input point
        x, y = [float(l) for l in next(f).split()]
        x_coord_1.append(x)
        y_coord_1.append(y)
        minx = min(minx, x)
        maxx = max(maxx, x)
        miny = min(miny, y)
        maxy = max(maxy, y)
    x_coord_final.append(x_coord_1)
    y_coord_final.append(y_coord_1)
    ## Number of final segments
    n = int(next(f))
    for i in range(n):
        ## This is a list of all the x-coordinates of the end points of the segment
        x_coord_2 = list()
        ## This is a list of all the y-coordinates of the end points of the segment
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
## matplotlib's objects
fig, ax = plt.subplots()
plt.xlim([minx - 5, maxx + 5])
plt.ylim([miny - 5, maxy + 5])

ax.set_facecolor('#ffff99')
ax.grid(True, which='both')
# ax.axhline(y=0, color='black', linewidth=.5)
# ax.axvline(x=0, color='black', linewidth=.5)

# ax.spines['bottom'].set_position('zero')
# ax.spines['left'].set_position('zero')
plt.plot(x_coord_final[0], y_coord_final[0],'o')
for i in range(1, len(x_coord_final)):
    plt.plot(x_coord_final[i], y_coord_final[i])
plt.show()