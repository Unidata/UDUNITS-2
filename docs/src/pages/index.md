---
title: Overview of UDUNITS-2
sidebar: sidebar
permalink: index.html
breadcrumbs: false
---

Welcome to the user documentation for **[UDUNITS-2](https://github.com/Unidata/UDUNITS-2)**, a C library and command-line tool for parsing and converting physical units.

## What is UDUNITS-2?

The UDUNITS-2 package is the core software engine required for processing dimensional metadata within NetCDF files adhering to the Climate and Forecast (CF) Metadata Conventions.
Its primary architectural role is to parse, validate, and convert the character strings provided in the `units` attribute, thereby ensuring that datasets are "self-describing."

## Core Capabilities

UDUNITS-2 provides:

- Unit parsing
- Unit conversion
- Dimensional analysis
- Time reference handling (e.g., “seconds since 1970-01-01”)
- Extensible XML-based unit database

It is widely used in scientific computing, meteorology, climate research, and
netCDF-based workflows.

## Integration with CF Conventions

As a generalization and extension of the **[COARDS conventions](https://ferret.pmel.noaa.gov/Ferret/documentation/coards-netcdf-conventions)**, the **[CF standard](https://cfconventions.org/)** requires that every variable representing a dimensional quantity (with specific exceptions for boundary and climatology variables) possesses a `units` attribute recognizable by the UDUNITS-2 package.
This integration is critical for:

1. **Machine Parseability**: Facilitating applications with automated
   extraction, regridding, and display capabilities by allowing machines to
   parse metadata and associate data values with specific spatiotemporal
   locations.

2. **Data Portability**: Enabling the sharing of files across disparate
   computer platforms by utilizing a standardized, grammar-based syntax for
   physical quantities.

3. **Spatiotemporal Mapping**: Mapping coordinate data to earth-based
   locations (latitude, longitude, vertical) and time.

## UDUNITS-1

Development on UDUNITS-1 stopped in 2014, with the first release of UDUNITS-2,
which introduced a new API, Unicode support, logarithmic units, XML-based unit
definitions, and improved conversion handling.

Backward compatibility with UDUNITS-1 is provided, as UDUNITS-2 includes a
Version 1 C API wrapper built on top of the Version 2 library for ease in
migration.

Most unit specifications from UDUNITS-1 still work in UDUNITS-2 (with the
notable change that `g` now means gram instead of standard gravity).

In practice, UDUNITS-1 is considered legacy software, and users are
expected to use UDUNITS-2 for current systems.
