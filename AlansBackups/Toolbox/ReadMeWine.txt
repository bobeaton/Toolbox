Using Toolbox in Linux under Wine

Toolbox works best with Wine version 20050725 or 
newer. If you have an  older version of Wine, you can 
download a new version from  www.winehq.com.

To install under KDE desktop:
1. Login as root.
2. Double click on ToolboxWine.zip to enter it. 
(Or single click if your mouse is set that way.) 
You will see a folder named ToolboxWine.
3. Copy the ToolboxWine folder to /usr/share.
4. Login as a normal user.
5. Navigate to /usr/share/ToolboxWine.
6. To test Toolbox, double click on Toolbox.exe. 
You should see Toolbox open with a dialog box 
saying "No project open". (If Toolbox does not 
open, see further instructions below.)
7. In Toolbox, choose "Exit" to close Toolbox.
8. Drag Toolbox.exe to the desktop, and 
choose "Link here".

You can run Toolbox by clicking the icon on the 
desktop. It will automatically open the most recent 
project.

In step 6, If Toolbox does not open, then right click 
on Toolbox.exe and choose Open With. Type "wine" 
and choose OK. Toolbox should open. If this works, 
then right click and do Open With, Wine again and 
this time check the box that says "Remember 
application association for this type of file".

If you get a message saying "Not executable",
then do right-click on Toolbox.exe, choose Properties,
Permissions, Execute, and click on "Allow executing
file as program".

Installation under Gnome desktop should work the 
same except for minor differences in wording of its 
messages. To put a link on the desktop in Gnome, 
right click on Toolbox.exe and choose Make Link. 
Drag the resulting link icon to the desktop.

If you are familiar with Linux and want to put 
Toolbox on the menu or make shell scripts to open 
particular projects, you may do so. I chose this way 
of running Toolbox because to me it seems the 
easiest and most straightforward.

Notes on running Toolbox:

In general, Toolbox seems to run the same as on 
Windows. Projects can move between Linux and
Windows, and should run the same on both.

The file usp10.dll is the Windows Uniscribe 
rendering engine. It is installed in the same folder 
with Toolbox because Toolbox has to have it 
available, even if it doesn't use it. If you use a 
Windows Unicode font, Toolbox will render the 
same as it does on Windows. (The file usp10.dll 
is a free download from Microsoft.)

It appears to be possible to use a Windows TrueType 
font on Linux. I found that when I put such a font in /usr/X11R6/lib/X11/fonts/TTF, then Toolbox was 
able to use it without any further installation required.

Toolbox help is not currently functional under 
Linux Wine. For this reason, the Toolbox.hlp file 
is not included in the Linux download. If you have a 
copy of Toolbox.hlp from a Windows installation, 
you may be able to view it in Linux by running 
"wine winhelp" and then choosing File, Open to 
open Toolbox.hlp.

