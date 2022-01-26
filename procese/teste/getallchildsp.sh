#!/bin/bash 

TESTDIR=$(realpath "$(dirname "BASH_SOURCE")")
TEMEDIR=$(realpath "$TESTDIR"/../teme)

<<COMENTARIU
Pentru ca testul sa treaca cu succes, trebuie sa rulam programul pentru a genera procese copil. Daca programul va fi rulat de mai multe ori, 
atunci se vor elimina procesele generate anterior si vor fi afisate cele noi. In cazul in care nu rulam programul, atunci nu vom avea procese 
copil si programul va afisa Failed. No process child found.
COMENTARIU

CMD="getallchildsp"

cleanup()
{
    # if the last process group ID identified by a specific process is different from the newest one, then the last process generated is eliminated
    while [ "$(ps -o pgid -p "$(pgrep -o $CMD)")" != "$(ps -o pgid -p "$(pidof -s $CMD)")" > /dev/null ]
    do
        kill -9 $(pgrep -o $CMD)
    done
}

check_processes()
{
    # check if there are process child
    if [[ -z $(pidof $CMD) ]];
    then
        echo -e Test child processes list: "\033[0;31mFailed. No process child found.\033[0m" $'\n';
        exit
    else
        echo -e Test child processes list: "\033[0;32mSuccess. Running...\033[0m" $'\n';
    fi
}

check_processes
cleanup

pidof $CMD

echo $'\n'"The number of child processes: "
pidof $CMD | wc -w

exit
