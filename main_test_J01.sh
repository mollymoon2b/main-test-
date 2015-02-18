yo=$(cat 42)
yo2="Coucou 42\n"
if [ $(yo) = $(yo2) ]
then
	echo "OK"
else
	echo "FAILED"
fi