BEGIN { print "// Fake Grammar to show examples of each type\n// WARNING: this grammar will probably not work with scramble\nsamples: listRules;\n\nr0:";
	}
/BAD_TYPE/ { next; }
{ print "\t{^" $3 ":  ^} " $3 " " $3  " " $3 " {n}";
  if (++count%10==0) printHeader(); }
END {print "\t;\nlistRules:\n\t";
    for(i=0; i<=line; i++) print "\tr" i;
    print "\n\t;\n"}
function printHeader() {  print "\t;\nr" ++line ":\n"}
