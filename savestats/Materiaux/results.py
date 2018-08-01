import numpy as np
import matplotlib.pyplot as plt
import pylab


decoupe_75=[7.5,
5,
6,
7.4,
7,
7.5,
7,
7,
6.9,
6]
decoupe_70=[7,
7.5,
7,
7.2,
6.1,
6.5,
7,
7.3,
6,
7
]
decoupe_65=[7.2,
7.2,
6,
7,
7,
5.6,
5.5,
6,
6.8,
5.5
]
decoupe_60=[7.5,
7.5,
7.5,
6.6,
7.4,
6.5,
7,
6.8,
7,
6.5
]
decoupe_55=[6.5,
6.5,
6.5,
6.5,
6.5,
6.3,
6.5,
6.5,
6,
6.2,
]
decoupe_50=[0.5,
0.2,
2.4,
2,
1,
2.5,
1,
1.5,
1.7,
0.5
]
#ax2=plt.supplot(111)
print "decoupe avance"
decoupe=[decoupe_50,decoupe_55,decoupe_60,decoupe_65,decoupe_70,decoupe_75]
plt.boxplot(decoupe)
pylab.xticks([i for i in range(7)],[i for i in range(45,80,5)]) #decoupe avance
plt.xlabel("vitesse des moteurs")
plt.ylabel("distance parcourue (cm)")
plt.show()
plt.close()

decoupe_d_75=[0.5,
0.2,
1.5,
0.2,
0.8,
2.2,
2,
2,
1.5,
2
]

decoupe_d_70=[1.4,
3.2,
1,
0.3,
1.5,
1,
1.2,
0.2,
0.9,
0.5
]

decoupe_d_65=[0,
0.2,
1,
0.8,
0.4,
1.2,
1,
0.7,
1.2,
1.50

]

decoupe_d_60=[0,
1.2,
0,
1,
0,
1,
0,
1.2,
1,
1.5,
]

decoupe_d_55=[0.7,
0.2,
0,
0.7,
0.5,
0.7,
1.2,
0.5,
0.6,
0.2
]

decoupe_d_50=[0,
0,
0.5,
0.5,
0,
0.6,
0.5,
0.2,
0,
0
]

print "decoupe decallage"
decoupe_d=[decoupe_d_50,decoupe_d_55,decoupe_d_60,decoupe_d_65,decoupe_d_70,decoupe_d_75]
plt.boxplot(decoupe_d)
pylab.xticks([i for i in range(7)],[i for i in range(45,80,5)]) #decoupe decallage
plt.xlabel("vitesse des moteurs")
plt.ylabel("translation (cm)")
plt.show()
plt.close()

carton_75=[3,
2,
2,
3,
1,
1,
2,
3,
2,
1,
1
]

carton_70=[6.8,
6.5,
7,
6.7,
6,
6.5,
6.7,
6.5,
6.2,
6.3
]

carton_65=[6,
6.2,
6.6,
6.1,
5,
6,
6.1,
5.2,
6,
6
]

carton_60=[6,
6,
6,
6,
6,
5.9,
5.8,
5.8,
6,
5.8
]

carton_55=[5.5,
6.2,
6.3,
6,
5.1,
5.2,
5.5,
5,
5,
4
]

carton_50=[0.1,
0.1,
0,
0.2,
0.1,
0.3,
0.2,
0,
0,
0.1
]

print "carton avance"
carton=[carton_50,carton_55,carton_60,carton_65,carton_70,carton_75]
plt.boxplot(carton)
pylab.xticks([i for i in range(7)],[i for i in range(45,80,5)]) #decoupe decallage
plt.xlabel("vitesse des moteurs")
plt.ylabel("distance parcourue (cm)")
plt.show()
plt.close()




carton_d_75=[3,
2.5,
3,
3.5,
3,
3.5,
3,
1.5,
2.5,
3,
3
]

carton_d_70=[1,
0.7,
0,
0.6,
0.2,
1,
0.2,
0.3,
0.3,
0
]

carton_d_65=[0.5,
0.7,
0.2,
0.4,
1,
0.4,
0.2,
1.1,
0.4,
0.6
]

carton_d_60=[0.7,
1,
0,
0.2,
0.1,
0.2,
0.2,
0,
0.1,
0.2
]

carton_d_55=[0.5,
0.2,
0,
0.2,
0.2,
0.1,
0.2,
0.2,
0.2,
0.3
]

