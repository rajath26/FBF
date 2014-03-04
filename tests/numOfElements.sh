#!/bin/bash

help_syntax()
{
  echo -e "\n [ERROR] USAGE: ./$0 -f <FILE> -t <TIMEOUT> \n" 
  echo -e "Exiting from the script.... ... .. . ."
  exit 1
}

if [ $# -ne 2 ] 
then 
  help_syntax
fi

while getopts f: opt
do 
  case $opt in 
    f) 
      # file name 
      FILE=${OPTARG}
      echo -e "\n [INFO] FILE NAME: ${FILE} \n"
    t) 
      # timeout for refresh
      REFRESH=${OPTARG}
      echo -e "\n [INFO] REFRESH TIME: ${REFRESH} \n"
    ?)
      # invalid
      echo -e "\n [ERROR] Invalid option \n"
  esac
done

#
# get the script path
#

# variable stores the absolute path of the script
SCRIPT_PATH="$( dirname "$( which "$0" )" )"
# check the return code
if [ $? -ne 0 ]
then
    echo -e "[ERROR]  Unable to get the path of this script"
    echo -e "Exiting from the script.... ... .. . ."
    exit 1
fi

echo -e "\n [INFO] The script path is: ${SCRIPT_PATH}\n"

if [ ! -f "${FILE}" ] 
then 
  echo -e "\n [ERROR] Invalid file passed \n"
  exit 1
fi

TIMESTAMP=`date +"%F-%T"`
OUTPUT="test${TIMESTAMP}"

exec 5< ${FILE}

while read -u5 line
do 
  cd ${SCRIPT_PATH}
  cd ..
  ./FBF ${REFRESH} ${line} >> ${SCRIPT_PATH}/${OUTPUT}
done