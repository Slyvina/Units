// License:
// 	Units/Headers/SlyvSharedArray.hpp
// 	Slyvina - Shared Array
// 	version: 24.11.25
// 
// 	Copyright (C) 2024 Jeroen P. Broks
// 
// 	This software is provided 'as-is', without any express or implied
// 	warranty.  In no event will the authors be held liable for any damages
// 	arising from the use of this software.
// 
// 	Permission is granted to anyone to use this software for any purpose,
// 	including commercial applications, and to alter it and redistribute it
// 	freely, subject to the following restrictions:
// 
// 	1. The origin of this software must not be misrepresented; you must not
// 	   claim that you wrote the original software. If you use this software
// 	   in a product, an acknowledgment in the product documentation would be
// 	   appreciated but is not required.
// 	2. Altered source versions must be plainly marked as such, and must not be
// 	   misrepresented as being the original software.
// 	3. This notice may not be removed or altered from any source distribution.
// End License
#pragma once

#include <memory>
#include <vector>
#include <map>
#include <string>
#include <iostream>


#define Panic Slyvina_SharedArray_Panic_56fc8eaeba2f10dbc003e418cdc921b9

namespace Slyvina {
	namespace Units {

		typedef void(*SharedArrayPanic)(std::string);
		void SharedArraySetPanic(SharedArrayPanic f);
		void Panic(std::string f);

		template <class MyClass> class SharedVector;
		template <class MyClass> class UniqueVector;
		template <class Key, class Value> class UniqueMap;
		template <class Key, class Value> class SharedMap;
		template <class MyType> class SharedArray;
		template <class MyType> class UniqueArray;
		typedef SharedMap<std::string, std::string> ShStringMap;
		typedef UniqueMap<std::string, std::string> UnStringMap;
		typedef SharedVector<std::string> ShVecString;
		typedef UniqueVector<std::string> UnVecString;


		template <class MyClass> class SharedVector {
		private:
			std::shared_ptr<std::vector<MyClass>> _actvector{ nullptr };
		public:
			inline SharedVector<MyClass>(std::vector<MyClass>* c) { _actvector = std::shared_ptr<std::vector<MyClass>>(c); }
			inline SharedVector<MyClass>() { _actvector = std::make_shared<std::vector<MyClass>>(); }
			inline std::vector<MyClass>* get() { return _actvector.get(); }
			inline size_t size() { return _actvector->size(); }
			inline MyClass& operator[](size_t idx) { if (!_actvector) { Panic("Can't index nullvector"); MyClass Niks{ nullptr }; return Niks; } return (*_actvector)[idx]; }
			inline void operator+=(MyClass v) { _actvector ? _actvector.get()->push_back(v) : Panic("Can't add to nullvector"); }
			inline std::vector<MyClass>& operator*() const noexcept { return *_actvector; }
			inline std::vector<MyClass>& operator->() const noexcept { return *_actvector; }
			inline void clear() { _actvector ? _actvector->clear() : Panic("Can't clear nullvector"); }
			//inline std::vector<MyClass>::iterator begin() { return _actvector->begin(); }
			//inline std::vector<MyClass>::iterator end() { return _actvector->end(); }
		}; 

		template <class MyClass> class UniqueVector {
		private:
			std::unique_ptr<std::vector<MyClass>> _actvector{ nullptr };
		public:
			inline UniqueVector<MyClass>(std::vector<MyClass>* c) { _actvector = std::unique_ptr<std::vector<MyClass>>(c); }
			inline UniqueVector<MyClass>() { _actvector = std::make_unique<std::vector<MyClass>>(); }
			inline std::vector<MyClass>* get() { return _actvector.get(); }
			inline size_t size() { return _actvector->size(); }
			inline MyClass& operator[](size_t idx) { if (!_actvector) { Panic("Can't index nullvector"); MyClass Niks{ nullptr }; return Niks; } return (*_actvector)[idx]; }
			inline void operator+=(MyClass v) { _actvector ? _actvector.get()->push_back(v) : Panic("Can't add to nullvector"); }
			inline std::vector<MyClass>& operator*() const noexcept { return *_actvector; }
			inline std::vector<MyClass>& operator->() const noexcept { return *_actvector; }
			inline void clear() { _actvector ? _actvector->clear() : Panic("Can't clear nullvector"); }
			//inline std::vector<MyClass>::iterator begin() { return _actvector->begin(); }
			//inline std::vector<MyClass>::iterator end() { return _actvector->end(); }
		};

