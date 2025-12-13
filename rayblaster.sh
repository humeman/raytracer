#!/bin/bash

set -e

# ----- Config -----
HOSTS=("local" "node1" "camden-laptop")
ARGS="-s 100 -w 8"
OUT="out.png"
# ------------------

NUM_HOSTS=${#HOSTS[@]}
declare -a STATUS
declare -a FILENAMES

for (( i=0; i<NUM_HOSTS; i++ )); do # Had no idea you could do this in Bash. Neat
    STATUS[$i]=0
    FILENAMES[$i]="out_f$((i+1)).png"
done

echo "RAYBLASTER (tm) (c) (r) (patent pending)"
echo "- Building"
make clean > /dev/null 2>&1
make > /dev/null 2>&1
echo "  - Done"

echo "- Starting remote workers"
HAS_LOCAL=0
for (( i=0; i<NUM_HOSTS; i++ )); do
    HOST=${HOSTS[$i]}
    FRAC_I=$((i + 1))
    echo "  - $HOST"
    if [ "$HOST" == "local" ]; then
        tmux new-session -d -s raytracer -n raytracer 2>/dev/null || \
        tmux new-window -t raytracer -n raytracer 2>/dev/null || \
        true;
        tmux send-keys -t raytracer:raytracer "./raytracer -f ${FRAC_I}/${NUM_HOSTS} $ARGS out_f1.png" C-m
        echo "    - Started raytracer (f${FRAC_I}/${NUM_HOSTS})"
        continue
    fi
    scp raytracer $HOST:~/raytracer/raytracer > /dev/null 2>&1
    echo "    - Copied latest binary"
    ssh "$HOST" "
        tmux new-session -d -s raytracer -n raytracer 2>/dev/null || \
        tmux new-window -t raytracer -n raytracer 2>/dev/null || \
        true;

        tmux send-keys -t raytracer:raytracer 'cd ~/raytracer && ./raytracer -f ${FRAC_I}/${NUM_HOSTS} $ARGS out_f${FRAC_I}.png' C-m
    " > /dev/null 2>&1
    echo "    - Started raytracer (f${FRAC_I}/${NUM_HOSTS})"
done
echo "  - Done"

echo "- Waiting for completion"
while true; do
    for (( i=0; i<NUM_HOSTS; i++ )); do
        HOST=${HOSTS[$i]}
        FILENAME=${FILENAMES[$i]}
        if [ "$HOST" == "local" ]; then
            if [ ${STATUS[$i]} -eq 0 ] && [ -f "$FILENAME" ]; then
                STATUS[$i]=1
                echo "  - $HOST is done"
            fi
        else
            if [ ${STATUS[$i]} -eq 0 ]; then
                scp "$HOST:~/raytracer/$FILENAME" "$FILENAME" > /dev/null 2>&1 || true
                if [ -f "$FILENAME" ]; then
                    STATUS[$i]=1
                    ssh "$HOST" "rm ~/raytracer/$FILENAME" > /dev/null 2>&1 || true
                    echo "  - $HOST is done"
                fi
            fi
        fi
    done
    ALL_DONE=1
    for (( i=0; i<NUM_HOSTS; i++ )); do
        if [ ${STATUS[$i]} -eq 0 ]; then
            ALL_DONE=0
        fi
    done
    if [ $ALL_DONE -eq 1 ]; then
        break
    fi
    sleep 5
done

echo "  - Done"
echo "- Joining images"
ARGS=""
for (( i=0; i<NUM_HOSTS; i++ )); do
    FILENAME=${FILENAMES[$i]}
    ARGS="$ARGS $FILENAME"
done
./joiner $ARGS $OUT > /dev/null 2>&1
echo "  - Cleaning up"
rm $ARGS
echo "  - Done"
echo "- Written to $OUT"
