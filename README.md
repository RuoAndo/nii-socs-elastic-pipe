# nii-socs-elastic-pipe

<ul>
<li> pagination: dump tool for Elasticsearch<br>
<li> discernCPU: traffic discrinimation tool (ingoing/outgoing) by IP address<br>
</ul>

You can use script (do.sh) with the correct setting user name, password and index name of your Elasticserch.
<pre>
# ./do.sh
</pre>

Pagination (dump from Elasticsearch).
<pre>
# ./pagination --host=http://USERNAME:PASSWORD@X.X.X.X:9200 --index=INDEX_NAME_"${date}`
</pre>

1. [do.sh] Building binaries.

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

2. [do.sh] Pagination (dump) with scroll API. 

<pre>
(do.sh) rm -rf dump.txt

(do.sh) date=$(date -d '1 day ago' "+%Y.%m.%d")
(do.sh) str=`echo "time ./pagination --host=http://USERNAME:PASSWORD@X.X.X.X:9200 --index=INDEX_NAME_"${date}`
(do.sh) echo $str
(do.sh) eval $str

(do.sh) echo "copying..."
(do.sh) cp dump.txt dump.txt.${date}
</pre>

3. [do.sh] Splitting dump.

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

4. [do.sh] Traffic discrimination (ingoing/outgoing).
After step 3, you have directory like: 

<pre>
# ls 20200710/                                                                                                                                                              
xaa.h  xag.h  xam.h  xas.h  xay.h  xbe.h  xbk.h  xbq.h  xbw.h  xcc.h  xci.h  xco.h  xcu.h  xda.h  xdg.h  
</pre>

<pre>
(do.sh) ./discernCPU $date list-sample
</pre>

Sample list file.
<pre>
# more list-sample 
192.168.0.0,24
</pre>

Note: traffic discrimination is based on the data format as follows:

<pre>
# cd src/
# ./build.sh gendata                                                                                                                                                    
# ./gendata 100
# head -n 3 random_data.txt                                                                                                                                             
"2019/07/02 00:00:19.952","2019/07/02 00:00:19","2019/07/02 00:00:19","841","198.112.80.204","25846","om","205.183.30.17","51321","lT","kFS","XkruUfNU9","mcm","oKEFj","giiOkVuT","8","vwamZWK1vUtsMs2dXAjA2xRCuh","912","198","336","769","278","554","rand-pa1"
"2019/07/02 00:00:30.870","2019/07/02 00:00:30","2019/07/02 00:00:30","478","174.0.245.225","41214","Xc","41.157.209.123","23907","p9","q40","EwkU6WZvo","laf","fM6ZX","90MTO4d6","5","3jBdNEObCi9QT8EUQw0dKhNSaG","953","917","636","718","142","607","rand-pa1"
"2019/07/02 00:00:46.949","2019/07/02 00:00:46","2019/07/02 00:00:46","17","255.8.70.117","15918","bj","223.159.139.187","8994","9n","vUu","Ymp8d7N3u","Rqe","9XREA","g5TvA86Z","8","Km9NlKHruyoB4WDO4Nig2eEhaW","157","401","130","109","999","219","rand-pa1"
</pre>