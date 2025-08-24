# some dumb tiny single-source-file programs i made in my free time (trash code warning (probably))
**all of these i put under the mit licence**

these are cli applications, you need to start them in a terminal to use them properly (on windows you can use what's called "powershell" or "cmd"/"command prompt")

on linux/unix (including bsd) systems make sure to `chmod +x [name of the binary]` and prefix `./` to the command to make it work instead of getting 'permission denied'

how to build: literally just `g++ $file.cpp` should work for linux (and other unix systems like bsd if no non-standard syscalls/libc-features are used)<br>
for windows idk tbh but some may not work because of system things like syscalls<br>
i made these programs on linux so it probably will compile and work for bsd (and other unix systems), but for windows ymmv<br>
the programs compile for and work on both glibc and musl linux systems<br>
<br>
tell me in issues/PRs if my code sucks, i want to learn how to make it not suck

## what does each program do?
**chargen-ascii:** generates random ascii characters with specified ranges and char count<br>
**dectime:** converts between decimal and sexagesimal ("normal", base-60) time<br>
**grug:** just outputs "Yes, "+arguments+" rhymes with Grug." or without arguments the word "that" in place of arguments. the most useless of these programs<br>
**hangman-blahblahblah:** a basic cli hangman implementation, currently doesn't support non-ascii characters. i totally didn't make this to compete against my brother learning java who i think gave up on his hangman implementation<br>
**spongecase:** converts text to sPoNgEbOb CaSe<br>
**tagsearch:** searches a specified 'taglist' file (or stdin) with a certain syntax for paths with certain tags assigned to them. probably the most useful of these programs. made when i was in belarus<br>
**volume:** (intentionally) awful pactl (pipewire?) cli volume management "tool"

## which binary to download (from releases)?
***ARCHITECTURE:*** on linux/unix (including BSD) you can check with `uname -m`. my attempt at an explanation if you can't use that command or are too lazy: if your computer was made within the past 15-20 years and your cpu is intel/amd you very likely have x86_64, but i686 will work on these as well. if your computer is really ancient you probably have x86 (i386-i686). if your cpu is not intel/amd (e.g. qualcomm) or if you're on a phone or RPi you probably have aarch64 (aka arm64), unless it is extra old in which case you might have one of those armv[number] 32-bit arm arches. if you run a dinosaur-era e.g. DOS computer you might have a 16-bit 8086/8088/whatever processor, don't even get me started on trying to build for this target. if you have a different arch you probably know it<br>
- **LINUX**: c library: if you use alpine or chimera you have musl, if you use void or gentoo you probably chose glibc but musl is a secondary option, if you have some other distro you probably have glibc<br>
- **FREEBSD**: all you really need to know is the arch<br>
- **WINDOWS**: as these are cli applications, you have to run them in cmd/powershell to use them properly!!!! use the x86_64 build (if available) if your computer is "64 bit" and has an intel/amd cpu from the last 15-20 years (it probably does), if the x86_64 build is unavailable you can use the i686 one. if it isn't then you probably have qualcomm (snapdragon) or something, which is aarch64 (the only non-x86-based arch desktop windows runs on atm)<br>
### No Mac because<br>
![tom searching for such a cross-compiler](https://i.imgur.com/4uIOScq.gif)
you can try building it yourself though
