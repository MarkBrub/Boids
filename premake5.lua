-- premake5.lua
workspace "Boids"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "Boids"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
include "Walnut/WalnutExternal.lua"

include "Boids"