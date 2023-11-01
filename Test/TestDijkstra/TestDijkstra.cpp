// Lic:
// ***********************************************************
// Units/Test/TestDijkstra/TestDijkstra.cpp
// This particular file has been released in the public domain
// and is therefore free of any restriction. You are allowed
// to credit me as the original author, but this is not
// required.
// This file was setup/modified in:
// 2023
// If the law of your country does not support the concept
// of a product being released in the public domain, while
// the original author is still alive, or if his death was
// not longer than 70 years ago, you can deem this file
// "(c) Jeroen Broks - licensed under the CC0 License",
// with basically comes down to the same lack of
// restriction the public domain offers. (YAY!)
// ***********************************************************
// Version 23.11.01
// EndLic
#include "../../Headers/SlyvQCol.hpp"
#include "../../Headers/SlyvSTOI.hpp"
#include "../../Headers/SlyvString.hpp"
#include "../../Headers/SlyvDijkstra.hpp"

using namespace std;
using namespace Slyvina;
using namespace Units;

string Veld[10]{
"XXXXXXXXXXXXXXXXXXXXXX",   // 0
"S   X            X   X",   // 1
"XX  X     XXXXXX X   X",   // 2
"X   X    XXX   X X X X",   // 3
"X X X    XX  X   X X E",   // 4
"  XXX XXXX   XXXX  XXX",   // 5
" XXX  X  X   X       X",   // 6
"      XX    XX  XXXXXX",   // 7
"XXXXXXXX        XXXXXX",   // 8
"XXXXXXXXXXXXXXXXXXXXXX" }; // 9



bool Block(int x, int y) {
	if (x < 0 || y < 0 || y >= 10 || x >= Veld[y].size()) return true;
	return Veld[y][x] == 'X';
}


int main() {
	int sx, sy, ex, ey;
	for (int y = 0; y < 10; y++) for (int x = 0; x < Veld[y].size(); x++) {
		switch (Veld[y][x]) {
		case 'S': sx = x; sy = y; break;
		case 'E': ex = x; ey = y; break;
		}
	}
	printf("We're going from (%2d,%2d) to (%2d,%2d).\n ", sx, sy, ex, ey);
	//Dijkstra D{ Block,DijkstraDirectionAllow::Everywhere };
	Dijkstra D{ Block };
	printf("Calculating route...\n");
	auto R = D.Route(sx, sy, ex, ey);
	if (!R)
		printf("FAILED!");
	else {
		printf("Done. The route has %d nodes\n\n", (int)R->CountNodes());
		for (auto N = R->First(); N; N = N->Next()) {
			if (Veld[N->y()][N->x()] == ' ') {
				Veld[N->y()][N->x()] = '*';
			} else {
				QCol->Error(TrSPrintF("Tried to override field (%d,%d) (%c)", N->x(), N->y(), Veld[N->y()][N->x()]));
			}
		}
		for (int y = 0; y < 10; y++) {
			for (int x = 0; x < Veld[y].size(); x++) {
				switch (Veld[y][x]) {
				case '*': QCol->Red("*"); break;
				case 'X': QCol->Yellow("X"); break;
				case 'S': QCol->Cyan("S"); break;
				case 'E': QCol->Green("E"); break;
				default: printf(" "); break;
				}
			}
			printf("\n");
		}
	}
	QCol->Reset();
	return 0;
}