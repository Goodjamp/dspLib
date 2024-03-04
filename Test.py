import math

N=8

myStr=""
valF = 0.0
for k in range(0, N):
    for i in range(0, N):
        valF = math.cos(2*math.pi*k*i/N) *100
        val = math.floor(valF) if valF >= 0 else math.ceil(valF)
        myStr += "  {:4}".format(val)
    print(myStr)
    myStr = ""
    
