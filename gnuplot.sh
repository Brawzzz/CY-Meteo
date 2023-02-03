#!/bin/bash

filter=""
while [[ "$1" != "" ]] ; do
    filter=$1
    shift
done

case $filter in
    # Filters options
    -t1)
        echo -n "Computing t1 graph..."
gnuplot << EOF
    reset
    set terminal png
    set output 't1_graph.png'
    set title "Temperature per stations"
    set xlabel "Stations"
    set ylabel "Temperature"
    set datafile separator ","
    set xrange [-1:63]                           
    set yrange [-40:80]   
    set xtics font ",6" 
    set colorbox vertical origin screen 0.9, 0.2 size screen 0.05, 0.6 front  noinvert bdefault
    plot "result1.csv" using 1:7:5:6:xtic(2) with errorbars pt 7 ps 2  //// changer le nom du fichier
EOF
        echo Done
    ;;
    -p1)
        echo -n "Computing p1 graph..."
gnuplot << EOF
    reset
    set terminal png
    set output 'p1_graph.png'
    set title "Pressure per stations"
    set xlabel "Stations"
    set ylabel "Pressure"
    set datafile separator ","
    set xrange [-1:63]                            
    set yrange [-40:80]  ///// changer pour les valeur de pressions 
    set xtics font ",6" 
    set colorbox vertical origin screen 0.9, 0.2 size screen 0.05, 0.6 front  noinvert bdefault
    plot "result1.csv" using 1:7:5:6:xtic(2) with errorbars pt 7 ps 2  //// changer le nom du fichier
EOF
        echo Done
    ;;
    -t2)
        echo -n "Computing t2 graph..."
gnuplot << EOF
    reset
    set terminal png
    set output 't2_graph.png'
    set title "Averrage Temperature per dates"
    set xlabel "Dates"
    set ylabel "Temperature"
    set datafile separator ","
    set xdata time      
    set format x "%d/%H"                                  
    set timefmt "%Y-%m-%dT%H:%M:%S"
    set xrange ["2010-01-01T01:00:00+01:00":"2022-09-30T20:00:00+02:00"]                                        
    set timefmt "%Y-%m-%dT%H:%M:%S"                        
    set format x "%Y/%m/%d"                                    
    plot "result1.csv" every 10 using 3:7 lt 5 lc 0 pt 27 w lp //// changer le nom du fichier
EOF
        echo Done
    ;;
    -p2)
        echo -n "Computing p2 graph..."
gnuplot << EOF
    reset
    set terminal png
    set output 'p2_graph.png'
    set title "Mean Temperature per dates"
    set xlabel "Dates"
    set ylabel "Temperature"
    set datafile separator ","
    set xdata time      
    set format x "%d/%H"                                  
    set timefmt "%Y-%m-%dT%H:%M:%S"
    set xrange ["2010-01-01T01:00:00+01:00":"2022-09-30T20:00:00+02:00"]                                        
    set timefmt "%Y-%m-%dT%H:%M:%S"                        
    set format x "%Y/%m/%d"                                    
    plot "t2_avl.csv" every 10 using 3:7 lt 5 lc 0 pt 27 w lp  //// changer le nom du fichier
EOF
        echo Done
    ;;
    -t3)
        echo -n "Computing t3 graph..."
gnuplot << EOF
    reset
    set terminal png
    set output 't3_graph.png'
    set datafile separator ","
    set title "Temperature per dates"
    set xlabel "Dates"
    set ylabel "Temperature"
    set xdata time                                        
    set timefmt "%Y-%m-%dT%H:%M:%S"
    set xrange ["2010-01-01T01:00:00+01:00":"2022-09-30T20:00:00+02:00"]       
    set timefmt "%Y-%m-%dT%H:%M:%S"                                                    
    set format x "%Y-%m-%dT%H:%M:%S"                                    
    plot for [b=0:STATS_blocks-1] "test.csv" u 3:4 w lp ps 1 //// changer le nom du fichier
EOF
        echo Done
    ;;
    -p3)
        echo -n "Computing p3 graph..."
gnuplot << EOF
    reset
    set terminal png
    set output 'p3_graph.png'
    set datafile separator ","
    set title "Pressure per dates"
    set xlabel "Dates"
    set ylabel "Pressure"
    set xdata time                                        
    set timefmt "%Y-%m-%dT%H:%M:%S"
    set xrange ["2010-01-01T01:00:00+01:00":"2022-09-30T20:00:00+02:00"]       
    set timefmt "%Y-%m-%dT%H:%M:%S"                                                    
    set format x "%Y-%m-%dT%H:%M:%S"                                    
    plot for [b=0:STATS_blocks-1] "test.csv" u 3:4 w lp ps 1 //// changer le nom du fichier
EOF 
        echo Done
    ;;
    -w)
        echo -n "Computing w graph..."
gnuplot << EOF
    reset
    set terminal png
    set output 'w_graph.png'
    set title "Wind direction"
    set xlabel "Longitude"
    set ylabel "Latitude"
    set size ratio 1
    set xrange [-120:120]                           
    set yrange [-100:220]
    set datafile sep ','
    unset key
    set palette rgb 33,13,10
    plot 'w_avl.csv' using 9:10:($7*10):($8*10):(sqrt($7**2 + $8**2)) with vectors palette   //// changer le nom du fichier
EOF
        echo Done
    ;;
    -h) 
        echo -n "Computing h graph..."
gnuplot << EOF
    reset 
    set terminal png
    set output 'h_graph.png'
    set contour base
    set pm3d
    unset surface
    set dgrid 60,60
    set hidden3d   
    set datafile separator ","
    set view map     
    splot "h_avl.csv" using 9:10:6 with line  //// changer le nom du fichier
    # titre / recadrer 
