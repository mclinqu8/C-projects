#!/bin/bash
FAIL=0

# Function to run a single test of the polish10 program.
testPolish10() {
  TESTNO=$1
  ESTATUS=$2

  rm -f output.txt
  
  echo "Test $TESTNO: ./polish10 < input-10-$TESTNO.txt > output.txt"
  ./polish10 < input-10-$TESTNO.txt > output.txt
  STATUS=$?

  # Make sure the program exited with the right exit status.
  if [ $STATUS -ne $ESTATUS ]; then
      echo "**** FAILED - Expected an exit status of $ESTATUS, but got: $STATUS"
      FAIL=1
      return 1
  fi

  # Make sure output matches expected output.
  if ! diff -q expected-10-$TESTNO.txt output.txt >/dev/null 2>&1 ; then
      echo "**** FAILED - output didn't match expected output."
      FAIL=1
      return 1
  fi

  echo "PASS"
  return 0
}

# Function to run a single test of the polish32 program.
testPolish32() {
  TESTNO=$1
  ESTATUS=$2

  rm -f output.txt
  
  echo "Test $TESTNO: ./polish32 < input-32-$TESTNO.txt > output.txt"
  ./polish32 < input-32-$TESTNO.txt > output.txt
  STATUS=$?

  # Make sure the program exited with the right exit status.
  if [ $STATUS -ne $ESTATUS ]; then
      echo "**** FAILED - Expected an exit status of $ESTATUS, but got: $STATUS"
      FAIL=1
      return 1
  fi

  # Make sure output matches expected output.
  if ! diff -q expected-32-$TESTNO.txt output.txt >/dev/null 2>&1 ; then
      echo "**** FAILED - output didn't match expected output."
      FAIL=1
      return 1
  fi

  echo "PASS"
  return 0
}

# Try to get a fresh compile of the project.
echo "Running make clean"
make clean

echo "Building polish10 with make"
make polish10
if [ $? -ne 0 ]; then
    echo "**** Make didn't run succesfully when trying to build your polish10 program."
    FAIL=1
fi

# Run tests for polish10
if [ -x polish10 ] ; then
    testPolish10 01 0
    testPolish10 02 0
    testPolish10 03 0
    testPolish10 04 0
    testPolish10 05 0
    testPolish10 06 0
    testPolish10 07 0
    testPolish10 08 0
    testPolish10 09 0
    testPolish10 10 0
    testPolish10 11 0
    testPolish10 12 100
    testPolish10 13 102
    testPolish10 14 101
else
    echo "**** Your polish10 program couldn't be tested since it didn't compile successfully."
    FAIL=1

fi

echo "Building polish32 with make"
make polish32
if [ $? -ne 0 ]; then
    echo "**** Make didn't run succesfully when trying to build your polish32 program."
    FAIL=1
fi

# Run tests for polish32
if [ -x polish32 ] ; then
    testPolish32 01 0
    testPolish32 02 0
    testPolish32 03 0
    testPolish32 04 0
    testPolish32 05 0
    testPolish32 06 0
    testPolish32 07 0
    testPolish32 08 0
    testPolish32 09 102
    testPolish32 10 100
    testPolish32 11 100
    testPolish32 12 102
else
    echo "**** Your polish32 program couldn't be tested since it didn't compile successfully."
    FAIL=1

fi

if [ $FAIL -ne 0 ]; then
  echo "**** There were failing tests"
  exit 1
else 
  echo "Tests successful"
  exit 0
fi
