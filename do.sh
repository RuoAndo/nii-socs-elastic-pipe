make clean
make

cd src
./build.sh discernCPU 
mv discernCPU ../
cd ..

rm -rf dump.txt

date=$(date -d '1 day ago' "+%Y.%m.%d")
str=`echo "time ./pagination --host=http://USERNAME:PASSWORD@X.X.X.X:9200 --index=INDEX_NAME_"${date}`
echo $str
eval $str

echo "copying..."
cp dump.txt dump.txt.${date}

date=$(date -d '1 day ago' "+%Y%m%d")
mkdir $date

echo "spliting..."
split -l 2500000 dump.txt 

ls x* > list

while read line; do
    echo ${line}

    cat header > ${line}.h
    cat ${line} >> ${line}.h

    mv ${line}.h ./${date}/
    rm -rf ${line}
done < list

rm -rf dump.txt

time ./discernCPU $date list-sample


