#!/bin/bash

# exit on first problem
set -e
trap "killall -9 master" EXIT

HTML_FILE="test_result.html"
IMGS_FOLDER="test_result_imgs"
TEST_FOLDER="tests"
TEST_FILES="${TEST_FOLDER}"/*.test

# remove old files, create new
rm -rf $HTML_FILE $IMGS_FOLDER
touch $HTML_FILE
mkdir $IMGS_FOLDER

# write beginning of the HTML
cat >> $HTML_FILE << EOF
<HTML>
	<BODY>
		<H1>Ray-tracing Unit Tests</H1>
EOF

for TEST_FILE in $TEST_FILES
do
	TAR_FILE=""
	WIDTH=""
	DESCRIPTION=""

	echo "Processing \"${TEST_FILE}\"..."
	source "$TEST_FILE"

	INPUT_FILE="$TEST_FOLDER"/"$TAR_FILE"
	OUTPUT_FILE="$IMGS_FOLDER"/"$TAR_FILE".bmp

	# run a master
	sh -c "./master --log 0 --realtime 0 --width $WIDTH \"$INPUT_FILE\" \"$OUTPUT_FILE\"" &
	PID_MASTER=$!
	
	# run a slave
	sh -c "sleep 1 && ./slave --log 0" &
	PID_SLAVE=$!

	# wait for the task to finish
	wait $PID_MASTER

	# add section to the HTML file
	cat >> $HTML_FILE << EOF
		<H2>$TEST_FILE</H2>
		<P>$DESCRIPTION</P>
		<IMG SRC="$OUTPUT_FILE" />

EOF
done

# write end of HTML
cat >> $HTML_FILE << EOF
	</BODY>
</HTML>
EOF
