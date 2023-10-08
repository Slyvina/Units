#include <map>
#include <vector>

#include "../Headers/SlyvString.hpp"
#include "../Headers/SlyvDijkstra.hpp"

#define NewNode(nextnode,x,y) std::shared_ptr<_DijkstraNode>(new _DijkstraNode(nextnode,x,y))

	// if(_x!=Node->x() && _y!=Node->y()) {

#define CheckSpot(Node,_x,_y) {\
		Spot s{ _x,_y };\
		if ((!WorkGrid.count(s.ts())) && Allow(_x,_y)) { \
			auto NN{ NewNode(Node, _x,_y) }; \
			WorkGrid[s.ts()] = NN; \
			Babies.push_back(NN); \
			hasmore = true; \
			stillactive++;\
			if(startx==_x && starty==_y) return std::shared_ptr<_DijkstraRoute>(new _DijkstraRoute(NN));\
		}\
	}
//}

#undef CountDownCrash 
#undef DEBUG_Dijkstra


#ifdef DEBUG_Dijkstra
#define Chat(a) std::cout << "DEBUG> " << a<<std::endl
#else
#define Chat(a)
#endif

namespace Slyvina {
	namespace Units {

#ifdef CountDownCrash
		static const int countdownstart{ 1000 };
		static int countdown{ 0 };
#endif
		class Spot {
		public:
			int x{ 0 }, y{ 0 };
			std::string ts() { return TrSPrintF("%d::%d",x,y); }
			Spot(int _x, int _y) { x = _x; y = _y; }
		};

		_DijkstraRoute::_DijkstraRoute(DijkstraNode StartNode) {
			_first = StartNode;

		}

		size_t _DijkstraRoute::CountNodes() {
			size_t r{ 0 };
			for (auto n = _first; n; n=n->Next()) {
				r++;
				//Chat("Counting nodes! " << (int)n.get() << " -> " << r);
			}
			return r;
		}




		DijkstraRoute Dijkstra::Route(int startx, int starty, int endx, int endy) {
			auto end_node = NewNode(nullptr, endx, endy);
			Spot es{ endx, endy };
			std::map<std::string, DijkstraNode> WorkGrid{{es.ts(), end_node}};
			//#ifndef ignore
			std::map<size_t, DijkstraNode> ActiveNodes{{0, end_node}};
			size_t ANID{ 0 }; // Active Nodes ID.
			size_t stillactive{ 0 };
			if (Block(endx, endy)) return nullptr;
#ifdef CountDownCrash
			countdown = countdownstart;
#endif
			do {
#ifdef CountDownCrash
				printf("Countdown: %8d\r", countdown);
				if (--countdown <= 0) {
					printf("ERROR! Timeout!");
					return nullptr;
				}
#endif
				stillactive = 0;
				std::vector<size_t> Victims{};
				std::vector<DijkstraNode> Babies{};
				Chat("Start new check: " << Babies.size() << " (must AWLAYS be ZERO!)");
				for (auto CN : ActiveNodes) {
					auto hasmore{ false };
					auto NID{ CN.first };
					auto Node{ CN.second };
					//int chk = 0; // debug
					//for (int chk = -1; chk <= 1; chk+=2) {
					{ int chk = 1;
						/*
						{
							Spot s{ Node->x() + chk, Node->y() };
							if ((!WorkGrid.count(s.ts())) && Allow(Node->x() + chk, Node->y())) {
								auto NN{ NewNode(Node, Node->x() + chk, Node->y()) };
								WorkGrid[s.ts()] = NN;
								Babies.push_back(NN);
								hasmore = true;
								stillactive++;
								if (startx == Node->x() + chk && starty == Node->y()) return std::shared_ptr<_DijkstraRoute>(new _DijkstraRoute(NN));
							}
						};
						//*/
						//*

						//*/
						//*
						CheckSpot(Node, Node->x() + chk, Node->y());
						CheckSpot(Node, Node->x(), Node->y() + chk);
						CheckSpot(Node, Node->x() - chk, Node->y());
						CheckSpot(Node, Node->x(), Node->y() - chk);
						//*/
						switch (DirectionAllow) {
						case DijkstraDirectionAllow::OnlyStraight:
							break;
						case DijkstraDirectionAllow::Everywhere:
							//*
							CheckSpot(Node, Node->x() + chk, Node->y() + chk);
							CheckSpot(Node, Node->x() + chk, Node->y() - chk);
							CheckSpot(Node, Node->x() - chk, Node->y() + chk);
							CheckSpot(Node, Node->x() - chk, Node->y() - chk);
							//*/
							break;
						case DijkstraDirectionAllow::DiagonalIfStraightIsPossible:
							//*
							if (Allow(Node->x() + chk, Node->y()) && Allow(Node->x(), Node->y() - chk)) CheckSpot(Node, Node->x() + chk, Node->y() - chk);
							if (Allow(Node->x() + chk, Node->y()) && Allow(Node->x(), Node->y() + chk)) CheckSpot(Node, Node->x() + chk, Node->y() + chk);
							if (Allow(Node->x() - chk, Node->y()) && Allow(Node->x(), Node->y() - chk)) CheckSpot(Node, Node->x() - chk, Node->y() - chk);
							if (Allow(Node->x() - chk, Node->y()) && Allow(Node->x(), Node->y() + chk)) CheckSpot(Node, Node->x() - chk, Node->y() + chk);
							//*/
							break;
						}
					}
					if (!hasmore) Victims.push_back(NID);
				}
				for (auto Baby : Babies) { ActiveNodes[++ANID] = Baby; }
				for (auto Victim : Victims) { ActiveNodes.erase(Victim); }
				Chat("Nodes Born=" << Babies.size() << "; Nodes Killed=" << Victims.size());
			} while (stillactive);
			//#endif
			return nullptr;
		}



		_DijkstraNode::_DijkstraNode(DijkstraNode N, int __x, int __y) {
			_next = N; _x = __x; _y = __y;
#ifdef DEBUG_Dijkstra
			if (_next)
				Chat("Node created at (" << _x << "," << _y << ") from (" << _next->x() << "," << _next->y() << ")");
			else
				Chat("Node created at (" << _x << "," << _y << ")");
#endif
		}

	}
}
