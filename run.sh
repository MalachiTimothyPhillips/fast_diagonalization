# Copy OKL files
cp *.okl ~/

# Test
run_test=false
interactive=false

if ${run_test}; then
    make clean
    make -j

    if ${interactive}; then
        jsrun -n 1 -c 1 -a 1 -g 1 ./fast_diagonalization 8 12
    else
        ./fast_diagonalization 8 4096
    fi
fi

# Timings
run_timings=false

if ${run_timings}; then
    make clean
    make -j

    #for N in 4 5 6 7 8 9 10 11 12
    for N in 8 9 10 11 12
    do
        for E in 1 2 4 8 16 32 64 128 256 512 1024 2048 4096 8162 16384
        do
            for run in 0 1 2
            do
                jsrun -n 1 -c 1 -a 1 -g 1 ./fast_diagonalization ${N} ${E} > timings_N${N}_E${E}_${run}.dat
            done
        done
    done
fi

# NVPROF profiling
run_nvprof=true

if ${run_nvprof}; then
    launcher='jsrun -n 1 -c 1 -a 1 -g 1'

    metrics=''
    metrics=${metrics}' --metrics flop_count_dp'
    metrics=${metrics}' --metrics flop_count_sp'
    metrics=${metrics}' --metrics gld_transactions'
    metrics=${metrics}' --metrics gst_transactions'
    metrics=${metrics}' --metrics l2_read_transactions'
    metrics=${metrics}' --metrics l2_write_transactions'
    metrics=${metrics}' --metrics dram_read_transactions'
    metrics=${metrics}' --metrics dram_write_transactions'
    metrics=${metrics}' --metrics sysmem_read_bytes'
    metrics=${metrics}' --metrics sysmem_write_bytes'

    declare -a kernels=("--kernels \"_occa_fdm_operator_0\"")

    make clean
    make -j

    #for N in 4 5 6 7 8 9 10 11 12
    for N in 4 5 6 7 8 9 10
    do
        for E in 1 2 4 8 16 32 64 128 256 512 1024 2048 4096 8162 16384
        do
            executable='./fast_diagonalization '${N}' '${E}

            for run in 0 1 2
            do
                profiling='nvprof --log-file /gpfs/alpine/scratch/belloma2/csc262/nvprof_N'${N}'_E'${E}'_'${run}'.dat'
    
                for kernel in "${kernels[@]}"
                do
                    profiling=${profiling}' '${kernel}' '${metrics}
                done

                eval ${launcher}' '${profiling}' '${executable} > fdm_N${N}_E${E}_${run}.dat
            done
        done
    done

    mv /gpfs/alpine/scratch/belloma2/csc262/*.dat .
fi
