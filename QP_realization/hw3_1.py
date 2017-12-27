import sys
import numpy as np
import pandas as pd

if len(sys.argv) != 5:
    print(sys.argv)
    sys.exit("usage: script.py datafilename N K outfile\n")
print(sys.argv)

try:
    datafile = open(sys.argv[1], 'r') # opens the data file
except IOError:
    sys.exit("Cannot open file %s\n" % sys.argv[1])


tmp=np.loadtxt(datafile, dtype=np.str, delimiter=",")
N = int(sys.argv[2])
K = int(sys.argv[3])
lower=0.0
upper=round(max(0.01,2/float(K)),2)
data=tmp[:K,:N].astype(np.float)
d=pd.DataFrame(data)
log_return=np.log(d)-np.log(d.shift(1,axis=1))
cov=log_return.transpose().cov(1)
c=pd.DataFrame(cov)

thisoutput = open(sys.argv[4], "w")
print ("n days", N, " K assets ", K)
print log_return

thisoutput.write("n " + str(K) + "\n")
thisoutput.write("\n"+"j_lower_upper_mu"+"\n"+"\n")

for i in range(K):
    m=100*round(log_return.mean(1)[i],4)
    thisoutput.write(str(i)+" "+str(lower)+"   "+str(upper)+"   "+str(m)+"\n")

thisoutput.write("\n"+"lambda "+str(N)+"\n"+"\n"+"covariance"+"\n"+"\n")
for i in range(K):
    for j in range(K):
        thisoutput.write(str(round(10000*c[i][j],2))+" ")
    thisoutput.write("\n")
thisoutput.write("\n"+"END")
print("Done!")







