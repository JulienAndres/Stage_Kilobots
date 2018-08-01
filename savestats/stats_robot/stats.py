import numpy as np
import matplotlib.pyplot as plt
#lecture

NB_ROBOT=100

test=[]
for i in range(NB_ROBOT):
    test.append(open(str(i),"r"))
    
    
#========================================================================================   
#STATISTIQUE SUR GENOME
robot=[[] for i in range(NB_ROBOT)]
for i in range(59):
    f=0
    for file in test:
        tmp=file.readline()
        robot[f].append(int(tmp))
        f+=1

#toRet=[]
#yep=dict()
#for i in range(59):
#    tmp=[j[i] for j in robot]
#    final={}.fromkeys(set(tmp),0)
#    for k in tmp:
#        final[k]+=1
#    if -1 in final.keys() : del final[-1]
#    toRet.append(len(final))
#
#print toRet

#ils tournent pour rester pres de la meme personne. 
#=> ils restent donc avec une probabilite eleve
#1 boxplot par generation


a=[[i[j] for i in robot] for j in range(59)]

for i in range(len(a)):
    for j in range(len(a[i])):
        a[i][j]/=32727.0
    
plt.xticks()

fig, ax = plt.subplots()
plt.boxplot(a,showmeans=True)
plt.xticks([i for i in range(0,60,5)],np.linspace(0,60,13,dtype=int))
plt.show()
    
plt.boxplot(a[25:],showmeans=True)
plt.xticks([i for i in range(0,60-25,5)],np.linspace(25,60,8,dtype=int))

plt.show()