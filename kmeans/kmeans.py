import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("KorjattuData.csv")
df = df.sort_values(by="direction", ascending=True)

dl1 = []
dl2 = []
dl3 = []
dl4 = []
dl5 = []
dl6 = []

for i, row in df.iterrows():
    direction = row['direction']
    match direction:
        case 1:
            dl1.append(row)
        case 2:
            dl2.append(row)
        case 3:
            dl3.append(row)
        case 4:
            dl4.append(row) 
        case 5:
            dl5.append(row)
        case 6:
            dl6.append(row)
dl1 = pd.DataFrame(dl1)
dl2 = pd.DataFrame(dl2)
dl3 = pd.DataFrame(dl3)
dl4 = pd.DataFrame(dl4)
dl5 = pd.DataFrame(dl5)
dl6 = pd.DataFrame(dl6)

np1 = dl1[["x","y","z"]].values
np2 = dl2[["x","y","z"]].values
np3 = dl3[["x","y","z"]].values
np4 = dl4[["x","y","z"]].values
np5 = dl5[["x","y","z"]].values
np6 = dl6[["x","y","z"]].values

def keskiarvo(list):
    average_values = np.mean(list, axis=0)
    rounded_values = [int(round(value)) for value in average_values]
    #print(rounded_values)
    return(rounded_values)

c1 = keskiarvo(np1)
c2 = keskiarvo(np2)
c3 = keskiarvo(np3)
c4 = keskiarvo(np4)
c5 = keskiarvo(np5)
c6 = keskiarvo(np6)
print (np1[:,2])

plt.figure()
plt.subplot(1,1,1,projection="3d")
plt.title("c1")
plt.plot(c1[0],c1[1],c1[2],"ro")
plt.plot(np1[:,0],np1[:,1],np1[:,2], "ro", c="g", alpha=0.25)

plt.title("c2")
plt.plot(c2[0],c2[1],c2[2],"ro")
plt.plot(np2[:,0],np2[:,1],np2[:,2], "ro", c="g", alpha=0.25)


plt.title("c3")
plt.plot(c3[0],c3[1],c3[2],"ro")
plt.plot(np3[:,0],np3[:,1],np3[:,2], "ro", c="g", alpha=0.25)


plt.title("c4")
plt.plot(c4[0],c4[1],c4[2],"ro")
plt.plot(np4[:,0],np4[:,1],np4[:,2], "ro", c="g", alpha=0.25)

plt.title("c5")
plt.plot(c5[0],c5[1],c5[2],"ro")
plt.plot(np5[:,0],np5[:,1],np5[:,2], "ro", c="g", alpha=0.25)

plt.title("c6")
plt.plot(c6[0],c6[1],c6[2],"ro")
plt.plot(np6[:,0],np6[:,1],np6[:,2], "ro", c="g", alpha=0.25)
plt.show()