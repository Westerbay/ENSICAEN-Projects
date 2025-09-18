set terminal png
set output "resources/citizens_situation.png"

set title "Citizens situation"
set xlabel "Day"
set ylabel "Number of citizens"

plot "resources/evolution.txt" using 1:2 title "Healthy Citizens" with lines, \
     "resources/evolution.txt" using 1:3 title "Sick Citizens" with lines, \
     "resources/evolution.txt" using 1:4 title "Dead Citizens" with lines, \
     "resources/evolution.txt" using 1:5 title "Burned Bodies" with lines