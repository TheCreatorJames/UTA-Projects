# Compile Instructions 
--- 

The following will compile the code with default options: 
```
g++ kfs.c -lpthread 
```

This code can be compiled/configured with other options though (appending -D flagname).

Flag             | Explanation
----             | ---- 
RANDOMIZE        | (Recommended) Shuffles the ordering of the parts that are given out. (For example, Kerbal 1 won't just grab parts "1 1 1", but might instead grab parts "5 3 2"). This allows the user to see that the parts actually are being tracked and exchanged between kerbals. Adds Uniqueness. 
Unsafe           | Removed the lock around the resource requests. This could potentially allow one Kerbal to rush past another one. In practice, this hasn't caused deadlocks, but caveat emptor.
releaseAtLanding | Allows you to specify that you want the Kerbal to launch AFTER the other Kerbal has landed.
OneTime          | Tells the Kerbals to only launch once, rather than continue in an infinite loop.. 

You may also specify how many rockets can launch simultaneously, using 
```
g++ kfs.c -lpthread -DLAUNCH_MAX=4
```
Replacing 4 with whatever the maximum you want it to be. 

So for example, combining some of these options, you can request 
```
g++ kfs.c -lpthread -DLAUNCH_MAX=2 -D RANDOMIZE -D OneTime
```
for a different experience.


This code can also compile on Windows with a Mingw compiler. 