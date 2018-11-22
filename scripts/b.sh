#!/bin/bash
cat data.txt |awk -F"," '
NR%3 == 1 {
	for (i = 1; i <= NF; i++) {
	split($i, a, "=");
	if (a[1] == " iodepth") printf "%s\t",a[2];
	}
}
NR%3 == 2 {
        for (i = 1; i <= NF; i++) {
        split($i, a, " ");
	printf "%s\t",a[2];
        }
}
NR%3 == 0 {
        for (i = 1; i <= NF; i++) {
        split($i, a, "=");
        if (a[1] == " bw") printf "%s\t",a[2];
	if (a[1] == " iops") print a[2];
        }
}
'|sort -nk 1 -t'\t'
#awk -F"," '
#NR%3 == 1 {i=NF; split($i, a, "="); if (a[1] == "iodepth") print $i;}
#' data.txt
#NF == 0 {next}    # skip blank line
#NF == 1 {printf "%s ", $1}    # for id line
# for data line
#{
#    for (i = 1; i <= NF; i++) {
#        split($i, a, "=");
#        if (a[1] == "d") print $i;
#    }
#}
