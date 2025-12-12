#!/bin/bash

set -e

echo "MEGARENDERER (tm) (patent pending)"

# Local machine makes frac1
# Wait for it to appear
local_seen=0
node1_seen=0
desktop_seen=0
while [ $local_seen -eq 0 ] || [ $node1_seen -eq 0 ] || [ $desktop_seen -eq 0 ]; do
    echo "checking..."
    if [ $local_seen -eq 0]; then
        if [ -f "out_f1.png" ]; then
            local_seen=1
            echo "local is done"
        fi
    fi

    if [ $node1_seen -eq 0]; then
        scp node1:/home/camden/raytracer/out_f2.png out_f2.png || true
        if [ -f "out_f2.png" ]; then
            node1_seen=1
            echo "node1 is done"
            ssh node1 'rm /home/camden/raytracer/out_f2.png' || true
        fi
    fi

    if [ $desktop_seen -eq 0]; then
        scp desktop:/home/camden/raytracer/out_f3.png out_f3.png || true
        if [ -f "out_f3.png" ]; then
            desktop_seen=1
            echo "desktop is done"
            ssh desktop 'rm /home/camden/raytracer/out_f3.png' || true
        fi
    fi

    sleep 60
done

./joiner out_f1.png out_f2.png out_f3.png out.png
rm out_f1.png out_f2.png out_f3.png