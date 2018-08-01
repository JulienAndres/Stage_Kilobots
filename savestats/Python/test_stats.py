import json
import matplotlib.pyplot as plt
import matplotlib as mpl
import numpy as np

def discrete_cmap(N, base_cmap=None):
    """Create an N-bin discrete colormap from the specified input map"""

    # Note that if base_cmap is a string or None, you can simply do
    #    return plt.cm.get_cmap(base_cmap, N)
    # The following works for string, None, or a colormap instance:

    base = plt.cm.get_cmap(base_cmap)
    color_list = base(np.linspace(0, 1, N))
    cmap_name = base.name + str(N)
    return base.from_list(cmap_name, color_list, N)
    
def flaten(s):
    try:
        iter(s)
    except TypeError:
        yield s
    else:
     for elem in s:
         for subelem in flaten(elem):
             yield subelem
             
    
#cmap = mpl.colors.ListedColormap(['red', 'green', 'blue','cyan'])
#
#c=discrete_cmap(3,cmap)

with open("current") as f:
    data=json.load(f)
#    data=data['bot_states']
    
print len(data)
res=[[0 for _ in range(51)] for _ in range(51)]
for iteration in data:
    iteration=iteration['bot_states']
    for robot in iteration:
        x=int((robot['x_position']+500) / 20)
        y=int((robot['y_position']+500) / 20)
        try:
            res[x][y]+=1
        except IndexError:
            print x,y
            raise IndexError

tt=[]
for i in res:
    for j in i:
        if j not in tt:
            tt.append(j)

tt.sort()
c=discrete_cmap(len(tt),'gray')


cmap = plt.cm.jet
# extract all colors from the .jet map
cmaplist = [cmap(i) for i in range(cmap.N)]
# force the first color entry to be grey
cmaplist[0] = (.5,.5,.5,1.0)
# create the new map
cmap = cmap.from_list('Custom cmap', cmaplist, cmap.N)

# define the bins and normalize
bounds = np.linspace(0,30,21)
norm = mpl.colors.BoundaryNorm(bounds, cmap.N)

fig, ax = plt.subplots(1,1, figsize=(6,6))

ax.imshow(res,cmap=cmap,norm=norm)
ax2 = fig.add_axes([0.95, 0.1, 0.03, 0.8])
cb = mpl.colorbar.ColorbarBase(ax2, cmap=cmap, norm=norm, spacing='proportional'   , ticks=bounds, boundaries=bounds, format='%1i')

plt.show()


a=flaten(res)
res2=[]
for i in a:
    res2.append(i)
plt.boxplot(res2)
plt.show()
print np.mean(res2),np.max(res2),np.min(res2),res2.count(0)