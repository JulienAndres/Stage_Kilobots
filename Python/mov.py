from mrpiZ_lib import *


#seuil detection obstacle
seuil=70


while true:
    sensors=[proxSensor(i) for i in range(1,6)]
    flag =0
    if sensors[2] < seuil:
        flag=1
        stop()
        turnLeft(50)

    if sensors[4]<seuil:
        flag=1
        stop()
        turnRight(50)
    if sum([1 if sensors[i]<seuil for i in range(2,5)])==3:
        flag=1
        stop()
        back(50)
    if flag==0:
        forward(50)
