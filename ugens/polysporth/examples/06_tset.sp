_in 1 zeros

1234 0 _in tset

# set dmetro to the trigger callback, set the clock to zero
( 0.5 dmetro 0 1 _in _out "06_tset.scm" ps )

0 _out tget
