# x16-vga
VGA-style demo for Commander X16

# Build and run demos
In bash (use Git bash shell if on Windows):
<pre>
$ ./build_tools.sh
$ ./run_basic_demo.sh brillig.data
$ ./run_banked_demo.sh brillig.data
</pre>

Note: Run scripts work best with defined environment variables. Windows requires %X16EMU% to be defined as the X16 emulator executable. For Linux/Unix/Mac, you can set $X16EMU to the executable, $X16EMU_HOME to its directory, or just run assuming that the emulator executable is ../x16-emulator/x16emu.

![](brillig.gif)<br>
"'Twas Brillig" by Matt Heffernan, 2006<br>
Pastel on concrete, inspired by Sir John Tenniel (1820-1914)
