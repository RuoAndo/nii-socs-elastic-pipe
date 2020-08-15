#RMDATE=`date --date '3 day ago' +%Y%m%d`                                                                                
#cd $RMDATE
#rm -rf OUTPUT_*

RMDATE=`date --date '5 day ago' +%Y%m%d`
echo $RMDATE
rm -rf $RMDATE



