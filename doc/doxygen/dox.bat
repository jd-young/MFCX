echo "************************************" > MFCX.log
echo "Running dox.bat for MFCX..." > MFCX.log
date /t >> MFCX.log
time /t >> MFCX.log

if exist html rmdir /S /Q html
mkdir html
copy MFCX.gif html
copy *.html html

doxygen MFCX.dox 2>> MFCX.log
rem doxygen MFCX.dox 2>> MFCX.log
rem doxygen -d Preprocessor MFCX.dox 1> MFCX.log

echo "Finished"

date /t >> MFCX.log
time /t >> MFCX.log
