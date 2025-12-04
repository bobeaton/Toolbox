\+DatabaseType KeyTermsDic
\ver 5.0
\desc Key Terms Dictionary for Bible Stories
\+mkrset 
\lngDefault Default
\mkrRecord r

\+mkr co
\nam comment
\lng Default
\+fnt 
\Name Verdana
\Size 8
\Italic
\charset 00
\rgbColor 0,0,0
\-fnt
\mkrOverThis r
\-mkr

\+mkr d
\nam *
\lng Default
\mkrOverThis r
\-mkr

\+mkr dt
\nam Date
\lng Date
\mkrOverThis r
\CharStyle
\-mkr

\+mkr nm
\nam line number
\lng Book References
\mkrOverThis r
\CharStyle
\-mkr

\+mkr r
\nam record marker
\lng Default
\+fnt 
\Name Verdana
\Size 8
\Bold
\charset 00
\rgbColor 0,0,0
\-fnt
\-mkr

\+mkr ref
\nam references
\lng Default
\+fnt 
\Name Verdana
\Size 8
\charset 00
\rgbColor 255,0,0
\-fnt
\mkrOverThis r
\CharStyle
\-mkr

\+mkr use
\nam usage--meaning
\lng Default
\+fnt 
\Name Verdana
\Size 8
\Italic
\charset 00
\rgbColor 0,128,0
\-fnt
\mkrOverThis r
\CharStyle
\-mkr

\+mkr why
\nam why-used
\lng Default
\+fnt 
\Name Verdana
\Size 8
\Italic
\charset 00
\rgbColor 0,128,0
\-fnt
\mkrOverThis r
\CharStyle
\-mkr

\+mkr wrd
\nam word
\lng Default
\+fnt 
\Name Verdana
\Size 8
\charset 00
\rgbColor 0,0,0
\-fnt
\mkrOverThis r
\CharStyle
\-mkr

\-mkrset

\iInterlinCharWd 8
\+filset 

\-filset

\+jmpset 
\+jmp KeyTerms
\+mkrsubsetIncluded 
\mkr r
\mkr ref
\-mkrsubsetIncluded
\+drflst 
\-drflst
\match_char i
\-jmp
\-jmpset

\+template 
\fld \wrd
\fld \use
\fld \why
\fld \ref
\-template
\mkrRecord r
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
\-expSF

\SkipProperties
\-expset
\-DatabaseType
