run_timings=true
if ${run_timings}; then
    make clean
    make -j

    for p in 3 4 5 6 7 8 9 10 11 12 13 14 15 16
    do
        for E in 1 2 4 8 16 32 64 128 256 512 1024 2048 4096 8162 16384
        do
                ./fast_diagonalization ${p} ${E} 1000 >> results.csv
        done
    done
fi
