\+DatabaseType StoryNotes
\ver 5.0
\+mkrset 
\lngDefault Default
\mkrRecord c

\+mkr apr
\nam Approval date
\lng Default
\mkrOverThis c
\-mkr

\+mkr bld
\nam Bold note
\lng Default
\+fnt 
\Name Arial
\Size 8
\Bold
\charset 00
\rgbColor 0,0,255
\-fnt
\mkrOverThis c
\CharStyle
\-mkr

\+mkr c
\nam Chapter
\lng Default
\-mkr

\+mkr cat
\nam Category
\lng International
\mkrOverThis c
\-mkr

\+mkr co
\nam Comment
\lng International
\mkrOverThis c
\-mkr

\+mkr con
\nam Consultant
\lng International
\+fnt 
\Name Comic Sans MS
\Size 8
\charset 00
\rgbColor 0,0,255
\-fnt
\mkrOverThis c
\-mkr

\+mkr dt
\nam Date
\lng Date
\mkrOverThis c
\CharStyle
\-mkr

\+mkr fac
\nam Facilitator
\lng International
\mkrOverThis c
\-mkr

\+mkr ln
\nam Line number
\lng International
\+fnt 
\Name Arial
\Size 8
\Bold
\charset 00
\rgbColor 128,0,0
\-fnt
\mkrOverThis c
\-mkr

\+mkr p
\nam *
\lng Default
\mkrOverThis c
\-mkr

\+mkr t
\nam Title
\lng International
\+fnt 
\Name Arial
\Size 8
\Bold
\charset 00
\rgbColor 0,0,0
\-fnt
\mkrOverThis c
\-mkr

\+mkr v
\nam *
\lng Default
\mkrOverThis c
\-mkr

\-mkrset

\iInterlinCharWd 8
\+filset 

\-filset

\+jmpset 
\+jmp BT
\+mkrsubsetIncluded 
\mkr ln
\-mkrsubsetIncluded
\+drflst 
\-drflst
\-jmp
\-jmpset

\+template 
\fld \t TITLE
\fld \cat ConNotes -- (consultant's name)
\fld \co Project name
\fld \co (facilitator's name)\n
\fld \con dd/mmm/yy = XX1
\fld \p
\fld \con INFO / BOOKKEEPING:
\fld \con STORY FLOW:
\fld \con TRANS ISSUES:
\fld \con ANCHORS / LINE LENGTH / EXEG:
\fld \con KEY TERMS / L&C NOTES:
\fld \con INFERENCE TESTING:
\fld \con RETELLING TESTING:
\fld \p
\fld \ln xx.01
\fld \con Consultant comment
\fld \ln xx.02
\fld \ln xx.03
\fld \ln xx.04
\fld \ln xx.05
\fld \ln xx.06
\fld \ln xx.07
\fld \ln xx.08
\fld \ln xx.09
\fld \ln xx.10
\fld \ln xx.11
\fld \ln xx.12
\fld \ln xx.13
\fld \ln xx.14
\fld \ln xx.15
\fld \ln xx.16
\fld \ln xx.17
\fld \ln xx.18
\fld \ln xx.19
\fld \ln xx.20
\fld \ln xx.21
\fld \ln xx.22
\fld \ln xx.23
\fld \ln xx.24
\fld \ln xx.25
\fld \ln xx.26
\fld \ln xx.27
\fld \dt 21/Sep/2009
\-template
\mkrRecord c
\mkrDateStamp dt
\mkrTextRef ln
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
\exportedFile L:\Pahari\Storying\CLS Recurrency\RecurrencyConNotes.TXT
\OpenExportedFile
\-expSF

\expDefault Standard Format
\-expset
\-DatabaseType
