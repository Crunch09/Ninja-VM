Main_main:
    asf 1
    new 0
    .addr VMT_C
    popl 0
    pushl 0
    vmcall 1,1
    drop 1
    rsf
    halt


A_m:
    asf 0
    pushc 1
    wrint
    rsf
    ret

VMT_A:
    .addr A_m

B_m:
    asf 0
    pushc 2
    wrint
    rsf
    ret

B_test:
    asf 0
    pushl -3
    vmcall 1,0
    drop 1
    pushl -3
    call A_m
    drop 1
    rsf
    ret

VMT_B:
    .addr B_m
    .addr B_test


C_m:
    asf 0
    pushc 3

    wrint
    rsf
    ret

VMT_C:
    .addr C_m
    .addr B_test
