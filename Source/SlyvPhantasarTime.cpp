// License:
// **********************************************
// 
// Units/Source/SlyvPhantasarTime.cpp
// (c) Jeroen Broks, 2024, All Rights Reserved.
// 
// This file contains material that is related
// to a storyline that is which is strictly
// copyrighted to Jeroen Broks.
// 
// This file may only be used in an unmodified
// form with an unmodified version of the
// software this file belongs to.
// 
// You may use this file for your study to see
// how I solved certain things in the creation
// of this project to see if you find valuable
// leads for the creation of your own.
// 
// Mostly this file comes along with a project
// that is for most part released under an
// open source license and that means that if
// you use that code with this file removed
// from it, you can use it under that license.
// Please check out the other files to find out
// which license applies.
// 
// This file comes 'as-is' and in no possible
// way the author can be held responsible of
// any form of damages that may occur due to
// the usage of this file
// 
// 
// **********************************************
// 
// 
// version: 24.12.07
// 
// End License
#include "../Headers/SlyvPhantasarTime.hpp"

namespace Slyvina {
	namespace Units {
		const char DecName[37][100] = {
			// The Decas are named after the Befindo gods (also worshipped by humans)
			/* Deca #1 */"Tarkials",     // Named after Tarkials, god of Thunder
			/* Deca #2 */"Waladra",      // Named after Waladra, goddess of Winter
			/* Deca #3 */"Malabia",      // Goddess of life and light
			/* Deca #4 */"Crono",        // God of time
			/* Deca #5 */"Nikkibasona",  // Goddess who pushies the sould of bad people
			// after they die
			/* Deca #6 */"Dopplosephuh", // God of death
			/* Deca #7 */"Poubelle",     // Goddess of Destruction
			/* Deca #8 */"Lentra",       // Goddess of Spring
			/* Deca #9 */"Yasathar",     // Boss God. Father of all Befindo Gods
			/* Deca #10 */"Cupa",         // Goddess of love
			/* Deca #11 */"Shalaga",      // Goddess who is the judge of your life after death
			/* Deca #12 */"Pyro",         // God of fire
			/* Deca #13 */"Hyd",          // God of water
			/* Deca #14 */"Oira",         // Goddess of wind
			/* Deca #15 */"Gaia",         // Goddess of earth
			/* Deca #16 */"Plu",          // God of rain
			/* Deca #17 */"Solar",        // God of the sun
			/* Deca #18 */"Etor",         // God of summer
			/* Deca #19 */"Quidion",      // God of the sea
			/* Deca #20 */"Galfrado",     // God of mischief
			/* Deca #21 */"Kasalon",      // Gatekeeper of heaven
			/* Deca #22 */"Ulbion",       // Procecutor of all bad things you do in lfie
			/* Deca #23 */"Yastura",      // Laywer who defends your bad deeds and
			// tries to convice Shalaga of all your good
			// deeds
			/* Deca #24 */"Wirdama",      // Goddess of wisdom
			/* Deca #25 */"Galba",        // God of hope
			/* Deca #26 */"Palandra",     // Goddess who is the caretaker of heaven
			/* Deca #27 */"Atomnus",      // God of Autumn
			/* Deca #28 */"Siren",        // Goddess of Sound
			/* Deca #29 */"Wodolona",     // Goddess of Nature
			/* Deca #30 */"Cantaro",      // God of Numbers
			/* Deca #31 */"Yalura",       // Goddess of Beauty
			/* Deca #32 */"Kanbando",     // God of Knowledge
			/* Deca #33 */"Astralany",    // Goddess of Creation
			/* Deca #34 */"Brokasty",     // Goddess of Caretaking
			/* Deca #35 */"Doniko",       // God of Fate
			/* Deca #36 */"Bell",         // God of Communication
			/* Deca #37 */"Astronium"    // God of the Universe
		};

		int DecaNum() {
			return (DayOfYear() / 10) + 1;
		}

		String DecaName() {
			return DecName[DayOfYear() / 10];
		}
		int DecaDay() {
			return (DayOfYear() % 10) + 1;
		}
		String PhanDate() {
			return DecaName()+"-"+std::to_string(DecaDay());
		}
	}
}
