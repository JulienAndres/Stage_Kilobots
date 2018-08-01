import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
#lecture

def remove_item(mat, item):
    for lst in mat:
        counter_item = lst.count(item)
        for _ in range(counter_item):
            lst.remove(item)
    return mat
    
    
def apply_pheno(mat):
    for lst in mat:
        for i in range(len(lst)):
            try:
                lst[i]=(1.0/lst[i])*1/4
            except ZeroDivisionError:
                lst[i]=(1.0/(1.0/32767))*1/4
                
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

remove_item(a,-1)

for i in range(len(a)):
    for j in range(len(a[i])):
        a[i][j]/=32767.0
    
plt.xticks()

#fig, ax = plt.subplots()
plt.boxplot(a,showmeans=True)
plt.xticks([i for i in range(0,60,5)],np.linspace(0,60,13,dtype=int))
plt.show()
    
plt.boxplot(a[25:],showmeans=True)
plt.xticks([i for i in range(0,60-25,5)],np.linspace(25,60,8,dtype=int))

plt.show()

sns.set_style("whitegrid")
ygen=[a[1],a[5],a[30],a[58]]
xticks=["0","30","58"]
ax = sns.swarmplot(data=ygen)
#ax.set_xtickslabels(xticks)




plt.show()

apply_pheno(a)
plt.boxplot(a,showmeans=True,showfliers=False)
plt.xticks([i for i in range(0,60,5)],np.linspace(0,60,13,dtype=int))
plt.show()