SHELL=/bin/sh

TEX2RTF=tex2rtf

FINALHTML=~Period98/public_html/manual
FINALEPS=~ftp/pub/Period98/current/doc

TEX_FILE=period98
TEX_ADDITIONAL=\
 src/copyright.tex\
 src/intro.tex\
 src/install.tex\
 src/using.tex\
 src/d_basic.tex\
 src/d_timestr.tex\
 src/d_period.tex\
 src/d_fourier.tex\
 src/d_log.tex\
 src/samples.tex\
 src/bugs.tex\
 src/faq.tex\
 src/appendix.tex

XPM_ADDITIONAL_FILES=\
 xpm/back.xpm xpm/forward.xpm xpm/contents.xpm xpm/up.xpm\

XPM_FILES=\
 xpm/TGraph.xpm xpm/TTable.xpm xpm/TAdjust.xpm xpm/TDefault.xpm\
 xpm/TEdit.xpm xpm/THeadings.xpm xpm/TExport.xpm xpm/TImport.xpm\
 xpm/TRename.xpm xpm/TSubdivide.xpm xpm/TFolder.xpm xpm/TChangeQ.xpm\
\
 xpm/FGraph.xpm xpm/FNoiseSpectrum.xpm xpm/FNoise.xpm xpm/FTable.xpm\
 xpm/FZeropoint.xpm xpm/FFolder.xpm xpm/FCalculate.xpm xpm/FRename.xpm\
 xpm/FInclude.xpm\
\
 xpm/PPhase.xpm xpm/PAliasgap.xpm xpm/PAmpvar.xpm xpm/PArtificial.xpm\
 xpm/PEpochs.xpm xpm/PPhaseBinning.xpm xpm/PPhaseFrequency.xpm\
 xpm/PPhaseFrequencyother.xpm xpm/PPredict.xpm xpm/PSpecial.xpm\
 xpm/PFolder.xpm xpm/PCalculate.xpm xpm/PZeropoint.xpm\
\
 xpm/LFolder.xpm\
\
 xpm/GGraphViewport.xpm xpm/GMainTop.xpm xpm/GMainBottom.xpm\
\
 xpm/MFile.xpm xpm/MTimestring.xpm xpm/MFit.xpm xpm/MFourier.xpm\
 xpm/MLog.xpm xpm/MHelp.xpm

LATEX_EPS=$(subst xpm,eps,$(subst xpm/,latex/,$(XPM_FILES)))
HTML_EPS=$(subst xpm,eps,$(subst xpm/,html/,$(XPM_FILES)))
HTML_GIF=$(subst xpm,gif,$(subst xpm/,html/,$(XPM_FILES) $(XPM_ADDITIONAL_FILES)))
WINHELP_BMP=$(subst xpm,bmp,$(subst xpm/,winhelp/,$(XPM_FILES)))
RTF_BMP=$(subst xpm,bmp,$(subst xpm/,rtf/,$(XPM_FILES)))

all:: doc

clean:: clean_images clean_data

clean_images:: clean_images_latex clean_images_html clean_images_xlp clean_images_winhelp clean_images_rtf

clean_data:: clean_latex clean_html clean_xlp clean_winhelp clean_rtf

doc:: doc_dvi doc_ps doc_html doc_xlp doc_winhelp doc_rtf

##############################################################################

clean_latex: 
	@cd latex;rm -f *.toc *.dvi *.aux *.log *.ps *.idx

clean_images_latex: 
	@rm -f $(LATEX_EPS)

doc_ps:: latex/$(TEX_FILE).ps

doc_dvi:: latex/$(TEX_FILE).dvi

latex/$(TEX_FILE).ps: latex/$(TEX_FILE).dvi
	@(cd latex;dvips $(TEX_FILE).dvi -o $(TEX_FILE).ps;)

latex/$(TEX_FILE).dvi: src/$(TEX_FILE).tex $(TEX_ADDITIONAL) $(LATEX_EPS)
	@ echo "creating latex..."
	@(export TEXINPUTS=".:../src:../sty:/usr/lib/texmf/texmf/tex//";\
	cd latex; \
	latex ../$<; \
	latex ../$<; \
	latex ../$<; \
	)

