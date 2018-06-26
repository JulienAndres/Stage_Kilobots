import json
import matplotlib.pyplot as plt

with open("test") as f :
    datastore=json.load(f)

d=[[0 for i in range(1100)] for j in range(1100)]
    
for iteration in datastore:
    for robot in iteration['bot_states']:     
#            print int(round(robot['x_position'])),int(round(robot['y_position']))
            d[int(round(robot['x_position']))+500][int(round(robot['y_position']))+500]+=1
#            print d[int(round(robot['x_position']))][int(round(robot['y_position']))]
            

fig=plt.figure()
#plt.imshow(d,cmap='gray_r')
a=plt.imshow(d, interpolation='none', cmap='gray_r' )
plt.colorbar()
a.save()
cpt=0
for i in d:
    for j in i:
        cpt+=j

print cpt