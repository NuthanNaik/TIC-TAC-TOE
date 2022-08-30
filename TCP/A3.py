import matplotlib.pyplot as plt
input=open('outfile','r')
lines=input.readlines();
x=[]
y=[]
for i in range(0,len(lines)):
    x.append(i+1)
    y.append(float(lines[i]))
plt.plot(x,y)
plt.xlabel('update number')
plt.ylabel('CW value')
plt.title('TCP congestion')
plt.show()

    
    
