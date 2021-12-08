# TEM Daily
The daily version of The Terrestrial Ecosystem Model (TEM)

The Terrestrial Ecosystem Model (TEM) is a process-based ecosystem model that describes carbon, nitrogen and water dynamics of plants and soils for terrestrial ecosystems of the globe. The TEM uses spatially referenced information on climate, elevation, soils and vegetation as well as soil- and vegetation-specific parameters to make estimates of important carbon, nitrogen and water fluxes and pool sizes of terrestrial ecosystems. The TEM normally operates on a monthly time step and at a 0.5 degrees latitude/longitude spatial resolution, but the model has been applied at finer spatial resolutions (down to 1 hectare).

This Model has to be compiled using a C++ compiler.


# Temhydro Disturb
The Terrestrial Ecosystem Model (TEM)

The Terrestrial Ecosystem Model (TEM) is a process-based ecosystem model that describes carbon, nitrogen and water dynamics of plants and soils for terrestrial ecosystems of the globe. 
The TEM uses spatially referenced information on climate, elevation, soils and vegetation as well as soil- and vegetation-specific parameters to make estimates of important carbon, nitrogen and water fluxes and pool sizes of terrestrial ecosystems. 
The TEM normally operates on a monthly time step and at a 0.5 degrees latitude/longitude spatial resolution, but the model has been applied at finer spatial resolutions (down to 1 hectare).


This Model has to be compiled using a C++ compiler.




# Model framework

![image](https://user-images.githubusercontent.com/47959376/145250181-c099ed9a-2fa7-4d0e-9081-7eb262d83f38.png)


# TEM history review

TEM was originally developed by the Marine Biological Lab. It is a process-based biogeochemical model, which involves the carbon (short-term cycle), nitrogen (more complex in terms of organic – inorganic, open – close) and hydrology cycle. It calculates GPP and NPP at the canopy level by using parameters obtained for photosynthesis and respiration at individual plants. 

The TEM-Hydro version (developed by Dr. Benjamin Felzer) involves leafs, hardwoods, roots, sapwoods, seeds, labile pool, and shuttleworth Wallace water pools in the model. 

In TEM, CTEM is the calibration mode, and the XTEM is the extrapolation mode. 

Potential vegetation in TEM is a cohort-based approach. That means any disturbances in the current date will create cohort for the upcoming year.

The TEM-mls version added multi-layer soil C and N pools, and allow temperature and moisture to vary according to depth in the soil. 

# TEM Input:

Transient datasets (site level or grid level):

1.	Cloud or radiation

2.	Temperature

3.	Precipitation

4.	Ozone (AOT40 – in order to convert to daily, change to CUO index [Pleijel et al. 2004, cumulative stomatal uptake of ozone])

5.	CO2 (a single value for the globe)

6.	Vapor pressure

7.	Vegetation cohorts


Static datasets

1.	Soil texture (sand/silt/clay)

2.	Elevation

3.	Wind speed at surface (to determine aerodynamic)


Parameter files (ecd files)

1.	Soil

2.	Rooting depth

3.	Vegetation

4.	Vegetation mosaic

5.	Leaf

6.	Microbe

7.	Agriculture

8.	Calibrated biome files


# Calibration Procedures

1.	Increase nmax to remove N-limitation and saturate with nitrogen.  Saturation is happening when INGPP=GPP and INNPP=NPP

2.	Adjust cmax to get the target NPPsat value

3.	Adjust nmax to get the target NPP value

4.	Adjust kra to get the target GPP value

5.	Recheck that NPPmax is still correct, by increasing nmax to saturate the Nitrogen, and readjust cmax if necessary, and then nmax to the correct value for the target NPP

6.	Adjust tauheartwood to get the target VegC value

7.	Adjust kdc to get the target SoilC value

8.	Adjust mnup to get the target availn value

9.	For reduced-form open N version, need to wait until YRNIN and YRNLOST balance each other out.  Need to adjust nmax, mnup, and nloss factors simultaneously to get correct target values.

 
# Key References:

VERSION 4.1

Tian, H., J.M. Melillo, D.W. Kicklighter, A.D. McGuire and J.
  Helfrich.  1999. The sensitvity of terrestrial carbon storage to
  historical climate variability and atmospheric CO2 in the United
  States.  Tellus 51B: 414-452.

VERSION 4.2

McGuire, A.D., S. Sitch, J.S. Clein, R. Dargaville, G. Esser, J. Foley,
  M. Heimann, F. Joos, J. Kaplan, D.W. Kicklighter, R.A. Meier, J.M.
  Melillo, B. Moore III, I.C. Prentice, N. Ramankutty, T. Reichenau,
  A. Schloss, H. Tian, L.J. Williams and U. Wittenberg (2001) Carbon
  balance of the terrestrial biosphere in the twentieth century:
  analyses of CO2, climate and land use effects with four process-
  based ecosystem models.  Global Biogeochemical Cycles 15: 183-206.

Tian, H., J.M. Melillo, D.W. Kicklighter, S. Pan, J. Liu, A.D. McGuire
  and B. Moore III (2003) Regional carbon dynamics in monsoon Asia
  and its implications for the global carbon cycle. Global and
  Planetary Change 37: 201-217.

VERSION 4.3

Felzer, B., D. Kicklighter, J. Melillo, C. Wang, Q. Zhuang, and
  R. Prinn (2004) Effects of ozone on net primary production and
  carbon sequestration in the conterminous United States using a
  biogeochemistry model. Tellus 56B: 230-248.

Runge - Kutta - Fehlberg (RKF) adaptive integrator:

Cheney, W., and D. Kincaid.  1985.  Numerical mathematics and
  computing.  Second edition.  Brooks/ Col Publishing Co.  Monterey,
  CA.  pp. 325-328.
  
 Version TEM-Hydro
 
1. Felzer et al., 2009. Importance of carbon-nitrogen interactions and ozone on ecosystem hydrology during the 21st century. JGR: Biogeosciences.
2. Felzer et al., 2011. Nitrogen effect on carbon-water coupling in forests, grasslands, and shrublands in the arid western United States. JGR: Biogeosciences.
3. Felzer, 2012. Carbon, nitrogen, and water response to climate and land use changes in Pennsylvania during the 20th and 21st centuries. Ecological modeling. 
4. Jiang et al., 2015. Improved understanding of Climate Change Impact to Pennsylvania Dairy Pasture. Crop Science. 

Version TEM-MLS

1. Yi et al., 2010. A dynamic organic soil biogeochemical model for simulating the effects of wildfire on soil environmental conditions and carbon dynamics of black spruce forests. JGR.

2. Yuan et al., 2012. Assessment of boreal forest historical C dynamics in Yukon River Basin: relative roles of warming and fire regime change. Ecological Applications.

3. Arora and Boer, 2003. A representation of variable root distribution in dynamic vegetation models. Earth Interactions. 

4. Clapp and Hornberger, 1978. Empirical equations for some soil hydraulic properties. Water Resour. Res. 

5. Oleson et al., 2010. Technical description of version 4.0 of the Community Land Model (CLM).

6. Zeng, 2001. Global vegetation root distribution for land modeling. J. Hydrometeorol.




# Contact:
Benjamin Felzer (bsf208@lehigh.edu)
