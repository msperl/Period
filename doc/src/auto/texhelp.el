(TeX-add-style-hook "texhelp"
 (function
  (lambda ()
    (LaTeX-add-environments
     '("indented" 1)
     "helpglossary"
     "twocollist"
     "toocomplex")
    (TeX-add-symbols
     '("overview" 2)
     '("textcolour" 1)
     '("background" 1)
     '("registered" 0)
     '("rbraceraw" 0)
     '("lbraceraw" 0)
     '("backslashraw" 0)
     '("settransparency" 1)
     '("sethotspotunderline" 1)
     '("sethotspotcolour" 1)
     '("bcol" 2)
     '("fcol" 2)
     '("definecolor" 4)
     '("definecolour" 4)
     '("normalboxd" 1)
     '("normalbox" 1)
     '("ruledrow" 1)
     '("row" 1)
     '("twocolitemruled" 2)
     '("twocolitem" 2)
     '("twocolspacing" 1)
     '("twocolwidthb" 1)
     '("twocolwidtha" 1)
     '("marginparodd" 1)
     '("marginpareven" 1)
     '("setfooter" 6)
     '("setheader" 6)
     '("imagemap" 3)
     '("imagel" 2)
     '("imager" 2)
     '("image" 2)
     '("gloss" 1)
     '("sectionheading" 1)
     '("chapterheading" 1)
     '("myheading" 1)
     '("helpinput" 1)
     '("htmlonly" 1)
     '("htmlignore" 1)
     '("xlponly" 1)
     '("xlpignore" 1)
     '("winhelponly" 1)
     '("winhelpignore" 1)
     '("rtfonly" 1)
     '("rtfignore" 1)
     '("latexonly" 1)
     '("latexignore" 1)
     '("helponly" 1)
     '("helpignore" 1)
     '("helpfontfamily" 1)
     '("helpfontsize" 1)
     '("footnotepopup" 2)
     '("popref" 2)
     '("helprefn" 2)
     '("urlref" 2)
     '("helpfigref" 2)
     '("helpref" 2)
     '("destruct" 1)
     '("class" 1)
     '("cparam" 2)
     '("param" 2)
     '("member" 2)
     '("functionsection" 1)
     '("clipssection" 1)
     '("clipsfunc" 3)
     '("membersection" 1)
     '("pfunc" 3)
     '("func" 3)
     "void"
     "cinsert"
     "cextract"
     "rtfsp"
     "twocolwidthaval"
     "twocolwidthbval"
     "twocolspacingval"))))

