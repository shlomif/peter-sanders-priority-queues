ADDED:
win32 compatibility (yay windows)

convert knupdown3.C into an actual benchmark :)


```
CPU:

	Intel(R) Pentium(R) Silver N6000 @ 1.10GHz

	Base speed:	1.11 GHz
	Sockets:	1
	Cores:	4
	Logical processors:	4
	Virtualization:	Enabled
	L1 cache:	256 KB
	L2 cache:	1.5 MB
	L3 cache:	4.0 MB

	Speed	3.01 GHz
```

10 million keys (10,000,000)

```
r = random
s = sequence
rr = random, reversed
rs = sequence, reversed
all fields (except for iteration) are shown in seconds

iteration  |      keys  |  insert (r)  |  delete (r)  |  insert (rr)  |  delete (rr)  |  insert (s)  |  delete (s)  |  insert (rs)  |  delete (rs)
   warmup  |  10000000  |       1.155  |       0.422  |        1.124  |        0.401  |       0.752  |       0.376  |         0.84  |        0.392
        1  |  10000000  |       1.144  |        0.42  |        1.144  |        0.414  |       0.744  |       0.394  |        0.868  |        0.403
        2  |  10000000  |       1.104  |       0.423  |        1.129  |        0.424  |       0.762  |       0.402  |        0.812  |        0.401
        3  |  10000000  |        1.13  |       0.429  |        1.102  |        0.424  |       0.789  |       0.406  |        0.845  |          0.4
        4  |  10000000  |       1.097  |       0.423  |        1.106  |         0.43  |         0.8  |       0.404  |        0.846  |          0.4
        5  |  10000000  |       1.112  |       0.417  |        1.115  |        0.427  |       0.795  |         0.4  |        0.857  |        0.435
        6  |  10000000  |       1.175  |       0.431  |        1.116  |        0.399  |       0.785  |       0.408  |        0.831  |        0.429
        7  |  10000000  |       1.113  |        0.42  |        1.137  |        0.431  |       0.791  |        0.42  |        0.846  |        0.405
        8  |  10000000  |       1.111  |       0.454  |        1.129  |        0.432  |       0.798  |       0.396  |        0.839  |        0.396
        9  |  10000000  |       1.111  |       0.448  |        1.128  |        0.407  |        0.79  |       0.388  |        0.822  |        0.415
       10  |  10000000  |       1.113  |       0.429  |        1.099  |        0.422  |       0.794  |       0.408  |        0.839  |        0.406
       11  |  10000000  |        1.11  |       0.386  |        1.131  |        0.448  |       0.785  |       0.412  |        0.859  |        0.424
       12  |  10000000  |       1.117  |       0.428  |        1.118  |        0.416  |       0.786  |       0.434  |        0.858  |        0.421
       13  |  10000000  |       1.087  |       0.417  |        1.131  |        0.432  |       0.789  |       0.399  |        0.851  |        0.423
       14  |  10000000  |       1.122  |       0.442  |        1.099  |        0.425  |        0.81  |       0.417  |        0.821  |        0.405
       15  |  10000000  |       1.147  |       0.448  |         1.08  |        0.431  |       0.777  |       0.416  |        0.823  |        0.408
       16  |  10000000  |        1.11  |       0.421  |        1.088  |        0.459  |        0.79  |       0.392  |        0.866  |        0.417
       17  |  10000000  |       1.082  |       0.451  |        1.169  |        0.413  |       0.772  |        0.43  |        0.798  |        0.402
       18  |  10000000  |       1.085  |        0.45  |        1.161  |        0.424  |       0.772  |        0.41  |        0.829  |        0.397
       19  |  10000000  |       1.112  |       0.423  |        1.102  |        0.428  |       0.783  |       0.398  |        0.844  |        0.402
       20  |  10000000  |       1.091  |       0.459  |        1.078  |        0.412  |       0.747  |       0.416  |        0.815  |        0.441
```

This is a maintenance and update version of Peter Sanders’ code from this
paper:

http://www.mpi-inf.mpg.de/~sanders/papers/spqjea.ps.gz

It was originally found here:

http://www.mpi-inf.mpg.de/~sanders/programs/spq/

Prof. Sanders has kindly licensed it under the Boost Software License
open source license:

http://en.wikipedia.org/wiki/Boost_Software_License

Note that a modified version of some of this code is available for TPIE
here:

http://madalgo.au.dk/Trac-tpie/ticket/15

Regards,

    Shlomi Fish, shlomif@cpan.org, http://www.shlomifish.org/ .
