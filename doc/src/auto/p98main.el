(TeX-add-style-hook "p98main"
 (function
  (lambda ()
    (LaTeX-add-labels
     "HELP_COPYRIGHT"
     "HELP_INTRODUCTION"
     "HELP_HOW_TO_USE"
     "HELP_DETAIL")
    (TeX-run-style-hooks
     "copyright"
     "intro"
     "install"
     "using"
     "d_basic"
     "d_timestr"
     "d_period"
     "d_fourier"
     "d_log"))))

