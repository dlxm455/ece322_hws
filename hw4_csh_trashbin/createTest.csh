#! /bin/csh

mkdir testfolder1
cd testfolder1
touch a.txt
touch b.txt

cd ..
mkdir testfolder2
cd testfolder2
touch c.txt
touch d.txt
mkdir testtest
cd testtest
touch e.txt

cd ../..
touch aa.txt
touch bb