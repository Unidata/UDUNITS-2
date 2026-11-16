---
title: XML Units Database Reference
sidebar: sidebar
toc: true
permalink: xml-units-database-reference.html
---

This page describes the definitions included in the standard UDUNITS-2 units database..

The reference is based on the UDUNITS-2 **2.2.28** database files. It contains **277 unit definitions** and **20 prefixes**. The database is assembled by [`udunits2.xml`](https://github.com/Unidata/UDUNITS-2/blob/main/lib/udunits2.xml), which imports the prefixes, base, derived, accepted, and common definition files.

This page is a reference to the contents of the standard database.

## Prefixes

| Names | Symbols | Value |
| --- | --- | --- |
| `yotta` | `Y` | `1e24` |
| `zetta` | `Z` | `1e21` |
| `exa` | `E` | `1e18` |
| `peta` | `P` | `1e15` |
| `tera` | `T` | `1e12` |
| `giga` | `G` | `1e9` |
| `mega` | `M` | `1e6` |
| `kilo` | `k` | `1e3` |
| `hecto` | `h` | `100` |
| `deka` | `da` | `10` |
| `deci` | `d` | `.1` |
| `centi` | `c` | `.01` |
| `milli` | `m` | `1e-3` |
| `micro` | `µ`, `μ`, `u` | `1e-6` |
| `nano` | `n` | `1e-9` |
| `pico` | `p` | `1e-12` |
| `femto` | `f` | `1e-15` |
| `atto` | `a` | `1e-18` |
| `zepto` | `z` | `1e-21` |
| `yocto` | `y` | `1e-24` |

## SI Base Units

| Names | Symbols | Definition | Description |
| --- | --- | --- | --- |
| `meter`, `metre` | `m` | — | The meter is the length of the path travelled by light in vacuum during a time interval of 1/299 792 458 of a second. |
| `kilogram` | `kg` | — | The kilogram is the unit of mass; it is equal to the mass of the international prototype of the kilogram. |
| `second` | `s` | — | The second is the duration of 9 192 631 770 periods of the radiation corresponding to the transition between the two hyperfine levels of the ground state of the cesium 133 atom. |
| `ampere` | `A` | — | The ampere is that constant current which, if maintained in two straight parallel conductors of infinite length, of negligible circular cross-section, and placed 1 meter apart in vacuum, would produce between these conductors a force equal to 2e-7 newton per meter of length. |
| `kelvin` | `K` | — | The kelvin, unit of thermodynamic temperature, is the fraction 1/273.16 of the thermodynamic temperature of the triple point of water. |
| `mole` | `mol` | — | The mole is the amount of substance of a system which contains as many elementary entities as there are atoms in 0.012 kilogram of carbon 12. When the mole is used, the elementary entities must be specified and may be atoms, molecules, ions, electrons, other particles, or specified groups of such particles. |
| `candela` | `cd` | — | The candela is the luminous intensity, in a given direction, of a source that emits monochromatic radiation of frequency 540e12 hertz and that has a radiant intensity in that direction of 1/683 watt per steradian. |

## Derived Units

| Names | Symbols | Definition | Description |
| --- | --- | --- | --- |
| `radian` | `rad` | — | standard unit of angular measure, an angle that creates an arc in a circle equal in length to that circle's radius (roughly 57.3 degrees); as a ratio of two lengths, it has no dimension |
| `steradian` | `sr` | `rad^2` | standard unit of solid angle measure, it is the solid angle which cuts out an area on a sphere that is the square of the sphere's radius; as a ratio of two areas, it has no dimension |
| `hertz` | `Hz` | `1/s` | unit of frequency meaning one cycle per second |
| `gram` | `g` | `1e-3 kg` | unit of mass equal to one thousandth of a kilogram |
| `newton` | `N` | `m.kg/s^2` | unit of force; the amount needed to accelerate 1 kilogram of mass at the rate of 1 metre per second squared |
| `pascal` | `Pa` | `N/m^2` | unit of pressure (force per unit area) equal to one newton per square meter |
| `joule` | `J` | `N.m` | standard unit of work or energy, equal to the work done by a force of one newton acting along a distance of one meter |
| `watt` | `W` | `J/s` | standard unit of power, equivalent to one joule per second, and equal to one ampere flowing across a potential difference of one volt |
| `coulomb` | `C` | `s.A` | standard unit of electric charge, the quantity of electricity transported in one second by a current of one ampere |
| `volt` | `V` | `W/A` | standard unit of electric potential difference (and electromotive force); the difference of potential between two points of a conductor carrying a constant current of one ampere and dissipating one watt of power |
| `farad` | `F` | `C/V` | standard unit of capacitance; the amount which, when a capacitor is charged to a potential difference of one volt, carries a charge of one coulomb |
| `ohm` | `Ω`, `Ω` | `V/A` | standard unit of electrical resistance; the resistance between two points of a conductor when a constant potential difference of 1.0 volt, applied to these points, produces in the conductor a current of 1.0 ampere |
| `siemens` | `S` | `A/V` | unit of electric conductance; the reciprocal of one ohm |
| `weber` | `Wb` | `V.s` | unit of magnetic flux (product of the average magnetic field times the perpendicular area that it penetrates), expressed as volt-seconds |
| `tesla` | `T` | `Wb/m^2` | unit of flux density, equal to one weber per square meter |
| `henry` | `H` | `Wb/A` | unit of inductance; where a circuit's current changes at a constant rate of 1 ampere per second, results in a generation of 1 V of potential difference |
| `degree_Celsius` | `°C` | `K @ 273.15` | unit (and scale) of temperature, with same magnitude as the kelvin and a zero-point offset of 273.15 |
| `lumen` | `lm` | `cd.sr` | unit of luminous flux, a measure of the total "amount" of visible light emitted by a source; one candela-steradian |
| `lux` | `lx` | `lm/m^2` | unit of illuminance and luminous emittance, measuring luminous flux per unit area, and used as a measure of intensity of light; equal to one lumen per square meter |
| `katal` | `kat` | `mol/s` | unit of catalytic activity (property of a catalyst, such as an enzyme); expresses the ability to break 1 mole of bonds per second under specified conditions |
| `becquerel` | `Bq` | `1/s` | unit of radioactivity; the activity of a quantity of material in which one nucleus decays per second (hence, equivalent to one unit per second) |
| `gray` | `Gy` | `J/kg` | unit of ionizing radiation, a measure of the absorbed dose of radiation; absorption of one joule of radiation energy by one kilogram of matter |
| `sievert` | `Sv` | `J/kg` | unit of ionizing radiation dose, measuring the health effect of low levels of ionizing radiation on the human body |

## Accepted Units

| Names | Symbols | Definition | Description |
| --- | --- | --- | --- |
| `minute` | `min` | `60 s` | period of time equal to 60 seconds |
| `hour` | `h`, `hr` | `60 min` | period of time equal to 60 minutes |
| `day` | `d` | `24 h` | period of time equal to 24 hours |
| `pi` | `π` | `3.141592653589793238462643383279` | mathematical constant equal to the ratio of a circle's circumference to its diameter |
| `arc_degree`, `angular_degree`, `degree`, `arcdeg` | `°` | `(pi/180) rad` | measurement of a plane angle representing 1/360 of a full rotation |
| `arc_minute`, `angular_minute`, `arcminute`, `arcmin` | `'`, `′` | `°/60` | measurement of a plane angle equal to 1/60 arc degree |
| `arc_second`, `angular_second`, `arcsecond`, `arcsec` | `"`, `″` | `'/60` | measurement of a plane angle equal to 1/60 arc minute |
| `liter`, `litre` | `L`, `l` | `dm^3` | unit of capacity equal to 1000 cubic centimeters |
| `metric_ton`, `tonne` | `t` | `1000 kg` | unit of weight equal to 1000 kilograms |
| `electronvolt`, `electron_volt` | `eV` | `1.60217733e-19 J` | unit of energy equal to the work accelerating an electron through a potential difference of one volt |
| `unified_atomic_mass_unit`, `atomic_mass_unit`, `atomicmassunit`, `amu` | `u` | `1.6605402e-27 kg` | standard unit for indicating mass on an atomic or molecular scale; is approximately the mass of one nucleon (either a single proton or neutron), and equivalent to 1 g/mol |
| `astronomical_unit` | `au` | `1.49597870700e11 m` | Exact definition according to 2012 resolution by the International Astronomical Union (IAU). Ostensibly equal to the mean distance from the center of the earth to the center of the sun. |
| `astronomical_unit_BIPM_2006` | `ua` | `1.495979e11 m` | unit of measurement equal to 149.6 million kilometers, the mean distance from the center of the earth to the center of the sun according to the International Bureau of Weights and Measures (BIPM) in 2006 |
| `nautical_mile` | — | `1852 m` | unit of distance at sea, set at 1852 meters (approximately one minute of arc measured along any meridian) |
| `international_knot`, `knot_international`, `knot` | — | `nautical_mile/hour` | derived unit of speed at sea |
| `angstrom`, `ångström` | `Å`, `Å` | `1e-10 m` | unit of length equal to one hundred-millionth of a centimeter (1 meter/10**10) |
| `are` | `a` | `dam^2` | unit of area equal to 100 square meters |
| `hectare` | — | `100 are` | unit of area equal to 100 are ('hectoare'), or 10000 square meters |
| `barn` | `b` | `100 fm^2` | unit of area, approximately the cross-sectional area of a uranium nucleus (10e-28 square meters) |
| `bar` | — | `1000 hPa` | unit of pressure equal to 100000 Pascals, or about 0.987 atmospheres of pressure |
| `gal` | — | `cm/s^2` | unit of acceleration equal to 1 cm per second squared |
| `curie` | `Ci` | `3.7e10 Bq` | unit of radioactivity corresponding to 3.7 * 10e10 disintegrations per second |
| `roentgen` | `R` | `2.58e-4 C/kg` | unit of ionizing radiation, the amount producing one electrostatic unit of positive or negative ionic charge in one cubic centimeter of air under standard conditions; rarely used |
| `rem` | — | `cSv` | unit of radiation dosage applied to humans; equal to one hundredth of a sievert |

## Common Definitions

| Names | Symbols | Definition | Description |
| --- | --- | --- | --- |
| `sec` | — | `s` | unit of time, synonym for second |
| `amp` | — | `A` | unit of electric current, synonym for ampere |
| `degree_kelvin`, `degree_K`, `degreeK`, `deg_K`, `degK` | `°K` | `K` | unit of temperature, synonym for kelvin |
| `candle` | — | `cd` | unit of luminous intensity, synonym for candela |
| `einstein` | — | `mole` | unit of chemical mass, synonym for mole |
| `baud` | `Bd`, `bps` | `Hz` | unit of frequency, synonym for hertz |
| `celsius`, `degree_C`, `degreeC`, `deg_C`, `degC` | `℃` | `degree_Celsius` | unit of temperature, synonym for 'K @ 273.15' (degree_Celsius) |
| — | `kt`, `kts` | `knot` | unit of speed, synonym for nautical_mile/hour |
| `avogadro_constant` | — | `6.02214179e23/mol` | number of constituent particles (usually atoms or molecules) per mole of a given substance |
| `percent` | `%` | `0.01` | number of parts per hundred |
| — | `ppv` | `1` | parts per volume |
| — | `ppm`, `ppmv` | `1e-6` | parts per million |
| — | `ppb`, `ppbv` | `1e-9` | parts per billion |
| — | `ppt`, `pptv` | `1e-12` | parts per trillion |
| — | `ppq`, `ppqv` | `1e-15` | parts per quadrillion |
| — | `B` | `lg(re 1)` | logarithmic unit used to express dimensionless ratios; a value of 0 B (0 dB) corresponds to a ratio of 1 |
| `grade` | — | `0.9 arc_degree` | 1/100 of a right angle (90 degrees) |
| `circle`, `cycle`, `turn`, `revolution`, `rotation` | — | `2 pi rad` | unit of angle in a plane signifying a full 360-degree circle |
| `degree_north`, `degree_N`, `degreeN`, `degree_east`, `degree_E`, `degreeE`, `degree_true`, `degree_T`, `degreeT` | — | `arc_degree` | unit of angle on a sphere |
| `degree_west`, `degree_W`, `degreeW` | — | `-1 degree_east` | unit of angle on a sphere (units for negative direction) |
| `assay_ton` | — | `2.916667e-2 kg` | reference unit of mass for a body of ore; roughly equal to 29167 milligrams |
| `avoirdupois_ounce` | — | `2.834952e-2 kg` | unit of mass equal to 1/16 avoirdupois pound, commonly used in the United States (16 oz = 1 pound = 7000 grains) |
| `avoirdupois_pound`, `pound` | `lb` | `4.5359237e-1 kg` | unit of mass in avoirdupois system of weights (a system commonly used in United States) |
| `carat` | — | `2e-4 kg` | unit of mass equal to 0.2 gram (defined 1907) |
| `grain` | `gr` | `6.479891e-5 kg` | unit of mass equal to 1/7000 pound |
| `long_hundredweight` | — | `5.080235e1 kg` | unit of mass; a British hundredweight, which is 8 stone * 14 pounds/stone |
| `pennyweight` | — | `1.555174e-3 kg` | unit of mass; based on historical US troy weight system (is 1/20 troy ounce) |
| `short_hundredweight` | — | `4.535924e1 kg` | unit of mass, a US hundredweight, which is 100 pounds |
| `slug` | — | `14.59390 kg` | unit of mass associated with Imperial units; a mass that accelerates by 1 ft/s2 when a force of one pound-force (lbF) is exerted on it |
| `troy_ounce`, `apothecary_ounce` | — | `3.110348e-2 kg` | unit of mass; based on historical US troy weight system (is 1/12 troy pound) |
| `troy_pound`, `apothecary_pound` | — | `3.732417e-1 kg` | unit of mass; based on historical US troy weight system (is 5760 grain) |
| `scruple` | — | `20 grain` | unit of mass in apothecaries' weight system (is 1/3 apdram) |
| `apdram` | — | `60 grain` | unit of mass in apothecaries' weight system (is 1/8 apounce) |
| `dram` | `dr` | `avoirdupois_ounce/16` | unit of mass in the avoirdupois system (the system commonly used in the United States) |
| `apounce` | — | `480 grain` | unit of mass in apothecaries' weight system (is 1/16 appound) |
| `appound` | — | `5760 grain` | unit of mass in apothecaries' weight system (is same as a troy pound) |
| `bag` | — | `94 pound` | unit of mass, for a traditional bag of portland cement |
| `short_ton`, `ton` | — | `2000 pound` | unit of mass based on US weight system |
| `long_ton` | — | `2240 pound` | unit of mass based on British imperial weight system |
| `fermi` | — | `1e-15 m` | unit of length equal to 10e-15 meters, a typical length-scale of nuclear physics |
| `light_year` | — | `9.46073e15 m` | unit of length equal to the distance traversed by light in one mean solar year (365.2422 days), a typical length-scale of astronomy |
| `micron` | — | `1e-6 m` | unit of length, a typical length-scale of technology and science fields |
| `mil` | — | `2.54e-5 m` | unit of length equal to 0.001 inch, a typical length-scale for measuring wire diameters |
| `parsec` | — | `3.085678e16 m` | unit of length corresponding to the distance at which the mean radius of the earth's orbit subtends an angle of one second of arc, a typical length-scale of astronomy |
| `printers_point` | — | `3.514598e-4 m` | unit of length equal to 1/72.27 inch, the original (standardized 1886) unit for measuring font size and other small items on a printed page (see also big_point) |
| `chain` | — | `2.011684e1 m` | unit of length equal to 66 feet (4 poles), or 1/10 furlong, a typical (historical) scale of land surveying |
| `printers_pica`, `pica` | — | `12 printers_point` | unit of length equal to 1/6 inch (12 printers points) |
| `nmile` | — | `nautical_mile` | unit of length in the US Customary System, equal to 6,076 feet; typically used for air and sea navigation |
| `US_survey_foot` | — | `(1200/3937) m` | unit of length used for earlier survey data in some countries, slightly different than the current international foot |
| `US_survey_yard` | — | `3 US_survey_feet` | unit of length used in earlier survey data in some countries, slightly different than the current international yard |
| `US_survey_mile`, `US_statute_mile` | — | `5280 US_survey_feet` | unit of length used for earlier survey data in some countries, slightly slightly different than the current international mile |
| `rod`, `pole`, `perch` | — | `16.5 US_survey_feet` | unit of length equal to one-fourth of a surveyor's chain |
| `furlong` | — | `660 US_survey_feet` | unit of length equal to 1/8 mile or 10 chains |
| `fathom` | — | `6 US_survey_feet` | unit of length equal to 6 feet in the imperial and US customary systems, typically used for measuring depth of water |
| `international_inch`, `inch` | `in` | `2.54 cm` | unit of length equal to 25.4 mm by definition, used in imperial and US customary systems |
| `international_foot`, `foot` | `ft` | `12 international_inches` | unit of length equal to 12 international inches, in the imperial and US customary systems; primarily used in the United States |
| `international_yard`, `yard` | `yd` | `3 international_feet` | unit of length equal to 3 international feet, in the imperial and US customary systems; primarily used in the United States |
| `international_mile`, `mile` | `mi` | `5280 international_feet` | unit of length equal to 5280 feet, equal to 12 international inches, in the imperial and US customary systems; primarily used in the United States and other smaller countries with ties to the US or United Kingdom |
| `big_point` | — | `inch/72` | unit of length equal to 1/72 inch, standardized unit in modern computer-based publishing for measuring font size and other small items on a printed page (contrast to printers_point) |
| `barleycorn` | — | `inch/3` | unit of length based in medieval laws of England and Wales, defining an inch as being 3 barleycorns long (length of a corn of barley); still the basis for current shoe sizes in Great Britain and Ireland |
| `arpentlin` | — | `191.835 foot` | unit of length in French regions; a linear arpent is of length 10 perch (10 rod) |
| `rotation_per_second` | `rps`, `cps` | `rotation/second` | unit of angular velocity |
| — | `rpm` | `rotation/minute` | unit of angular velocity measuring the angular distance covered by a rotating object, divided by the amount of time used to cover that distance; measured perpendicular to the plane of rotation, with direction usually indicated by the right-hand rule |
| `denier` | — | `1.111111e-7 kg/m` | unit of lineic mass density for fibers, equal to the mass in grams per 9000 meters (more common in United States and United Kingdom); a single strand of silk is approximately one denier |
| `tex` | — | `1e-6 kg/m` | unit of lineic mass density for fibers, defined as mass in grams per 1000 meters (more common in Canada and Continental Europe) |
| `perm_0C` | — | `5.72135e-11 kg/(Pa.s.m^2)` | unit of mass per unit time (includes flow) for how fast water vapor flows through substance, or permeance; equals 1 gram of water vapor per hour, per square meter, per millimeter of mercury at 0 degrees C |
| `perm_23C` | — | `5.74525e-11 kg/(Pa.s.m^2)` | unit of mass per unit time for how fast water vapor flows through substance, or permeance; equals 1 gram of water vapor per hour, per square meter, per millimeter of mercury at 23 degrees C |
| `circular_mil` | — | `5.067075e-10 m^2` | unit of area equal to the area of a one-mil diameter circle |
| `darcy` | — | `9.869233e-13 m^2` | unit of area for measuring permeability to fluid, equal to 1 cubic centimeter of fluid with 1 centipoise viscosity in 1 second through a 1-square-centimeter cross section of porous medium 1 centimeter long at 1 atmosphere |
| `acre` | — | `160 rod^2` | unit of area in the US Customary System, used in land and sea floor measurement, equal to 43560 square feet |
| `acre_foot` | — | `1.233489e3 m^3` | unit of volume used to describe large-scale water resources in the United State; equal to the volume of one acre of surface area with one foot of depth depth |
| `board_foot` | — | `2.359737e-3 m^3` | unit of volume equal to the cubic contents of a piece of lumber one foot square and one inch thick, used in measuring logs and lumber in the United States and Canada |
| `bushel` | `bu` | `3.523907e-2 m^3` | unit of volume defined as 2150.42 cubic inches or 4 pecks in the US Customary system (and formerly in England), where it is used as a dry measure |
| `peck` | `pk` | `bushel/4` | unit of volume defined as 537.6 cubic inches in the US Customary system (and formerly in England), where it is used as a dry measure |
| `Canadian_liquid_gallon` | — | `4.546090e-3 m^3` | unit of volume for liquids in the Imperial system |
| `US_dry_gallon` | — | `4.404884e-3 m^3` | unit of volume for dry measure in the US Customary system, defined as 1/2 peck or 1/8 bushel |
| — | `cc` | `cm^3` | unit of volume equal to the volume of a cube 1 centimeter on each side |
| `stere` | — | `1 m^3` | unit of volume equal to a cubic meter, originally defined primarily as a measure for firewood |
| `register_ton` | — | `2.831685 m^3` | unit of volume used for internal capacity of ships, equal to 100 cubic feet |
| `US_dry_quart`, `dry_quart` | — | `US_dry_gallon/4` | unit of volume for dry measure in the US Customary system, equal to 1/32 US bushel |
| `US_dry_pint`, `dry_pint` | — | `US_dry_gallon/8` | unit of volume for dry measure in the US Customary system, equal to 1/2 US dry quart |
| `US_liquid_gallon`, `liquid_gallon`, `gallon` | — | `3.785412e-3 m^3` | unit of volume for liquid measure in the US Customary system, defined as 3.785412 liters |
| `barrel` | `bbl` | `42 US_liquid_gallon` | unit of volume used by US and Canadian petroleum organizations |
| `firkin` | — | `barrel/4` | unit of volume whose exact quantity depends on the type of barrel on which it is defined; in this table it is defined based on the oil barrel used by the petroleum industry |
| `US_liquid_quart`, `liquid_quart`, `quart` | — | `US_liquid_gallon/4` | unit of volume for liquid measure in the US Customary system, equal to 1/4 liquid gallon |
| `US_liquid_pint`, `liquid_pint`, `pint` | `pt` | `US_liquid_gallon/8` | unit of volume for liquid measure in the US Customary system, equal to 1/8 liquid gallon |
| `US_liquid_cup`, `liquid_cup`, `cup` | — | `US_liquid_gallon/16` | unit of volume for liquid measure in the US Customary system, equal to 1/16 liquid gallon |
| `US_liquid_gill`, `liquid_gill`, `gill` | — | `US_liquid_gallon/32` | unit of volume for liquid measure in the US Customary system, equal to 1/32 liquid gallon |
| `US_fluid_ounce`, `US_liquid_ounce`, `fluid_ounce`, `liquid_ounce` | `oz`, `floz` | `US_liquid_gallon/128` | unit of volume for liquid measure in the US Customary system, equal to 1/128 liquid gallon |
| `tablespoon` | `Tbl`, `Tbsp`, `tbsp`, `Tblsp`, `tblsp` | `US_fluid_ounce/2` | unit of volume for liquid measure in the US Customary system, equal to 1/2 liquid ounce |
| `fluid_dram` | `fldr` | `US_fluid_ounce/8` | unit of volume for liquid measure in the apothecary system, equal to 1/8 liquid ounce |
| `teaspoon` | `tsp` | `tablespoon/3` | unit of volume defined as 1/3 tablespoon, the actual volume of which can vary depending on the measurement system (but is based on the US Customary system in this database) |
| `UK_liquid_gallon` | — | `4.546090e-3 m^3` | unit of volume for liquid measure in the Imperial system |
| `UK_liquid_quart` | — | `UK_liquid_gallon/4` | unit of volume for liquid measure in the Imperial system, equal to 1/4 liquid gallon |
| `UK_liquid_pint` | — | `UK_liquid_gallon/8` | unit of volume for liquid measure in the Imperial system, equal to 1/8 liquid gallon |
| `UK_liquid_cup` | — | `UK_liquid_gallon/16` | unit of volume for liquid measure in the Imperial system, equal to 1/16 liquid gallon |
| `UK_liquid_gill` | — | `UK_liquid_gallon/32` | unit of volume for liquid measure in the Imperial system, equal to 1/32 liquid gallon |
| `UK_fluid_ounce`, `UK_liquid_ounce` | — | `UK_liquid_gallon/160` | unit of volume for liquid measure in the Imperial system, equal to 1/160 liquid gallon |
| — | `BZ`, `Bz` | `lg(re (1e-6 m)^3)` | logarithmic unit used to compare the reflectivity (Z) of an unknown remote object, to the reflective return created by a rain with a density of a single drop with a diameter of 1 mm in every 1 m^3 of air; is the base unit for "dBz", which is the typical logarithmic unit for measuring atmospheric radar reflectivity; a value of 0 Bz (0 dBz) corresponds to the return from a single cubic micron; a value of 6 Bz (60 dBz) corresponds to extremely heavy rain. |
| `shake` | — | `1e-8 s` | unit of time approximating the lifetime of an individual neutron, useful for describing very brief durations, e.g., in nuclear physics |
| `sidereal_day` | — | `8.616409e4 s` | unit of time that it takes the earth to complete one revolution with respect to a star, roughly 23 hours, 56 minutes, 4 seconds |
| `sidereal_hour` | — | `3.590170e3 s` | unit of time equal to 1/24 sidereal day |
| `sidereal_minute` | — | `5.983617e1 s` | unit of time equal to 1/60 sidereal hour |
| `sidereal_second` | — | `0.9972696 s` | unit of time equal to 1/60 sidereal minute |
| `sidereal_year` | — | `3.155815e7 s` | unit of time for the earth to make one complete revolution around the sun, relative to the fixed stars |
| `tropical_year`, `year` | `yr` | `3.15569259747e7 s` | unit of time; Interval between 2 successive passages of sun through vernal equinox (365.242198781 days). See <http://www.ast.cam.ac.uk/pubinfo/leaflets/>, <http://aa.usno.navy.mil/AA/>, and <http://adswww.colorado.edu/adswww/astro_coord.html> |
| `lunar_month` | — | `29.530589 day` | unit of time equal to the average time between successive new or full moons; equal to approximately 29 days, 12 hours, 44 minutes |
| `common_year` | — | `365 day` | unit of time corresponding to a 'normal' calendar year, that is, one without insertion of a leap day |
| `leap_year` | — | `366 day` | unit of time corresponding to a calendar year with a leap day inserted |
| `Julian_year` | — | `365.25 day` | unit of time recognized by the International Astronomical Union for use in astronomy, defined as 365.25 days of 86400 seconds |
| `Gregorian_year` | — | `365.2425 day` | unit of time based on the Gregorian Calendar, the one commonly used today; approximates the tropical year as 365 + 97/400 days |
| `sidereal_month` | — | `27.321661 day` | unit of time based on 1/12 of the sidereal year |
| `tropical_month` | — | `27.321582 day` | unit of time based on 1/12 of the tropical year |
| `fortnight` | — | `14 day` | unit of time commonly defined as 14 days |
| `week` | — | `7 day` | unit of time commonly defined as 7 days |
| `jiffy` | — | `0.01 s` | unit of time used in computer animation as a method of defining playback rate |
| `eon` | — | `1e9 year` | unit of time defined in astronomy as 1 billion years |
| `month` | — | `year/12` | unit of time defined as the average length of time for a calendar month |
| `sverdrup` | — | `1e6 m^3/s` | unit of volume transport, used almost exclusively to measure the volumetric rate of ocean currents |
| `standard_free_fall` | — | `9.806650 m/s^2` | unit of acceleration corresponding to the nominal gravitational acceleration of an object in a vacuum near the surface of earth |
| `gravity` | — | `standard_free_fall` | unit of acceleration synonymous with standard rate of free fall (in earth's gravity) |
| `conventional_water`, `water` | `H2O`, `h2o` | `gravity 1000 kg/m^3` | specifies the acceleration at the earth's surface of a substance with the density of water |
| `water_4C`, `water_39F` | — | `gravity 999.972 kg/m^3` | specifies the acceleration at the earth's surface of a substance with the density of water at 4 degrees C |
| `water_60F` | — | `gravity 999.001 kg/m^3` | specifies the acceleration at the earth's surface of a substance with the density of water at 60 degrees F |
| `mercury_0C`, `mercury_32F`, `conventional_mercury` | `Hg` | `gravity 13595.10 kg/m^3` | specifies the acceleration at the earth's surface of a substance with the density of mercury at 0 degrees C |
| `mercury_60F` | — | `gravity 13556.8 kg/m^3` | specifies the acceleration at the earth's surface of a substance with the density of mercury at 60 degrees F |
| `force` | — | `standard_free_fall` | unit of force equivalent to the force generated by the effect of gravity |
| `dyne` | — | `1e-5 N` | unit of force, equal to the force that produces an acceleration of one centimeter per second per second on a mass of one gram (the standard centimeter-gram-second unit of force) |
| `pond` | — | `9.806650e-3 N` | unit of force, equal to the magnitude of the force exerted by one gram of mass in a 9.80665 m/s2 gravitational field |
| `force_kilogram`, `kilogram_force` | `kgf` | `9.806650 N` | unit of force, equal to the magnitude of the force exerted by one kilogram of mass in a 9.80665 m/s2 gravitational field |
| `force_ounce`, `ounce_force` | `ozf` | `2.780139e-1 N` | unit of force, equal to the magnitude of the force exerted by one ounce of mass in a 9.80665 m/s2 gravitational field |
| `force_pound`, `pound_force` | `lbf` | `4.4482216152605 N` | unit of force, equal to the magnitude of the force exerted by one pound of mass in a 9.80665 m/s2 gravitational field |
| `poundal` | — | `1.382550e-1 N` | unit of force, that which is necessary to accelerate 1 pound-mass to 1 foot per second per second |
| `gram_force`, `force_gram` | `gf` | `gram force` | unit of force, equal to the magnitude of the force exerted by one gram of mass in a 9.80665 m/s2 gravitational field |
| `force_ton`, `ton_force` | — | `2000 force_pound` | unit of force, equal to the magnitude of the force exerted by one ton of mass in a 9.80665 m/s2 gravitational field (specifically a short ton of mass) |
| `kip` | — | `1000 lbf` | unit of force, equal to the magnitude of the force exerted by one thousand pounds of mass in a 9.80665 m/s2 gravitational field |
| `standard_atmosphere`, `atmosphere` | `atm` | `1.01325e5 Pa` | unit of pressure, an international reference pressure intended to represent the mean atmospheric pressure at mean sea level at the latitude of Paris, France |
| `technical_atmosphere` | `at` | `1 kg gravity/cm2` | unit of pressure equal to one kilogram force per square centimeter |
| — | `cm_H2O`, `cmH2O` | `cm H2O` | unit of pressure derived from pressure head calculations using metrology; represents the pressure exerted by a column of water of 1 cm height at 4 degrees C |
| `inch_H2O_39F` | — | `inch water_39F` | unit of pressure representing the pressure exerted by a column of water of 1 inch height at 39 degrees F |
| `inch_H2O_60F` | — | `inch water_60F` | unit of pressure representing the pressure exerted by a column of water of 1 inch height at 60 degrees F |
| `foot_water`, `foot_H2O`, `footH2O` | `ftH2O`, `fth2o` | `foot water` | unit of pressure representing the pressure exerted by a column of water of 1 foot height at 4 degrees C |
| — | `cm_Hg`, `cmHg` | `cm Hg` | unit of pressure representing the pressure exerted by a column of mercury of 1 cm height at 0 degrees C |
| `millimeter_Hg_0C` | — | `mm mercury_0C` | unit of pressure representing the pressure exerted by a column of mercury of 1 mm height at 0 degrees C |
| `inch_Hg_32F` | — | `inch mercury_32F` | unit of pressure representing the pressure exerted by a column of mercury of 1 inch height at 32 degrees F |
| `inch_Hg_60F` | — | `inch mercury_60F` | unit of pressure representing the pressure exerted by a column of mercury of 1 inch height at 60 degrees F |
| `millimeter_Hg`, `torr` | `mm_Hg`, `mm_hg`, `mmHg`, `mmhg` | `mm Hg` | unit of pressure representing the pressure exerted by a column of mercury of 1 mm height at 0 degrees C; approximately (within 0.000015%, generally below measurement error) 1 Torr, which is 1/760 standard atmospheric pressure |
| `inch_Hg` | `in_Hg`, `inHg` | `inch Hg` | unit of pressure representing the pressure exerted by a column of mercury of 1 inch height at 0 degrees C |
| — | `psi` | `1 pound gravity/in^2` | unit of pressure representing the pressure exerted, due to gravity, by a one-pound mass, of area one square inch; commonly referred to as "pounds per square inch" |
| — | `ksi` | `kip/in^2` | unit of pressure representing the pressure exerted, due to gravity, by a 1000-pound mass, of area one square inch |
| `barie`, `barye` | — | `0.1 N/m^2` | unit of pressure equal to one dyne per square centimeter |
| — | `B_SPL` | `lg(re 20e-6 Pa)` | logarithmic unit used to compare the sound pressure of an unknown sound to a standard reference sound pressure; is the base unit for "dB_SPL", a more typical logarithmic unit for measuring sound pressure (the SI unit for sound pressure is pascal); the standard reference sound pressure in air or other gases is 20 microPascal, which is usually considered the threshold of human hearing (at 1 kHz) |
| `poise` | — | `1e-1 Pa.s` | unit of dynamic viscosity, corresponding to 0.1 pascal-second (pascal-second: a fluid placed between two plates, when one plate is pushed sideways with a shear stress of one pascal, moves a distance equal to the thickness of the layer between the plates in one second) |
| `stokes` | `St` | `1e-4 m^2/s` | unit of kinematic viscosity, measuring the ratio of the dynamic viscosity to the density of the fluid; water at 20 degrees C has a kinematic viscosity about 100 stokes, or more cmomonly, 1 cSt |
| `rhe` | — | `10/(Pa.s)` | unit of fluidity (reciprocal of velocity), measured in reciprocal poise |
| `erg` | — | `1e-7 J` | unit of work, equal to the amount of work done by a force of one dyne exerted for a distance of one centimeter (in CGS base units, one gram centimeter-squared per second-squared) |
| `IT_Btu`, `Btu` | — | `1.05505585262e3 J` | unit of energy, equal to the energy needed to cool or heat one pound of water by one degree F; this uses the International Steam Table (IT) calorie , defined by the Fifth International Conference on the properties of Steam (1956) |
| `EC_therm` | — | `1.05506e8 J` | unit of energy legally defined by the Council Directive of 20 December 1979, Council of the European Communities (now the European Union, EU);.roughly equal to 100,000 IT_Btu |
| `thermochemical_calorie` | — | `4.184000 J` | unit of heat energy defined as 4.184 Joules exactly (International Standard ISO 31-4: Quantities and units, Part 4: Heat); approximately the energy needed to increase the temperature of 1 gram of water by 1 C |
| `IT_calorie`, `calorie` | `cal` | `4.1868 J` | unit of heat energy used in thermochemistry, the International Steam Table (IT) calorie defined by the Fifth International Conference on the properties of Steam (1956) |
| `TNT` | — | `4.184 MJ/kg` | unit of energy; approximately the energy released by the detonation of a given amount of mass of TNT |
| `ton_TNT` | — | `4.184e9 J` | unit of energy; approximately the energy released by the detonation of a 1000 kilograms of TNT |
| `US_therm`, `therm` | `thm` | `1.054804e8 J` | unit of energy legally defined in the U.S. Federal Register of July 27, 1968, and the legal unit used by the U.S. natural gas industry |
| `watthour` | — | `watt.hour` | unit of energy equal to the product of the power in watts and the time in hours (if the energy is being transmitted or used at a constant rate (power) over a period of time); one watt is equal to 1 Joule/second |
| — | `bev` | `1e9 eV` | unit of energy corresponding to 1 billion electron volts (eV) |
| `voltampere` | `VA` | `V.A` | unit of electric power equal to the product of one volt and one ampere, equivalent to one watt for direct current systems and a unit of apparent power for alternating current systems |
| `boiler_horsepower` | — | `9.80950e3 W` | unit of power equal to the power required to evaporate 34.5 lb of fresh water at 212 degrees F in one hour; describes a boiler's capacity to deliver steam to a steam engine |
| `shaft_horsepower`, `horsepower` | `hp` | `7.456999e2 W` | unit of power originally corresponding to the estimated typical power of draft horses, calculated as lifting 33000 pounds one foot in one minute (550 foot-pounds/second) |
| `metric_horsepower` | — | `7.35499e2 W` | unit of power corresponding to a calculation of 75 kilogram-meters/second |
| `electric_horsepower` | — | `7.460000e2 W` | unit of power defined in the International System of Units as exactly 746 W; generally used for power used by electrical machines |
| `water_horsepower` | — | `7.46043e2 W` | unit of power used in the U.S. primarily in rating pumps; calculated as pump capacity Q (gallons per minute) times pump pressure ("head") of P (feet of head), divided by 3956 water horsepower; the calculation assumes water density is 8 1/3 pounds per U.S. gallon, which is not exact. |
| `UK_horsepower` | — | `7.4570e2 W` | unit of power originally corresponding to the estimated typical power of draft horses, calculated as lifting 33000 pounds one foot in one minute (550 foot-pounds/second), as calculated/specified in the United Kingdom |
| `refrigeration_ton`, `ton_of_refrigeration` | — | `12000 Btu/hr` | unit of power describing the heat-extraction capacity of cooling equipment; defined as the heat absorbed by melting 1 short ton of pure ice at 0 degrees C in 24 hours |
| — | `BW` | `lg(re 1 W)` | unit of power in decibel scale, referenced to 1 watt; the power in bel-watts is equal to 100 times the base 10 logarithm of the power in watts (making decibel-watt, the more common term, 10 times the base 10 log of the power in watts) |
| — | `Bm` | `lg(re 1 mW)` | unit of power in decibel scale, referenced to 1 milliwatt; the power in bel-milliwatts is equal to 100 times the base 10 logarithm of the power in milliwatts (making decibel-milliwatt, the more common term, 10 times the base 10 log of the power in milliwatts) |
| `clo` | — | `1.55e-1 K.m^2/W` | unit of thermal resistance used in describing the insulating value of clothing; the amount of thermal resistance needed to maintain in comfort a resting subject in a normally ventilated room (air movement 10 cm/sec) at a temperature of 20 degrees C and a humidity less than 50% |
| `abampere` | — | `10 A` | basic unit of electricity in the electromagnetic CGS system of units |
| `gilbert` | — | `7.957747e-1 A` | unit of electricity/magnetism |
| `statampere` | — | `3.335640e-10 A` | unit of electricity/magnetism |
| `biot` | — | `10 A` | basic unit of electricity in the electromagnetic CGS system of units (same as abampere), named after Jean-Baptiste Biot |
| `abfarad` | — | `1e9 F` | unit of electricity/magnetism |
| `abhenry` | — | `1e-9 H` | unit of electricity/magnetism |
| `abmho` | — | `1e9 S` | unit of electricity/magnetism |
| `abohm` | — | `1e-9 ohm` | unit of electricity/magnetism |
| `abvolt` | — | `1e-8 V` | unit of electricity/magnetism |
| — | `e` | `1.602176487e-19 C` | unit of electricity/magnetism |
| `chemical_faraday` | — | `9.64957e4 C` | unit of electricity/magnetism |
| `physical_faraday` | — | `9.65219e4 C` | unit of electricity/magnetism |
| `C12_faraday`, `faraday` | — | `9.648531e4 C` | unit of electricity/magnetism |
| `gamma` | — | `1e-9 T` | unit of electricity/magnetism |
| `gauss` | — | `1e-4 T` | unit of electricity/magnetism |
| `maxwell` | — | `1e-8 Wb` | unit of electricity/magnetism |
| `oersted` | `Oe` | `7.957747e1 A/m` | unit of electricity/magnetism |
| `statcoulomb` | — | `3.335640e-10 C` | unit of electricity/magnetism |
| `statfarad` | — | `1.112650e-12 F` | unit of electricity/magnetism |
| `stathenry` | — | `8.987554e11 H` | unit of electricity/magnetism |
| `statmho` | — | `1.112650e-12 S` | unit of electricity/magnetism |
| `statohm` | — | `8.987554e11 ohm` | unit of electricity/magnetism |
| `statvolt` | — | `2.997925e2 V` | unit of electricity/magnetism |
| `unit_pole` | — | `1.256637e-7 Wb` | unit of electricity/magnetism |
| — | `BV` | `lg(re 1 V)` | unit of electricity/magnetism |
| — | `Bv` | `lg(re 0.775 V)` | unit of electricity/magnetism |
| — | `BµV` | `lg(re 1e-6 V)` | unit of electricity/magnetism |
| `degree_rankine`, `degreeR`, `degree_R`, `degR`, `deg_R` | `°R` | `K/1.8` | unit of thermodynamic temperature |
| `fahrenheit`, `degree_fahrenheit`, `degreeF`, `degree_F`, `degF`, `deg_F` | `°F`, `℉` | `°R @ 459.67` | unit of thermodynamic temperature |
| `footcandle` | — | `1.076391e1 lx` | unit of illumination |
| `footlambert` | — | `3.426259 cd/m^2` | unit of illumination |
| `lambert` | — | `(1e4/pi) cd/m^2` | unit of illumination |
| `stilb` | `sb` | `1e4 cd/m^2` | unit of illumination |
| `phot` | `ph` | `1e4 lm/m^2` | unit of illumination |
| `nit` | `nt` | `1 cd/m^2` | unit of illumination |
| `langley` | — | `4.184000e4 J/m^2` | unit of illumination |
| `blondel`, `apostilb` | — | `cd/(pi m^2)` | unit of illumination |
| `kayser` | — | `100/m` | exact |
| `geopotential`, `dynamic` | `gp` | `gravity` | exact |
| `work_year` | — | `2056 hours` | exact |
| `work_month` | — | `work_year/12` | exact |
| `potential_vorticity_unit` | `PVU` | `1e-6 m^2 s^-1 K kg^-1` | exact |
| `count` | — | `1` | — |
| `bit` | — | `1` | — |
| `octet`, `byte` | — | `8` | — |
| `dobson` | `DU` | `446.2 micromoles/meter^2` | — |
| `molecule`, `molec`, `nucleon`, `nuc` | — | `1/avogadro_constant` | — |

## See Also

- **[User Guide: Units Database](units-database.html)**
- **[Reference: Unit Expression Language Specification](unit-expression-language-specification.html)**
- **[Concepts: Unit Systems](concepts-unit-systems.html)**