		template <class Key, class Value> class SharedMap {
		private:
			std::shared_ptr < std::map<Key, Value> >_actmap{ nullptr };
			Value Niks{};
		public:
			inline SharedMap<Key, Value>() { _actmap = std::shared_ptr < std::map<Key, Value> >(new std::map<Key, Value>()); }
			inline SharedMap<Key, Value>(std::map<Key, Value>&  CopyFrom) {
				_actmap = std::shared_ptr < std::map<Key, Value>>(new std::map<Key, Value>());
				for (auto& cpf : CopyFrom) 
					(*_actmap)[cpf.first] = cpf.second;
			}
			inline void clear() { _actmap ? _actmap->clear() : Panic("Can't clear nullmap"); }
			inline bool count(Key k) { return _actmap != nullptr && _actmap->count(k); }
			inline Value& operator[] (Key k) {
				if (!_actmap) { Panic("Can't index nullmap"); return Niks; }
				return (*_actmap)[k];
			}
			/// <summary>
			/// Contrary to using the [] operatie CkV will throw an error when the key doesn't exist.
			/// </summary>
			inline Value& CkV(Key k) {
				if (count(k)) return (*_actmap)[k];
				Panic("Key doesn't exist!"); // I cannot display the key. I don't what type it will have.
				return Niks;
			}
			/// <summary>
			/// Only accepts the value if the key is not yet in the map
			/// </summary>
			/// <param name="k">key</param>
			/// <param name="v">value</param>
			inline void NewValue(Key k, Value v) {
				if (!count(k)) {
					if (!_actmap) { Panic("Can't index nullmap"); return; }
					(*_actmap)[k] = v;
				}
			}

			inline void erase(Key K) { _actmap ? _actmap->erase(K) : Panic("Can't erase from nullmap!"); }
			inline size_t size() { if (_actmap) return _actmap->size(); Panic("Can't get the size from nullmap!"); }
			
			bool hasmap() { return _actmap != nullptr; }
			std::map<Key, Value>* get() { return _actmap.get(); }
			std::map<Key, Value>& operator*() { return *get(); }
			std::map<Key, Value>& operator->() { return *get(); }
		};


		template <class Key, class Value> class UniqueMap {
		private:
			std::unique_ptr < std::map<Key, Value> >_actmap{ nullptr };
			Value Niks{};
		public:
			inline UniqueMap<Key, Value>() { _actmap = std::unique_ptr < std::map<Key, Value> >(new std::map<Key, Value>()); }
			inline UniqueMap<Key, Value>(std::map<Key, Value>& CopyFrom) {
				_actmap = std::unique_ptr < std::map<Key, Value>>(new std::map<Key, Value>());
				for (auto& cpf : CopyFrom)
					(*_actmap)[cpf.first] = cpf.second;
			}
			inline void clear() { _actmap ? _actmap->clear() : Panic("Can't clear nullmap"); }
			inline bool count(Key k) { return _actmap != nullptr && _actmap->count(k); }
			inline Value& operator[] (Key k) {
				if (!_actmap) { Panic("Can't index nullmap"); return Niks; }
				return (*_actmap)[k];
			}
			/// <summary>
			/// Contrary to using the [] operatie CkV will throw an error when the key doesn't exist.
			/// </summary>
			inline Value& CkV(Key k) {
				if (count(k)) return (*_actmap)[k];
				Panic("Key doesn't exist!"); // I cannot display the type. I don't what type it will have.
				return Niks;
			}
			/// <summary>
			/// Only accepts the value if the key is not yet in the map
			/// </summary>
			/// <param name="k">key</param>
			/// <param name="v">value</param>
			inline void NewValue(Key k, Value v) {
				if (!count(k)) {
					if (!_actmap) { Panic("Can't index nullmap"); return; }
					(*_actmap)[k] = v;
				}
			}

			inline void erase(Key K) { _actmap ? _actmap->erase(K) : Panic("Can't erase from nullmap!"); }
			inline size_t size() { if (_actmap) return _actmap->size(); Panic("Can't get the size from nullmap!"); }

			bool hasmap() { return _actmap != nullptr; }
			std::map<Key, Value>* get() { return _actmap.get(); }
			std::map<Key, Value>& operator*() { return *get(); }
			std::map<Key, Value>& operator->() { return *get(); }
		};

