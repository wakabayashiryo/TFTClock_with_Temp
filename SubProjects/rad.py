import math as mt
 

def printXY(x,y,r,time):
    fai = 360 * (float(time) / 60)
    rad = fai * mt.pi / 180
    return mt.cos(rad),mt.sin(rad)

def main():
    for i in range(0,60):
        (x,y) = printXY(0,0,10,i)
     
        if y>0:
            print(" ",end="") 
        print ("{0:.5f},".format(y), end="")
        if x<0:
            print(" ",end="")
        print ("{0:.5f},".format(-x))

if __name__ == '__main__':
    main()