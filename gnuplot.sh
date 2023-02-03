#!/bin/bash

filter="$1"
file="$2"

case $filter in 
	t1)
		echo -n "Computing t1 graph..."
		gnuplot <<- EOF
		    	reset
		    	set terminal png
		    	set output 't1_graph.png'
		    	set title "Temperature per stations"
		   		set xlabel "Stations"
		    	set ylabel "Temperature"
		    	set datafile separator ","
		    	set xrange [-1:63]                           
		    	set yrange [-40:70]   
		    	set xtics font ",6" 
		    	set colorbox vertical origin screen 0.9, 0.2 size screen 0.05, 0.6 front  noinvert bdefault
		    	plot "$file" using 1:8:6:7:xtic(2) with errorbars pt 7 ps 2 
		EOF
		echo "DONE"
	;;
	t2)
        	echo -n "Computing t2 graph..."
		gnuplot <<- EOF
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
		    set yrange [-40:70]                                        
		    set timefmt "%Y-%m-%dT%H:%M:%S"                        
		    set format x "%Y/%m/%d"                                    
		    plot "$file"  every 10 using 3:8 lt 5 lc 0 pt 27 w lp
		EOF
		echo "DONE"
	;;
	t3)
		echo -n "Computing t3 graph..."
		gnuplot <<- EOF
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
			set yrange [-40:70]                                                                
			set timefmt "%Y-%m-%dT%H:%M:%S"                                                    
			set format x "%Y-%m-%d"
			set palette rgb 33,13,10                                             
			plot "$file" every 100 u 3:5:4 w l palette
		EOF
		echo "DONE"
	;;
	p1)
		echo -n "Computing t1 graph..."
		gnuplot <<- EOF
		    	reset
		    	set terminal png
		    	set output 'p1_graph.png'
		    	set title "Pressure per stations"
		   		set xlabel "Stations"
		    	set ylabel "Temperature"
		    	set datafile separator ","
		    	set xrange [-1:63]                           
		    	set yrange [90000:110000]    
		    	set xtics font ",6" 
		    	set colorbox vertical origin screen 0.9, 0.2 size screen 0.05, 0.6 front  noinvert bdefault
		    	plot "$file" using 1:8:6:7:xtic(2) with errorbars pt 7 ps 2 
		EOF
		echo "DONE"
	;;
	p2)
        	echo -n "Computing t2 graph..."
		gnuplot <<- EOF
		    reset
		    set terminal png
		    set output 'p2_graph.png'
		    set title "Averrage Pressure per dates"
		    set xlabel "Dates"
		    set ylabel "Temperature"
		    set datafile separator ","
		    set xdata time      
		    set format x "%d/%H"                                  
		    set timefmt "%Y-%m-%dT%H:%M:%S"
		    set xrange ["2010-01-01T01:00:00+01:00":"2022-09-30T20:00:00+02:00"]    
		    set yrange [90000:110000]                                        
		    set timefmt "%Y-%m-%dT%H:%M:%S"                        
		    set format x "%Y/%m/%d"                                    
		    plot "$file"  every 10 using 3:8 lt 5 lc 0 pt 27 w lp
		EOF
		echo "DONE"
	;;
	p3)
		echo -n "Computing t3 graph..."
		gnuplot <<- EOF
			reset
		    set terminal png
		    set output 'p3_graph.png'
			set datafile separator ","                                                        
			set title "TPressure per dates"                                                  
			set xlabel "Dates"                                                                
			set ylabel "Temperature"                                                          
			set xdata time                                                                    
			set timefmt "%Y-%m-%dT%H:%M:%S"                                                    
			set xrange ["2010-01-01T01:00:00+01:00":"2022-09-30T20:00:00+02:00"]              
			set yrange [90000:110000]                                                                
			set timefmt "%Y-%m-%dT%H:%M:%S"                                                    
			set format x "%Y-%m-%d"
			set palette rgb 33,13,10                                             
			plot "$file" every 100 u 3:5:4 w l palette
		EOF
		echo "DONE"
	;;
	-w)
		echo -n "Computing w graph..."
		gnuplot <<- EOF
		    reset
		    set terminal png
		    set output 'w_graph.png'
		    set title "Wind direction"
		    set xlabel "Longitude"
		    set ylabel "Latitude"
		    set size ratio 1
		    set xrange [-100:200]                           
		    set yrange [-100:200]
		    set datafile sep ','
		    unset key
		    set palette rgb 33,13,10
		    plot "$file" using 11:10:(column(8)*10):(column(9)*10):(sqrt(column(8)**2 + column(9)**2)) with vectors palette
		EOF
        	echo "Done"
        ;;
        h) 
        	echo -n "Computing h graph..."      
		gnuplot <<- EOF
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
		    splot "$file" using 9:10:6 with line
		EOF
        	echo "Done"
        ;;
        m)
		echo -n "Computing m graph..."
		gnuplot <<- EOF
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
		    splot "$file" using 9:10:6 with line 
		EOF
		echo "Done"
	;;
esac



