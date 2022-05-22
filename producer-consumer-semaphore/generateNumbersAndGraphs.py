import pandas as pd
import matplotlib.pyplot as plt
import subprocess
import pathlib
pathlib.Path().resolve()

data = []

for n in [1,2,4,16,32]:
  nData = []
  for (np,nc) in [(1, 1),(1, 2),(1, 4),(1, 8),(2, 1),(4, 1),(8, 1)]:
    elapsedTime = 0.0

    for i in range(10):
      x = subprocess.Popen(['{}/main'.format(pathlib.Path().resolve()), str(np), str(nc), str(n), '0'], stdout = subprocess.PIPE)
      elapsedTime += float(x.communicate()[0].decode("utf-8"))
    
    elapsedTime /= 10

    nData.append([np/nc, elapsedTime])
  
  data.append([n, nData]);

plt.figure(1)

for [n,dataSet] in data:
  df = pd.DataFrame(dataSet, columns=['np/nc', 'elapsedTime'])

  plt.plot(df['np/nc'], df['elapsedTime'],  label=n)

plt.title('Número de Threads vs Tempo (ms)')
plt.ylabel('Tempo (ms)')
plt.xlabel('Np/Nc')
plt.legend()
plt.savefig('graph.png')

