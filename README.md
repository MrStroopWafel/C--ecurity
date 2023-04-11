# C--ecurity -- Sys32 tester
 
1. Compile fileInfector door volgende commando's te gebruiken in MingW64 (NIET 32):
- cd "plaats waar je fileInfector hebt" (niet teveel paden anders zegt ie teveel arguments en 2x \ gebruiken niet 1x\)
- g++ fileInfector.cpp -o fileInfector.exe -lshell32 -lole32 (dit maakt fileInfector.exe)
2. Run fileInfector.exe met admin rights, werkt niet zonder admin