carton_d_50=[0,
0,
0,
0.1,
0.1,
0.1,
0.1,
0,
0,
0
]

print "carton decallage"
carton_d=[carton_d_50,carton_d_55,carton_d_60,carton_d_65,carton_d_70,carton_d_75]
plt.boxplot(carton_d)
pylab.xticks([i for i in range(7)],[i for i in range(45,80,5)]) #decoupe decallage
plt.xlabel("vitesse des moteurs")
plt.ylabel("translation (cm)")
plt.show()
plt.close()


plexi_75=[0.5,
-1,
-2,
-2,
-1,
-1,
0.5,
0,
2.5,
-4
]

plexi_70=[0,
0,
0,
0,
-3,
-1.5,
-3.5,
0,
0,
-2
]

plexi_65=[-2,
-1,
-2,
-2,
0,
0.5,
-2.5,
-1,
-1.7,
0
]

plexi_60=[6,
6,
6,
5,
6.2,
6.3,
6.5,
3,
4.5,
4.5
]

plexi_55=[6,
5.5,
6.1,
6,
6.5,
6,
5,
5.3,
5.5,
5.5
]

plexi_50=[5.1,
5,
5,
4,
4.8,
6.5,
4.7,
4.7,
4.9,
4.3
]

print "plexi avance"
plexi=[plexi_50,plexi_55,plexi_60,plexi_65,plexi_70,plexi_75]
plt.boxplot(plexi)
pylab.xticks([i for i in range(7)],[i for i in range(45,80,5)]) #decoupe decallage
plt.xlabel("vitesse des moteurs")
plt.ylabel("distance parcourue (cm)")
plt.show()
plt.close()

plexi_d_75=[1.5,
1,
0.8,
9,
2,
7,
3,
3,
5.5,
2.5
]

plexi_d_70=[2,
2.5,
2.5,
1.8,
2.5,
2,
0,
2.5,
1.5,
2.8
]

plexi_d_65=[1.5,
2,
1.9,
2.1,
2.4,
2,
1.5,
2.2,
2.7,
1.2
]

plexi_d_60=[1.5,
1,
0.5,
1.5,
1,
0.5,
0.7,
2,
2,
1.5
]

plexi_d_55=[0.2,
1.2,
0.1,
0.5,
0.2,
0.2,
0.1,
0,
1,
0.5
]

plexi_d_50=[0.1,
0.1,
0.2,
0.4,
0.7,
0.5,
0.9,
0.7,
0.5,
0.4
]

print "plexi decallage"
plexi_d=[plexi_d_50,plexi_d_55,plexi_d_60,plexi_d_65,plexi_d_70,plexi_d_75]
plt.boxplot(plexi_d)
pylab.xticks([i for i in range(7)],[i for i in range(45,80,5)]) #decoupe decallage
plt.xlabel("vitesse des moteurs")
plt.ylabel("translation (cm)")
plt.show()
plt.close()

color=['orange','green','blue','grey','red','purple']
point_decoupe_dist=[np.mean(eval("decoupe_"+str(i))) for i in range(50,80,5)]
point_decoupe_decal=[np.mean(eval("decoupe_d_"+str(i))) for i in range(50,80,5)]
a=plt.scatter(point_decoupe_dist,point_decoupe_decal,c=color,marker=">",s=80)

point_carton_dist=[np.mean(eval("carton_"+str(i))) for i in range(50,80,5)]
point_carton_decal=[np.mean(eval("carton_d_"+str(i))) for i in range(50,80,5)]
b=plt.scatter(point_carton_dist,point_carton_decal,c=color,marker="+",s=80)

point_plexi_dist=[np.mean(eval("plexi_"+str(i))) for i in range(50,80,5)]
point_plexi_decal=[np.mean(eval("plexi_d_"+str(i))) for i in range(50,80,5)]
c=plt.scatter(point_plexi_dist,point_plexi_decal,c=color,marker="*",s=80)


x=[a,b,c]
for i in range(6):
    x.append(plt.scatter([],[],c=color[i]))

leg=['decoupe','carton','PVC']
leg+=[str(i) for i in range(50,80,5)]

plt.legend(x,leg,fontsize=8,loc='upper right',ncol=3)

ax=plt.gca()
leg=ax.get_legend()
leg.legendHandles[0].set_color('black')
leg.legendHandles[1].set_color('black')
leg.legendHandles[2].set_color('black')

plt.xlabel("distance (cm)")
plt.ylabel("translation (cm)")


plt.show()
