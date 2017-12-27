

Part 1:

(This CSV file (zipped) has data for 1000 assets over 250 days. Write a Python script that compute the average asset returns and the covariance of returns for the first K assets over the first N days. Here K and N are part of the command line. Output the results to a file.)

The functionality is implemented in "hw3_1.py". To generate a formatted result, run "python hw3_1.py datafilename N(days) K(assets) outfilename". For example, run "python hw3_1.py dumps.csv 10 4 formatted.dat".


Part 2:

(Implement the first-order algorithm we discussed in class. Test it on this example. Test it on larger examples obtained as in Part 1 (formatted as in the small example). You may assume that the maximum weight for any asset is max(0.01, 2/n), and the minimum weight is zero.)

The functionality is implemented in "hw3_2.cpp". To compute the maximized portfolio, compile "hw3_2.cpp" in a cpp project to get an executable "QP.exe". Run "QP.exe formatteddatafilename". For example, run "QP.exe formatted.dat". The result will be written into "theoutput.dat".
