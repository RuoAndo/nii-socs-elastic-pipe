RMDATE=`date --date '31 day ago' +%Y%m%d`
echo $RMDATE

rm -rf egress_25_${RMDATE}
rm -rf ingress_25_${RMDATE}

rm -rf ingress_25/*${RMDATE}
rm -rf egress_25/*${RMDATE}



