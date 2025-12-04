\+DatabaseType StoryBT
\ver 5.0
\desc BT and Anchors. Tracks with ConNotes
\+mkrset 
\lngDefault Book References
\mkrRecord c

\+mkr anc
\nam Anchor
\lng Default
\+fnt 
\Name Verdana
\Size 8
\charset 00
\rgbColor 255,0,0
\-fnt
\mkrOverThis bt
\Hide
\-mkr

\+mkr bt
\nam Back translation
\lng Default
\+fnt 
\Name Verdana
\Size 10
\Bold
\charset 00
\rgbColor 0,0,0
\-fnt
\mkrOverThis c
\-mkr

\+mkr c
\lng Default
\-mkr

\+mkr cat
\nam Catetory
\lng Default
\mkrOverThis c
\-mkr

\+mkr chk
\nam Consultant checked
\lng Book References
\+fnt 
\Name Verdana
\Size 9
\Bold
\Italic
\charset 00
\rgbColor 0,0,0
\-fnt
\mkrOverThis c
\-mkr

\+mkr cn
\nam Crafting Note
\lng Default
\+fnt 
\Name Verdana
\Size 8
\charset 00
\rgbColor 0,0,0
\-fnt
\mkrOverThis bt
\Hide
\-mkr

\+mkr co
\nam Comment
\lng Default
\+fnt 
\Name Verdana
\Size 9
\Italic
\charset 00
\rgbColor 0,0,0
\-fnt
\mkrOverThis c
\-mkr

\+mkr d
\nam *
\lng Book References
\mkrOverThis c
\-mkr

\+mkr dt
\nam *
\lng Book References
\mkrOverThis c
\-mkr

\+mkr fac
\nam *
\lng Book References
\mkrOverThis c
\-mkr

\+mkr id
\nam ID of Text
\lng English
\mkrOverThis c
\-mkr

\+mkr ln
\nam Line number
\lng Book References
\+fnt 
\Name Times New Roman
\Size 10
\charset 00
\rgbColor 128,128,128
\-fnt
\mkrOverThis c
\-mkr

\+mkr omit
\nam *
\lng Book References
\mkrOverThis c
\-mkr

\+mkr p
\nam *
\lng Book References
\mkrOverThis c
\-mkr

\+mkr r
\nam *
\lng Book References
\mkrOverThis c
\-mkr

\+mkr ref
\nam Reference Number
\lng Book References
\+fnt 
\Name Times New Roman
\Size 10
\charset 00
\rgbColor 128,128,128
\-fnt
\mkrOverThis c
\-mkr

\+mkr t
\nam *
\lng Default
\+fnt 
\Name Verdana
\Size 10
\Bold
\charset 00
\rgbColor 0,0,0
\-fnt
\mkrOverThis c
\-mkr

\+mkr voi
\nam *
\lng English
\+fnt 
\Name Arial
\Size 8
\charset 00
\rgbColor 0,0,0
\-fnt
\mkrOverThis c
\-mkr

\-mkrset

\iInterlinCharWd 8
\+filset 

\-filset

\+jmpset 
\+jmp Con Notes
\+mkrsubsetIncluded 
\mkr ln
\-mkrsubsetIncluded
\+drflst 
\-drflst
\-jmp
\+jmp Keyterms
\+mkrsubsetIncluded 
\mkr bt
\-mkrsubsetIncluded
\+drflst 
\+drf 
\File L:\Pahari\Storying\GSH\GSH-L&C Notes.txt
\mkr r
\-drf
\-drflst
\match_char p
\-jmp
\+jmp Net Bible
\+mkrsubsetIncluded 
\mkr anc
\-mkrsubsetIncluded
\+drflst 
\+drf 
\File L:\Pahari\Storying\NETBIBLE.TXT
\mkr v
\-drf
\-drflst
\match_char p
\-jmp
\-jmpset

\+template 
\fld \id \n
\fld \t
\-template
\mkrRecord c
\+PrintProperties 
\header File: &f, Date: &d
\footer Page &p
\topmargin 1.00 in
\leftmargin 0.25 in
\bottommargin 1.00 in
\rightmargin 0.25 in
\recordsspace 10
\-PrintProperties
\+expset 

\+expRTF Rich Text Format
\dotFile L:\Pahari\Storying\GSH.dot
\exportedFile C:\OneStory(Epic)\00--Trainings\VOS--Dec06 Waxhaw\proj handout\GSH-13dec06.rtf
\+rtfPageSetup 
\paperSize letter
\topMargin 1
\bottomMargin 1
\leftMargin 1.25
\rightMargin 1.25
\gutter 0
\headerToEdge 0.5
\footerToEdge 0.5
\columns 1
\columnSpacing 0.5
\-rtfPageSetup
\-expRTF

\+expSF Standard Format
\exportedFile L:\Pahari\Storying\GSH\23 Boldness (Nathan).txt
\OverwriteOutputFile
\-expSF

\expDefault Standard Format
\CurrentRecord
\-expset
\-DatabaseType
