#!/bin/bash
echo "***************Generate Coverage*****************"

if [ -d "./coverage" ]; then
    rm -rf ./coverage
fi
mkdir coverage

lcov -c -d ./build_ut/test/ut/CMakeFiles/mssanitizer_test.dir -o ./coverage/mssanitizer_test.info -b ./coverage --rc lcov_branch_coverage=1 --rc geninfo_no_exception_branch=1

lcov -r ./coverage/mssanitizer_test.info '*platform*' -o ./coverage/mssanitizer_test.info --rc lcov_branch_coverage=1 --rc geninfo_no_exception_branch=1
lcov -r ./coverage/mssanitizer_test.info '*opensource*' -o ./coverage/mssanitizer_test.info --rc lcov_branch_coverage=1 --rc geninfo_no_exception_branch=1
lcov -r ./coverage/mssanitizer_test.info '*test*' -o ./coverage/mssanitizer_test.info --rc lcov_branch_coverage=1 --rc geninfo_no_exception_branch=1
lcov -r ./coverage/mssanitizer_test.info '*c++*' -o ./coverage/mssanitizer_test.info --rc lcov_branch_coverage=1 --rc geninfo_no_exception_branch=1
lcov -r ./coverage/mssanitizer_test.info '/usr/include/*' -o ./coverage/mssanitizer_test.info --rc lcov_branch_coverage=1 --rc geninfo_no_exception_branch=1

genhtml ./coverage/mssanitizer_test.info -o ./coverage/report --branch-coverage

mv test_detail.xml coverage/report/

cd coverage
tar -zcvf report.tar.gz ./report
