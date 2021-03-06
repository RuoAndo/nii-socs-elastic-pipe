# nii-socs-elastic-pipe

<img src="191429A7-F6BD-4463-98AF-5DF6583952C9.jpeg">

Executables.
<ul>
<li> pagination: dump tool for Elasticsearch<br>
<li> discernCPU: traffic discrinimation tool (ingoing/outgoing) by IP address<br>
</ul>

You can use script (do.sh) with the correct setting user name, password and index name of your Elasticserch at line 12 of do.sh.
<pre>
# ./do.sh
</pre>

<pre>
12: ./pagination --host=http://USERNAME:PASSWORD@X.X.X.X:9200 --index=INDEX_NAME_"${date}`
</pre>

<hr>

First of all, before running script (building binaries). 
<pre>
# git submodule update --init
</pre>

Script is divided into four steps.

1. [do.sh] Building binaries.

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

<hr>

Note 1: traffic discrimination of step 4 is based on the data format as follows:

<pre>
# cd src/
# ./build.sh gendata                                                                                                                                                    
# ./gendata 100
# head -n 3 random_data.txt                                                                                                                                             
"2019/07/02 00:00:19.952","2019/07/02 00:00:19","2019/07/02 00:00:19","841","198.112.80.204","25846","om","205.183.30.17","51321","lT","kFS","XkruUfNU9","mcm","oKEFj","giiOkVuT","8","vwamZWK1vUtsMs2dXAjA2xRCuh","912","198","336","769","278","554","rand-pa1"
"2019/07/02 00:00:30.870","2019/07/02 00:00:30","2019/07/02 00:00:30","478","174.0.245.225","41214","Xc","41.157.209.123","23907","p9","q40","EwkU6WZvo","laf","fM6ZX","90MTO4d6","5","3jBdNEObCi9QT8EUQw0dKhNSaG","953","917","636","718","142","607","rand-pa1"
"2019/07/02 00:00:46.949","2019/07/02 00:00:46","2019/07/02 00:00:46","17","255.8.70.117","15918","bj","223.159.139.187","8994","9n","vUu","Ymp8d7N3u","Rqe","9XREA","g5TvA86Z","8","Km9NlKHruyoB4WDO4Nig2eEhaW","157","401","130","109","999","219","rand-pa1"
</pre>

Sample list file.
<pre>
# more list-sample 
192.168.0.0,24
</pre>

Note 2: data discription (randomly generated by gendata.cpp):

<pre>
# head -n 2 random_data.txt                                                        
"2019-05-01 08:38:45.010",
"2019-05-01 08:38:45",
"2019-05-01 08:38:45",
"841",
"*.*.82.39",
"25846",
"ua",
"*.*.163.161",
"51321",
"aI",
"SCu",
"HFCi1qAnY",
"Xub",
"Aregp",
"ab8tqr2Y",
"8",
"cyX3duDunIjVkTzAMIFLO7S8WA",
"912",
"198",
"336",
"769",
"278",
"554",
"rand-pa1"
</pre>

<pre>
  - 1  capture_time 
  - 2  generated_time
  - 3  start_time
  - 4  elapsed_time
  - 5  source_ip
  - 6  source_port
  - 7  src_country_code
  - 8  destination_ip
  - 9  destination_port
  - 10  dest_country_code
  - 11  protocol
  - 12  application
  - 13  subtype
  - 14  action
  - 15  session_end_reason
  - 16  repeat_count
  - 17  category
  - 18  packets
  - 19  packets_sent
  - 20  packets_received
  - 21  bytes
  - 22  bytes_sent
  - 23  bytes_received
  - 24  device_name
</pre>

<hr>

## discernPort
port filter.
<pre>
# ./discernPort [DATE_DIR] [list-sample] [PortNo]
</pre>

## discernIPaddr
IPaddr filter.
<pre>
Usage: ./discernIPaddr [DIR] [LIST-NAME] [srcIP_rowNo] [destIP_rowNo]  
# ./discernIPaddr 20200811 list-sample 27 20
</pre>

<pre>
[0]        [1]            [20]                    [27]
timestamp, protocol, ... , X.X.X.X (srcIP), ... , Y.Y.Y.Y (destIP)
</pre>

## singleIP
single IP address finder.
<pre>
# ./build.sh singleIP                                                                                                                                          
# ./singleIP 
Usage: ./singleIP [IPAddr] [LIST_FILE_NAME] 
</pre>

<pre>
# cat list-sample
8.8.8.0,24
# ./singleIP 8.8.8.1 list-sample                                                                                                                             
HIT: 8.8.8.1 <- 8.8.8.0/24
# ./singleIP 8.8.8.3 list-sample
HIT: 8.8.8.3 <- 8.8.8.0/24
</pre>