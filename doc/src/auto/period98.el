(TeX-add-style-hook "period98"
 (function
  (lambda ()
    (LaTeX-add-labels
     "HELP_COPYRIGHT"
     "HELP_INTRODUCTION"
     "HELP_HOW_TO_USE"
     "HELP_DETAIL"
     "HELP_BUGS")
    (TeX-add-symbols
     '("myitemize" 1)
     '("normskip" 0)
     '("myfoot" 3)
     '("myhead" 3)
     '("note" 1)
     '("button" 1)
     '("menuentry" 1)
     '("menu" 1)
     '("periodbinaries" 0)
     '("toedit" 0)
     '("periodunixversion" 0)
     '("periodntftp" 0)
     '("wxxtutilsurl" 0)
     '("wxxtutils" 0)
     '("wxxtliburl" 0)
     '("wxxtlib" 0)
     '("wxxtbase" 0)
     '("periodfull" 0)
     '("period" 0)
     '("periodname" 0)
     '("STACCHOME" 0)
     '("WETHOME" 0)
     '("DSNHOME" 0)
     '("wxxtftp" 0)
     '("periodnt" 0)
     '("periodmail" 0)
     '("periodhome" 0)
     '("periodftpincoming" 0)
     '("periodftp" 0)
     '("wxver" 0)
     '("version" 0)
     '("periodnamens" 0)
     "periodsrc"
     "periodsrcurl"
     "perioddoc"
     "perioddocurl")
    (TeX-run-style-hooks
     "latex2"
     "bk10"
     "book"
     "a4"
     "twoside"
     "makeidx"
     "verbatim"
     "texhelp"
     "fancyhea"
     "mysober"
     "mytitle"
     "psbox"
     "copyright"
     "intro"
     "install"
     "using"
     "d_basic"
     "d_timestr"
     "d_period"
     "d_fourier"
     "d_log"
     "bugs"
     "faq"))))

