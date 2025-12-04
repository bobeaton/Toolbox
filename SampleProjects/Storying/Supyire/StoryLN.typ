\+DatabaseType StoryLN
\ver 5.0
\desc BT and Anchors. Temporary--for converting from PTX
\+mkrset 
\lngDefault Book References
\mkrRecord ln

\+mkr V
\nam *
\lng Book References
\mkrOverThis c
\-mkr

\+mkr an
\nam *
\lng Book References
\mkrOverThis c
\-mkr

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
\mkrOverThis nm
\-mkr

\+mkr c
\lng Default
\mkrOverThis ln
\-mkr

\+mkr cat
\nam Catetory
\lng Default
\mkrOverThis name
\CharStyle
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

\+mkr con
\nam *
\lng Book References
\mkrOverThis ln
\-mkr

\+mkr dt
\nam *
\lng Book References
\mkrOverThis c
\-mkr

\+mkr fac
\nam *
\lng Book References
\mkrOverThis ln
\-mkr

\+mkr id
\nam ID of Text
\lng English
\mkrOverThis name
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
\-mkr

\+mkr name
\nam Name of Text
\lng English
\mkrOverThis c
\-mkr

\+mkr nm
\nam Number
\lng Book References
\+fnt 
\Name Times New Roman
\Size 10
\charset 00
\rgbColor 128,128,128
\-fnt
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
\mkrOverThis name
\-mkr

\+mkr t
\nam *
\lng Book References
\mkrOverThis c
\-mkr

\+mkr v
\lng Default
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
\mkrRecord ln
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
\cctFile L:\Pahari\Storying\merge.cct
\OverwriteOutputFile
\-expSF

\expDefault Standard Format
\SkipProperties
\-expset
\+numbering 
\mkrRef ref
\mkrTxt v
\+subsetTextBreakMarkers 
\+mkrsubsetIncluded 
\mkr v
\-mkrsubsetIncluded
\-subsetTextBreakMarkers
\-numbering
\-DatabaseType
