# nii-socs-elastic-pipe

<ul>
<li> pagination: dump tool for Elasticsearch<br>
<li> discernCPU: traffic discrinimation tool (ingoing/outgoing) by IP address<br>
</ul>

Pagination (dump from Elasticsearch).
<pre>
# ./pagination --host=http://USERNAME:PASSWORD@X.X.X.X:9200 --index=INDEX_NAME_"${date}`
</pre>

Traffic discrimination.
<pre>
# ./discernCPU 20200710 list-sample
</pre>

You can use script (do.sh). 

Traffic discrimination.
<pre>
# ./do.sh
</pre>
 
1. Building binaries.

<pre>
# git submodule update --init
</pre>

<pre>
(do.sh) make clean
(do.sh) make

(do.sh) cd src
(do.sh) ./build.sh discernCPU 
(do.sh) mv discernCPU ../
(do.sh) cd ..
</pre>

2. Pagination (dump) with scroll API. 

<pre>
(do.sh) rm -rf dump.txt

(do.sh) date=$(date -d '1 day ago' "+%Y.%m.%d")
(do.sh) str=`echo "time ./pagination --host=http://USERNAME:PASSWORD@X.X.X.X:9200 --index=INDEX_NAME_"${date}`
(do.sh) echo $str
(do.sh) eval $str

(do.sh) echo "copying..."
(do.sh) cp dump.txt dump.txt.${date}
</pre>

3. Splitting dump.

<pre>
(do.sh) date=$(date -d '1 day ago' "+%Y%m%d")
(do.sh) mkdir $date

(do.sh) echo "spliting..."
(do.sh) split -l 2500000 dump.txt 

(do.sh) ls x* > list

(do.sh) while read line; do
(do.sh)     echo ${line}

(do.sh)     cat header > ${line}.h
(do.sh)     cat ${line} >> ${line}.h

(do.sh)     mv ${line}.h ./${date}/
(do.sh)     rm -rf ${line}
(do.sh) done < list

(do.sh) rm -rf dump.txt
</pre>

4. Traffic discrimination (ingoing/outgoing).

<pre>
(do.sh) ./discernCPU $date list-sample
</pre>

<pre>
# more list-sample 
192.168.0.0,24
</pre>