##############################################################################

clean_html: 
	@cd html;rm -f *.html *.con *.ref
clean_images_html: 
	@rm -f $(HTML_EPS) $(HTML_GIF)

doc_html:: html/$(TEX_FILE)_contents.html

html/$(TEX_FILE)_contents.html: src/$(TEX_FILE).tex $(TEX_ADDITIONAL) $(HTML_EPS) $(HTML_GIF)
	@ echo "creating html..."
	@ rm -f html/$(TEX_FILE).html
	@ cd html; $(TEX2RTF) ../src/$(TEX_FILE).tex $(TEX_FILE) -twice -html -path ../src -path ../sty
	@ ln -s $(TEX_FILE)_contents.html html/$(TEX_FILE).html

##############################################################################

clean_xlp: 
	@cd xlp;rm -f *.xlp *.ref *.con
clean_images_xlp: 
	@

doc_xlp:: xlp/$(TEX_FILE).xlp

xlp/$(TEX_FILE).xlp: src/$(TEX_FILE).tex $(TEX_ADDITIONAL)
	@ echo "creating xlp..."
	@ cd xlp; $(TEX2RTF) ../src/$(TEX_FILE).tex $(TEX_FILE).xlp -twice -xlp -path ../src -path ../sty

##############################################################################

clean_rtf: 
	@cd rtf;rm -f *.rtf *.con *.ref
clean_images_rtf: 
	@rm -f $(RTF_BMP)

doc_rtf:: rtf/$(TEX_FILE).rtf

rtf/$(TEX_FILE).rtf: src/$(TEX_FILE).tex $(TEX_ADDITIONAL) $(RTF_BMP)
	@ echo "creating rtf..."
	@ cd rtf; $(TEX2RTF) ../src/$(TEX_FILE).tex $(TEX_FILE).rtf -twice -rtf -path ../src -path ../sty

##############################################################################

clean_winhelp: 
	@cd winhelp;\
	rm -f *.[Hh][Ll][Pp] \
	      *.[Gg][Ii][Dd] \
	      *.[Ll][Oo][Gg]\
	      *.[Ff][Tt][Ss]\
	      *.hpj \
	      *.cnt *.con *.ref *.rtf
clean_images_winhelp: 
	@rm -f $(WINHELP_BMP)

doc_winhelp:: winhelp/$(TEX_FILE).rtf

winhelp/$(TEX_FILE).rtf: src/$(TEX_FILE).tex $(TEX_ADDITIONAL) $(WINHELP_BMP)
	@ echo "creating winhelp..."
	@ cd winhelp; $(TEX2RTF) ../src/$(TEX_FILE).tex $(TEX_FILE).rtf -twice -winhelp -path ../src -path ../sty
	@ cp src/period98.hpj winhelp

##############################################################################

package: Period98doc-src.tgz

Period98doc-src.tgz::
	@tar cf -  Makefile\
	           html/.empty \
	           latex/.empty \
	           rtf/.empty \
	           src \
	           sty \
	           xlp/.empty \
	           xpm \
	 | gzip -c > Period98doc-src.tgz

##############################################################################

.SUFFIXES:

latex/%.eps : xpm/%.xpm
	@echo create $@ from $<
	@xpmtoppm $< | (ppmtogif 2>/dev/null) | giftopnm | (pnmtops -rle -center -noturn -scale 0.5 - 2>/dev/null) > $@
html/%.eps : xpm/%.xpm
	@echo create $@ from $<
	@xpmtoppm $< | (ppmtogif 2>/dev/null) | giftopnm | (pnmtops -rle -center -noturn -scale 0.5 - 2>/dev/null) > $@
html/%.gif : xpm/%.xpm
	@echo create $@ from $<
	@xpmtoppm $< | (ppmtogif 2>/dev/null) > $@
winhelp/%.bmp : xpm/%.xpm
	@echo create $@ from $<
	@xpmtoppm $< | (ppmtobmp -windows - 2>/dev/null) > $@
rtf/%.bmp : xpm/%.xpm
	@echo create $@ from $<
	@xpmtoppm $< | (ppmtobmp -windows - 2>/dev/null) > $@
