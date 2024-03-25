#!/bin/bash
# This is a shell script to check your program(s) on test cases.
# It usese the same language you normally use to type commands in
# a terminal window.  You can write whole programs in shell.

# Assume we've succeeded until we see otherwise.
FAIL=0

# Print an error message and set the fail flag.
fail() {
    echo "**** $1"
    FAIL=1
}

# Check the exit status.  The actual exit status (ASTATUS) should match
# the expected status (ESTATUS)
checkStatus() {
  ESTATUS="$1"
  ASTATUS="$2"

  # Make sure the program exited with the right exit status.
  if [ "$ASTATUS" -ne "$ESTATUS" ]; then
      fail "FAILED - incorrect program exit status. (expected $ESTATUS,  Got: $ASTATUS)"
      return 1
  fi

  return 0
}

# Check the contents of an a file.  If the expected file (EFILE)
# exists, then the actual file (AFILE) should exist and it should match.
checkFile() {
  NAME="$1"
  EFILE="$2"
  AFILE="$3"

  # Make sure we're really expecting this file.
  if [ ! -f "$EFILE" ]; then
      return 0
  fi

  # Make sure the output matches the expected output.
  echo "   diff $EFILE $AFILE"
  diff -q "$EFILE" "$AFILE" >/dev/null 2>&1
  if [ $? -ne 0 ]; then
      fail "FAILED - $NAME ($AFILE) doesn't match expected ($EFILE)"
      return 1
  fi

  return 0
}

# Same as checkFile, but if the expected file (EFILE) doesn't exist, the
# actual file (AFILE) should be empty.
checkFileOrEmpty() {
  NAME="$1"
  EFILE="$2"
  AFILE="$3"
  
  # if the expected output file doesn't exist, the actual file should be
  # empty.
  if [ ! -f "$EFILE" ]; then
      if [ -s "$AFILE" ]; then
	  fail "FAILED - $NAME ($AFILE) should be empty."
	  return 1
      fi
      return 0
  fi

  # Make sure the output matches the expected output.
  echo "   diff $EFILE $AFILE"
  diff -q "$EFILE" "$AFILE" >/dev/null 2>&1
  if [ $? -ne 0 ]; then
      fail "FAILED - $NAME ($AFILE) doesn't match expected ($EFILE)"
      return 1
  fi

  return 0
}

# The given file should exist but should be empty.
checkEmpty() {
  NAME="$1"
  AFILE="$2"
  
  if [ -s "$AFILE" ]; then
      fail "FAILED - $NAME ($AFILE) should be empty."
      return 1
  fi

  return 0
}

# Test the replace program.
testReplace() {
  TESTNO=$1
  ESTATUS=$2

  echo "Test $TESTNO"
  rm -f output.txt stderr.txt

  echo "   ./replace ${args[@]} input-$TESTNO.txt output.txt > stdout.txt 2> stderr.txt"
  ./replace ${args[@]} input-$TESTNO.txt output.txt > stdout.txt 2> stderr.txt
  ASTATUS=$?

  if ! checkStatus "$ESTATUS" "$ASTATUS" ||
     ! checkFile "output file" "expected-$TESTNO.txt" "output.txt" ||
     ! checkEmpty "Stdout output" "stdout.txt" ||
     ! checkFileOrEmpty "Stderr output" "expected-stderr-$TESTNO.txt" "stderr.txt"
  then
      FAIL=1
      return 1
  fi

  echo "Test $TESTNO PASS"
  return 0
}

# make a fresh copy of the target programs
make clean
make
if [ $? -ne 0 ]; then
  fail "Make exited unsuccessfully"
fi

# Run test cases
if [ -x replace ]; then
    args=()
    testReplace 01 0
    
    args=(a b)
    testReplace 02 0
    
    args=(too two)
    testReplace 03 0
    
    args=(for four too two)
    testReplace 04 0
    
    args=(for four too two)
    testReplace 05 0
    
    args=(and or)
    testReplace 06 0
    
    args=(28 30 a_b A_B Facebook Meta)
    testReplace 07 0
    
    args=(assemble gather speech talking press media redress payback shall must)
    testReplace 08 0
    
    args=(abc word_1 XYZ word_2 1234 word_3 abc_123 word_4)
    testReplace 09 0

    args=(abcdefghijklmnopqrstuvwxyz ABCs)
    testReplace 10 0

    args=(ABCs abcdefghijklmnopqrstuvwxyz)
    testReplace 11 0

    args=(COUNT 10 EXIT_SUCCESS 0 Hello Hola World Mundo)
    testReplace 12 0

    args=(Quoth Said Nevermore Nope chamber bedroom thy your thee you implore ask hath has countenance expression)
    testReplace 13 0

    args=(most Most One one)
    testReplace 14 0

    args=(good_word bad-word)
    testReplace 15 1

    args=(five number_1 ten number_2 five number_3)
    testReplace 16 1
    
    args=(no input file for this test)
    testReplace 17 1
else
    fail "Since your program didn't compile, we couldn't test it"
fi

if [ $FAIL -ne 0 ]; then
  echo "FAILING TESTS!"
  exit 13
else 
  echo "TESTS SUCCESSFUL"
  exit 0
fi
