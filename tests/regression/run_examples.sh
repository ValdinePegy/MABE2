BUILD_DIR="../../build"

THIS_DIR=`pwd`
MABE_FILES=`ls ${THIS_DIR} | grep .mabe$`

cd ${BUILD_DIR}
make clean
make debug
cd ${THIS_DIR}

for filename in ${MABE_FILES}
do
  NAME=`echo ${filename} | grep -P "^[\w|_][\w|\d_]+" -o`
  echo "${NAME}"
  DIR="output/${NAME}"
  mkdir ${DIR} -p
  cd ${DIR}
  cp ../../${BUILD_DIR}/MABE ./
  cp ${THIS_DIR}/${filename} ./
  cp ${THIS_DIR}/copy* ./
  ./MABE -f ${filename} > terminal_output.txt

  # Check generated data against expected data
  cmp terminal_output.txt ${THIS_DIR}/expected_terminal_output_${NAME}.txt 
  TERMINAL_ERROR_CODE=$?
  if ! test ${TERMINAL_ERROR_CODE} -eq 0;
  then
    echo "Error! File generated different terminal output!"
    head terminal_output.txt -n 200
    exit 1
  fi

  cmp output.csv ${THIS_DIR}/expected_csv_output_${NAME}.csv
  CSV_ERROR_CODE=$?
  if ! test ${CSV_ERROR_CODE} -eq 0;
  then
    echo "Error! File generated different values for output.csv"
    head output.csv -n 50
    exit 2
  fi
  # Reset back to original directory
  cd ${THIS_DIR} 
done
echo "All examples successfully executed!"
# Placeholder comment
