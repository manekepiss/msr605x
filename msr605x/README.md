+--------------+
| Introduction |
+--------------+
The msr605x is a very simple program that encapsulates characters
into USB packets to be sent through USB connection to the MSR605X card
reader/writer. There are two hardware models that use USB connection but
physically have a different USB class.
Devices MSR605 use USB and identify as PL2303 serial port UART and
are handled by linux driver pl2303 and list as /dev/ttyUSB devices.
Devices MSR605X use USB and identify as HID device and are handled by
linux driver usbhid and list as /dev/hidraw devices.
The msr605x program makes it possible to directly communicate
with the MSR605X as if it where a MSR605 through a USB serial port. 
The MSR605X also uses the same set of communication commands as described 
in programming manual msr605.pdf. Beware that the program msr605x does not
replicate the behaviour of a serial port.

+--------------+
| Requirements |
+--------------+
A computing device:
 - Kernel linux running
 - Module usbhid loaded
 - C compiler available
 - USB port available
 

+---------------------+
| Testing the MSR605X |
+---------------------+
I assume that if you are reading this file, you have downloaded all
the files. Run the following commands in the linux console:

cc -o msr605x msr605x_hid_wrapper.c; 
./msr605x /dev/hidraw0;

Type ^[v ( esc character letter v). This is the command for firmware 
version. It should return something similar to this: EVH7.31
Alternativelly, if typing character <ESC> is impossible, do the
following:
echo -ne "\x1bv" | ./msr605x /dev/hidraw0;

The term "\x1bv" means " [ESC] v ". You shall find it often throughout
this file. To end the program msr605x type CTRL-C.

The msr605x program accepts messages from STDIN (keyboard or a file) and writes
those messages to the MSR605X using usbhid linux driver.
The msr605x program reads messages from usbhid driver and writes those messages
to STDOUT.

Now, what you have been waiting for: CLONING A CARD
##!/bin/bash
#This is a script example of how to make use of the msr605x both for
#writing and reading a card.
#NOTICE: Be carefull will you? I accidentally erased my current bank card and made
#it useless. I recommend that you practice with an expired bankcard or any
#card that is no longer necessary. Bear in mind that handling cards
#that are not your own even if expired, not excluding reading them or
#saving the data in your computer, may be seen as an offense.

#Check that MSR605X is online
echo -ne "\x1bv" | msr605x /dev/hidraw0; 

#Reset it
echo -ne "\x1ba" | msr605x /dev/hidraw0;

#Set hi-co. To write/erase my own card hi-co must be set. See exception bellow.
echo -ne "\x1bx" | msr605x /dev/hidraw0;

#Place write command at begin of file clone_card.txt.
echo -ne "\x1bw" > clone_card.txt;

#Read original card and add it to clone_card.txt
echo -ne "\x1br" | msr605x /dev/hidraw0 >> clone_card.txt;

#Write the clone card with the command prepared in clone_card.txt.
./msr605x /dev/hidraw0 < clone_card.txt;

#Go to the nearest cashpoint and see if it works.

#END


+------+
| TODO |
+------+
There may be bugs in this program which I would try to correct if
they are brought to my attention. I dont plan to make any
further development. I also dont recommend making any changes to
msr605x. This program is so small and simple that can be included in
your own program if you want to write one. msr650x  works lovelly,
many thanks to the operating system!


+------------------------------+
| The USB HID packet explained |
+------------------------------+
Each product handles the HID packet for different purposes according
to the function of the device itself. The file hid_gadget_test.c program shows 
examples of this with keyboards, mice and joysticks.
Regarding the MSR605X, the structure of the HID packet is as follows:
PKT[0] = HID report number. MSR605X is happy with it always 0x00.
PKT[1] = 0x80 | 0xNN 
PKT[1] = 0X00 | 0xNN
PKT[1] = 0x60 | 0xNN where 0xNN is the lenght of the packet up to 63 characters.
0x80 corresponds to the first packet of the message.
0x00 corresponds to intermediate packet of the message.
0x60 corresponds to the last packet of the message, considering that
there isn't a length limit for a message.
But it is also possible splitting a message in shorter messages up to
63 characters long, where PKT[1] = 0xC0. Meaning that the transmitted packet 
is the first and last from the shorter message and, this way avoiding some 
more complex flowcontrol.


+--------------------------------------+
| Exceptions and unexpected situations |
+--------------------------------------+
1 - Persistently not writing or erasing, it is possible that Hi-Co must be set first.
echo -ne "\x1bx" | msr605x /dev/hidraw0;
The command <Esc> x shall do the job.


+------------+
| References |
+------------+
hid-example.c - Alan Ott
hid_gadget_test.c - Unknown
msr605.pdf - Unknown
MSRX Windows - Deftun
Wireshark - Debian
SniffUSB - Benoit Pappillaut
Windows XP - Microsoft

Copyright:
Use, abuse, distribute