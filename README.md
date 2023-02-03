# --------------- CY-Météo --------------- #

The purpose of this school prject is to treat a very big
meteorological datas file and make some graphs to represent 
theses datas.

# ------ How to use the application ------ #

In order to know how you can use this application you can
type the command ./filter.sh --help
Here two exemples : 

./meteo.sh -p2 -w -A -f data.csv
./meteo.sh -A -f data.csv -p2 --avl -w  

Theses two commands make the same result, a graph with 
pression informations and another graph with wind 
informations in the Antilles

# ------ Differents types of graphs -------- #

Temperatures (-t) / pressures (-p) in mode 1 :
    Error bar chart with the station ID on the x-axis
    and the minimum, maximum and mean on the y-axis.

Temperatures (-t) / pressures (-p) in mode 2 :
    Simple line diagram with the day and time of the 
    measurements on the x-axis and the average of the 
    measurements on the y-axis

Temperatures (-t) / pressures (-p) in mode 3 :
    Multi-line diagram with days on the x-axis and 
    measured values on the y-axis.

Wind (-w) :
    Vector diagram (oriented arrows) with the 
    abscissa corresponding to the longitude (West-East axis)
    and the ordinate corresponding to the latitude 
    (South-North axis).

Altitude (-h) :
    Interpolated and coloured map diagram with the abscissa
    corresponding to longitude (West-East axis) and the ordinate
    corresponding to latitude (South-North axis).

Moisture (-m) :
    Interpolated and coloured map diagram with the abscissa
    corresponding to longitude (West-East axis) and the ordinate
    corresponding to latitude (North-South axis).

# ---------------------------------------- #

