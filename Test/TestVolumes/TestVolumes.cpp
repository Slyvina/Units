// Lic:
// ***********************************************************
// Units/Test/TestVolumes/TestVolumes.cpp
// This particular file has been released in the public domain
// and is therefore free of any restriction. You are allowed
// to credit me as the original author, but this is not
// required.
// This file was setup/modified in:
// 2022
// If the law of your country does not support the concept
// of a product being released in the public domain, while
// the original author is still alive, or if his death was
// not longer than 70 years ago, you can deem this file
// "(c) Jeroen Broks - licensed under the CC0 License",
// with basically comes down to the same lack of
// restriction the public domain offers. (YAY!)
// ***********************************************************
// Version 22.12.14
// EndLic
#include <SlyvVolumes.hpp>
#include <SlyvDir.hpp>
#include <SlyvQCol.hpp>

using namespace std;
using namespace Slyvina::Units;

int main() {
	auto V{ Volumes() };
	for (auto v : *V) {
		cout << v.first << " -> " << v.second << endl;
	}

	string p = "Slyvina:/Folder1/Folder2/Directory3/";
	cout << p << "\t>>\t"<<AVolPath(p)<<endl;

	auto gt = GetTree(".");
	for (auto f : *gt) {
		//cout << f << endl;
		QCol->Doing("Found", f);
	}
	QCol->Reset();

	return 0;
}