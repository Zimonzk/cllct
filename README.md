# cllct
##constant length linked comparison table
compares large numbers of numbers of constant length quickly but uses a lot of space

##How it works:
The number gets divided into smaller units (e.g. bytes). For the first number in a file each unit gets as many links as there are numbers that can be represented with that unit. the link of the number wihch this unit actually is filled with the address of the current end of the file. To the end of the file the next set of links will be written but the link which cottesponds to the next unit of the file will now be filled with the address of the ond of the file. this is repeated until the end of the number. All empty links must be zero.
When other numbers already are in the file the link corresponding to the first unit of the number has to be checked for an address. If the address is zeor the address of end of file gehts filled in and the programm continues as when the number s the first number. If the link if nonzero th eprogramm jumps to it and checks the next link corresponding to the next unit. If the last unit in the number is reached and all links have already been set it means the number has already been written to the file.
