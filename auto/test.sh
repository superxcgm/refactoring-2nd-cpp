#!/bin/bash

./auto/build.sh

./build/main > actual.txt
diff_file=test.diff

if diff expect.txt actual.txt > "$diff_file" ; then
  echo -e "  Test \033[32mpass\033[0m. "
  rm "$diff_file"
else
  echo -e "  Test \033[31mfailed\033[0m. "
  cat $diff_file
  ret_code=1
fi

exit $ret_code