EOF       
        echo Done
    ;;
    -m)
        echo -n "Computing m graph..."
gnuplot << EOF
    reset 
    set terminal png
    set output 'm_graph.png'
    set contour base
    set pm3d
    unset surface
    set dgrid 60,60
    set hidden3d   
    set datafile separator ","
    set view map     
    splot "m_avl.csv" using 9:10:6 with line  //// changer le nom du fichier
EOF
        echo Done
    ;;
esac

# gnuplot << EOF

# set terminal png
# set output 'w_graph.png'
# set datafile sep ','
# plot 'w_avl.csv' using 3:4:5:6 with vectors

# EOF



# # T1 GRAPH
# reset
# set title "Temperature per stations"
# set xlabel "Stations"
# set ylabel "Temperature"
# set datafile separator ","
# set xrange [-1:63]                           
# set yrange [-40:80]   
# set xtics font ",6" 
# set colorbox vertical origin screen 0.9, 0.2 size screen 0.05, 0.6 front  noinvert bdefault
# plot "result1.csv" using 1:7:5:6:xtic(2) with errorbars pt 7 ps 2

# #T2 GRAPH
# reset
# set title "Mean Temperature per dates"
# set xlabel "Dates"
# set ylabel "Temperature"
# set datafile separator ","
# set xdata time      
# set format x "%d/%H"                                  
# set timefmt "%Y-%m-%dT%H:%M:%S"
# set xrange ["2010-01-01T01:00:00+01:00":"2022-09-30T20:00:00+02:00"]                                        
# set timefmt "%Y-%m-%dT%H:%M:%S"                        
# set format x "%Y/%m/%d"                                    
# plot "t2èavl.csv" using 3:8 lt 5 lc 0 pt 27 w lp

# #T3 GRAPH
# reset
# set datafile separator ","
# set title "Temperature per dates"
# set xlabel "Dates"
# set ylabel "Temperature"
# set xdata time                                        
# set timefmt "%Y-%m-%dT%H:%M:%S"
# set xrange ["2010-01-01T01:00:00+01:00":"2022-09-30T20:00:00+02:00"] 
# set yrange [-40:70]      
# set timefmt "%Y-%m-%dT%H:%M:%S"                                                    
# set format x "%Y-%m-%dT%H:%M:%S"
# set palette rgb 33,13,10                                    
# plot for [b=0:STATS_blocks-1] "t3_avl.csv" u 3:4:1 w lp ps 1 palette

# #H GRAPH
# reset 
# set contour base
# set pm3d
# unset surface
# set dgrid 60,60
# set hidden3d   
# set datafile separator ","
# set view map     
# splot "h_avl.csv" using 9:10:6 with line
# # #titre / recadrer 

# #M GRAPH
# reset 
# set contour base
# set pm3d
# unset surface
# set dgrid 60,60
# set hidden3d   
# set datafile separator ","
# set view map     
# splot "m_avl.csv" using 9:10:6 with line

# #W GRAPH

# # set terminal png
# # set output 'w_graph.png'
# reset
# set title "Wind direction"
# set xlabel "Longitude"
# set ylabel "Latitude"
# set datafile sep ','
# plot 'w_avl.csv' using 9:10:7:8 with vectors


# reset
# set title "Wind direction"
# set xlabel "Longitude"
# set ylabel "Latitude"
# set size ratio 1
# set xrange [-120:120]                           
# set yrange [-100:220]
# set datafile sep ','
# unset key
# set palette rgb 33,13,10
# plot 'w_avl.csv' using 9:10:($7*10):($8*10):(sqrt($7**2 + $8**2)) with vectors palette 



# reset
# stats "t3.csv"            
# set datafile separator ","                                                         
# set title "Temperature per dates"                                                  
# set xlabel "Dates"                                                                 
# set ylabel "Temperature"                                                           
# set xdata time                                                                     
# set timefmt "%Y-%m-%dT%H:%M:%S"                                                    
# set xrange ["2010-01-01T01:00:00+01:00":"2012-09-30T20:00:00+02:00"]               
# set yrange [-40:70]                                                                
# set timefmt "%Y-%m-%dT%H:%M:%S"                                                    
# set format x "%Y-%m-%d"  
# set palette rgb 33,13,10                                                                                                 
# plot "t3.csv" u 3:5:4 w lp palette



# reset
# set title "Wind direction"
# set xlabel "Longitude"
# set ylabel "Latitude"
# set size ratio 1
# set xrange [-120:120]                           
# set yrange [-100:220]
# set datafile sep ','
# unset key
# set palette rgb 33,13,10
# plot 'w_avl.csv' using 9:10:($7*10):($8*10):(sqrt($7**2 + $8**2)) with vectors palette


# set datafile separator ","                                                         
# set title "Temperature per dates"                                                  
# set xlabel "Dates"                                                                 
# set ylabel "Temperature"                                                           
# set xdata time                                                                     
# set timefmt "%Y-%m-%dT%H:%M:%S"                                                    
# set xrange ["2010-01-01T01:00:00+01:00":"2012-09-30T20:00:00+02:00"]               
# set yrange [-40:70]                                                                
# set timefmt "%Y-%m-%dT%H:%M:%S"                                                    
# set format x "%Y-%m-%d"  
# set palette rgb 33,13,10                                                                                                                                                                                                     
# plot "result1.csv" every 100 u 3:5:4 w l palette









# reset
# set title "Pressure per stations"
# set xlabel "Stations"
# set ylabel "Pressure"
# set datafile separator ","
# set xrange [-1:63]                            
# set yrange [-40:80]
# set xtics font ",6" 
# set colorbox vertical origin screen 0.9, 0.2 size screen 0.05, 0.6 front  noinvert bdefault
# plot "t2éavl.csv" using 3:8:xtic(3) w l