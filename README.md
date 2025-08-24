# some dumb tiny one-source-file programs i made in my free time (trash code warning (probably))
**all of these i put under the mit licence**

on linux/unix (including bsd) systems make sure to `chmod +x [name of the binary]` to make it work instead of getting 'permission denied'

how to build: literally just `g++ $file.cpp` should work for linux (and maybe other unix systems like bsd)<br>
for windows idk tbh but some may not work because of system things like syscalls<br>
i made these programs on linux so it probably will compile and work for bsd (and other unix systems), but for windows ymmv<br>
the programs compile for and work on both glibc and musl linux systems<br>
<br>
tell me if my code sucks, i want to learn how to make it not suck

## which binary should i download?
LINUX:
 - architecture: if you use a desktop or laptop from the past 20 years and an intel/amd cpu you probably have x86_64 (aka amd64). if your computer is more ancient you might have x86 (i386-i686). if you use a phone or raspberry pi or your cpu is not intel/amd you probably have aarch64 (aka arm64). if you have another arch you probably know it<br>
 - c library: if you use alpine or chimera you have musl, if you use void or gentoo you probably chose glibc but just check to be sure, if you have some other distro you probably have glibc<br>
FREEBSD: there's only amd64 for now but i might make aarch64 builds if there is demand (there probably won't be)<br>
WINDOWS: if your computer is from the past 20 years and your cpu is intel/amd you very likely have x86_64, but i686 will work on these as well. if your computer is more ancient you probably have x86 (i386-i686). if your cpu is not intel/amd (e.g. qualcomm) you (definitely?) have aarch64 so build the binary yourself<br>
### No Mac because<br>
![tom searching for such a cross-compiler](https://i.imgur.com/4uIOScq.gif)
