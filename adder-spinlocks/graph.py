import pandas as pd
import matplotlib.pyplot as plt

plt.figure(1)

data10000000 = [
  [1,21],
  [2, 11],
  [4, 5],
  [8,4],
  [16,5],
  [32,11],
  [64,18],
  [128,22],
  [256,26]
]

data100000000 = [
  [1,213],
  [2,172],
  [4,142],
  [8,123],
  [16,94],
  [32,91],
  [64,110],
  [128,100],
  [256,118]
]

data1000000000 = [
  [1,2170],
  [2,1178],
  [4,975],
  [8,896],
  [16,936],
  [32,962],
  [64,828],
  [128,581],
  [256,541]
]

df10000000 = pd.DataFrame(data10000000)
df100000000 = pd.DataFrame(data100000000)
df1000000000 = pd.DataFrame(data1000000000)

plt.plot(df10000000[0], df10000000[1],  label= "N = 10000000")
plt.plot(df100000000[0], df100000000[1],  label= "N = 100000000")
plt.plot(df1000000000[0], df1000000000[1],  label= "N = 1000000000")

plt.title('Número de Threads vs Tempo (ms)')
plt.ylabel('Tempo (ms)')
plt.xlabel('2^x')
plt.legend()
plt.savefig('graph.png')