		template <class MyType> class SharedArray {
			class ActArray {
			public:
				MyType* _array{ nullptr };
				size_t _size{ 0 };
				inline ActArray() {};
				inline ActArray(size_t s) { _size = s; _array = new MyType[s]; }
				inline ActArray(size_t s,const MyType* a) {
					_size = s; 
					_array = new MyType[s]; 
					for (size_t i = 0; i < s; ++i) _array[i] = a[i];
				}
//				inline ActArray(std::vector<MyType>& a) {
//					_size = a.size();
//					_array = new MyType[_size];
//					for (size_t i = 0; i < _size; ++i) _array[i] = a[i];
//				}
				inline ~ActArray() { if (_array) delete[] _array; }
			};
		private:
			std::shared_ptr<ActArray> _myarray{ nullptr };
			MyType Niks{};
		public:
			inline SharedArray() {};
			inline SharedArray(size_t s) { _myarray = std::shared_ptr<ActArray>(new ActArray(s)); }
			inline SharedArray(size_t s, const MyType* a) { _myarray = std::shared_ptr<ActArray>(new ActArray(s,a)); }
			inline MyType& operator[](size_t i) {
				if (!_myarray) { Panic("Can't index nullarray!"); return Niks; }
				if (i >= _myarray->_size) { Panic("Index out of bounds! (" + std::to_string(i) + "/" + std::to_string(_myarray->_size) + ")"); return Niks; }
				return _myarray->_array[i];
			}
			inline MyType* get() { return _myarray->_array; }
			inline MyType* operator*() const noexcept { return _myarray->_array; }
			inline size_t size() { return _myarray->_size; }
		};


		template <class MyType> class UniqueArray {
			class ActArray {
			public:
				MyType* _array{ nullptr };
				size_t _size{ 0 };
				inline ActArray() {};
				inline ActArray(size_t s) { _size = s; _array = new MyType[s]; }
				inline ActArray(size_t s, const MyType* a) {
					_size = s;
					_array = new MyType[s];
					for (size_t i = 0; i < s; ++i) _array[i] = a[i];
				}
				inline ~ActArray() { if (_array) delete[] _array; }
			};
		private:
			std::unique_ptr<ActArray> _myarray{ nullptr };
			MyType Niks{};
		public:
			inline UniqueArray() {};
			inline UniqueArray(size_t s) { _myarray = std::unique_ptr<ActArray>(new ActArray(s)); }
			inline UniqueArray(size_t s, const MyType* a) { _myarray = std::unique_ptr<ActArray>(new ActArray(s, a)); }
			//inline UniqueArray(std::vector<MyType> a) { _myarray = std::unique_ptr<ActArray>(new ActArray(a)); }
			inline MyType& operator[](size_t i) {
				if (!_myarray) { Panic("Can't index nullarray!"); return Niks; }
				if (i >= _myarray->_size) { Panic("Index out of bounds! (" + std::to_string(i) + "/" + std::to_string(_myarray->_size) + ")"); return Niks; }
				return _myarray->_array[i];
			}
			inline MyType* get() { return _myarray->_array; }
			inline MyType* operator*() const noexcept { return _myarray->_array; }
			inline size_t size() { return _myarray->_size; }
		};
	}
}

#undef Panic




