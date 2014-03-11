#!/bin/bash

help_syntax()
{
  echo -e "\n [ERROR] USAGE: $0 -f <FILE> -t <TIMEOUT> -b <BATCHOPS> \n" 
  echo -e "Exiting from the script.... ... .. . ."
  exit 1
}

if [ $# -ne 6 ] 
then 
  help_syntax
fi

while getopts f:t:b: opt
do 
  case $opt in 
    f) 
      # file name 
      FILE=${OPTARG}
      echo -e "\n [INFO] FILE NAME: ${FILE} \n";;
    t) 
      # timeout for refresh
      REFRESH=${OPTARG}
      echo -e "\n [INFO] REFRESH TIME: ${REFRESH} \n";;
    b) 
      # no of batch operations 
      BATCHOPS=${OPTARG}
      echo -e "\n[INFO] BATCH OPERATIONS: ${BATCHOPS} \n";;
    ?)
      # invalid
      echo -e "\n [ERROR] Invalid option \n"
      help_syntax;;
  esac
done

if [ ! -f "${FILE}" ] 
then 
  echo -e "\n [ERROR] Invalid file passed \n"
  exit 1
fi

echo -e "\n [INFO] Compiling again ... \n"
cd ..
./compile.sh
cd tests

TIMESTAMP=`date +"%F-%T"`
OUTPUT="test${TIMESTAMP}"

exec 5< ${FILE}

while read -u5 line
do 
  #cd ${SCRIPT_PATH}
  #cd ..
  echo -e "\n [INFO] TESTING FOR ${REFRESH} time and ${line} elements\n"
  ./../FBF ${REFRESH} ${line} ${BATCHOPS}>> ${OUTPUT}
  echo -e "\n---------------------------------\n" >> ${OUTPUT}
done

##
# END OF SCRIPT 
##
