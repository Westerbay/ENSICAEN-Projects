set terminal png
set output "resources/press_agency.png"

set xlabel "Day"

set multiplot layout 2, 1
set title "People"
plot "resources/evolutionPress.txt" using 1:2 title "Healthy Citizens" with lines, \
     "resources/evolutionPress.txt" using 1:3 title "Sick Citizens" with lines, \
     "resources/evolutionPress.txt" using 1:4 title "Dead Citizens" with lines, \
     "resources/evolutionPress.txt" using 1:5 title "Burned Bodies" with lines
	 

set title "Contamination Rate"
set format y "%g%%" 

stats 'resources/evolutionPress.txt' using 6 name 'A'
stats 'resources/evolutionPress.txt' using 7 name 'B'
stats 'resources/evolutionPress.txt' using 8 name 'C'

set yrange [*:((A_max > B_max ? (A_max > C_max ? A_max : C_max) : (B_max > C_max ? B_max : C_max)) * 1.25)]
plot "resources/evolutionPress.txt" using 1:7 title "Reporter 1 (if > 80%)" with lines, \
     "resources/evolutionPress.txt" using 1:8 title "Reporter 2 (if > 80%)" with lines, \
	 "resources/evolutionPress.txt" using 1:6 title "City" with lines linecolor rgb "red"
	 
unset multiplot