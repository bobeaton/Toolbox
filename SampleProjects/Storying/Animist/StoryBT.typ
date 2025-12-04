\+DatabaseType StoryBT
\ver 5.0
\desc BT and Anchors. Tracks with ConNotes
\+mkrset 
\lngDefault Default
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
\nam Chapter number (record marker)
\lng Default
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

\+mkr dt
\nam Date
\lng Date
\mkrOverThis c
\-mkr

\+mkr ln
\nam Line number
\lng Book References
\+fnt 
\Name Verdana
\Size 8
\Bold
\charset 00
\rgbColor 128,0,0
\-fnt
\mkrOverThis c
\-mkr

\+mkr old
\nam *
\lng Default
\mkrOverThis c
\Hide
\-mkr

\+mkr pict
\nam *
\lng Default
\mkrOverThis c
\-mkr

\+mkr play
\nam *
\lng Default
\mkrOverThis c
\-mkr

\+mkr sound
\nam *
\lng Default
\mkrOverThis c
\-mkr

\+mkr t
\nam Title
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

\+mkr ver
\nam *
\lng Default
\mkrOverThis c
\-mkr

\-mkrset

\iInterlinCharWd 8
\+filset 

\-filset

\+jmpset 
\+jmp Keyterms
\+mkrsubsetIncluded 
\mkr bt
\-mkrsubsetIncluded
\+drflst 
\+drf 
\File L:\Pahari\Storying\Animist\AnimistKeyTerms.txt
\mkr r
\-drf
\-drflst
\match_char p
\-jmp
\+jmp LineNumber
\+mkrsubsetIncluded 
\mkr ln
\-mkrsubsetIncluded
\+drflst 
\-drflst
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
\fld \t TITLE
\fld \co Lg. name or abbrev.
\fld \co Reason for having this story in the set
\fld \co Story crafter's name or initials
\fld \co Backtranslator's name or initials
\fld \ln 02.01
\fld \bt
\fld \anc
\fld \cn
\fld \ln 02.02
\fld \bt
\fld \anc
\fld \ln 02.03
\fld \bt
\fld \anc
\fld \ln 02.04
\fld \bt
\fld \anc
\fld \ln 02.05
\fld \bt
\fld \anc
\fld \ln 02.06
\fld \bt
\fld \anc
\fld \ln 02.07
\fld \bt
\fld \anc
\fld \ln 02.08
\fld \bt
\fld \anc
\fld \ln 02.09
\fld \bt
\fld \anc
\fld \ln 02.10
\fld \bt
\fld \anc
\fld \ln 02.11
\fld \bt
\fld \anc
\fld \ln 02.12
\fld \bt
\fld \anc
\fld \ln 02.13
\fld \bt
\fld \anc
\fld \ln 02.14
\fld \bt
\fld \anc
\fld \ln 02.15
\fld \bt
\fld \anc
\fld \ln 02.16
\fld \bt
\fld \anc
\fld \ln 02.17
\fld \bt
\fld \anc
\fld \ln 02.18
\fld \bt
\fld \anc
\fld \ln 02.19
\fld \bt
\fld \anc
\fld \ln 02.20
\fld \bt
\fld \anc
\fld \dt 09/Oct/2006
\-template
\mkrRecord c
\mkrDateStamp dt
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
\exportedFile L:\Pahari\Storying\Animist\Animist14dec06.rtf
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
\exportedFile L:\Pahari\Storying\Individual Toolbox Files\EX find replace.txt
\OverwriteOutputFile
\-expSF

\expDefault Standard Format
\CurrentRecord
\-expset
\-DatabaseType
