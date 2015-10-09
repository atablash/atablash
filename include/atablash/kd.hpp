#pragma once

#include <atablash/types.hpp>
#include <atablash/3d/aabb.hpp>

namespace ab{
	
	
	
	



// kd-tree for points
// dynamic number of dimensions
//
// not really optimized
//
// please random_shuffle the input
//
template<class T, class DATA = Void, class VECTOR = std::vector<T> >
class KD{

private:
	const uint dims;

	struct Node{
		Node(uint axis_, const T _cut, const VECTOR& _point, const DATA& _data = DATA())
			: axis(axis_), n0(NULL), n1(NULL), cut(_cut), point(_point), data(_data) {}

		~Node(){
			if (n0) delete n0;
			if (n1) delete n1;
		}

		// node data
		uint axis;
		Node *n0 = nullptr;
		Node *n1 = nullptr;
		T cut;	// duplicate to avoid jumping to point's memory


		// point data
		bool has_element = true;
		const VECTOR point;
		DATA data;
	};

	Node* root;

public:
	KD(uint dimensions) : dims(dimensions), root(NULL) {}
	~KD(){
		if(root) delete root;
	}

	Node* insert(const VECTOR& point, const DATA& data = DATA()){
		if(root == NULL){
			root = new Node(0, point[0], point, data);
			return root;
		}

		int caxis = 0;
		Node* cnode = root;

		for(;;){
			auto next_axis = caxis==dims-1 ? 0 : caxis+1;
			if(point[caxis] >= cnode->cut){
				if(cnode->n1 == NULL){
					return cnode->n1 = new Node(next_axis, point[next_axis], point, data);
				}
				cnode = cnode->n1;
			}
			else{
				if(cnode->n0 == NULL){
					return cnode->n0 = new Node(next_axis, point[next_axis], point, data);
				}
				cnode = cnode->n0;
			}
			if(++caxis == dims) caxis = 0;
		}
		ASS(false);
	}

	// returns a point closest to 'point' along with its data
	//
	// kd-tree must contain at least 1 point
	//
	struct ClosestResult{
		const VECTOR* point;
		DATA* data;
		T distSqr;
	};
	ClosestResult closest(const VECTOR& point){
		//ASS(root);
		if (!root){
			return{ nullptr, nullptr, std::numeric_limits<T>::max() };
		}

		// DFS
		// node, caxis
		std::vector<Node*> s;
		s.pub(root);

		T bestSqr = std::numeric_limits<T>::max();
		Node* bestNode = NULL;

		while(!s.empty()){
			auto cnode = s.back();
			s.pop_back();
			
			auto caxis = cnode->axis;

			T minDist = cnode->cut - point[caxis];
			if(minDist*minDist >= bestSqr){
				// skip one of sons
				if(minDist > 0){
					if(cnode->n0) s.pub(cnode->n0);
				}
				else{
					if(cnode->n1) s.pub(cnode->n1);
				}
			}
			else{
				// check cnode->point
				if (cnode->has_element){
					T cand = 0;
					for (uint i = 0; i < dims; ++i){
						T d = point[i] - cnode->point[i];
						cand += d*d;
					}
					if (cand < bestSqr){
						bestSqr = cand;
						bestNode = cnode;
					}
				}

				// no skipping, DFS into children
				if(cnode->n0) s.pub(cnode->n0);
				if(cnode->n1) s.pub(cnode->n1);
			}
		}

		if(bestNode) return{ &bestNode->point, &bestNode->data, bestSqr };
		else return{ nullptr, nullptr, std::numeric_limits<T>::max() };
	}


	static const int CONTINUE = 0;
	static const int BREAK = 1;
	static const int ERASE = 2;
	
	// call fun for each point inside given AABB
	template<class FUN>
	void for_each_intersect(const AABB<VECTOR>& aabb, const FUN&& fun){
		std::vector<Node*> s;
		s.push_back(root);
		
		while(!s.empty()){
			auto cnode = s.back();
			s.pop_back();
			
			if(cnode->has_element && intersect(aabb,  cnode->point)){
				auto flags = fun(cnode->point, cnode->data);
				if (flags & ERASE){
					cnode->has_element = false;
				}
				if(flags & BREAK) return;
			}
			
			if(cnode->n0 && aabb.fr[cnode->axis] < cnode->cut){
				s.push_back(cnode->n0);
			}
			if(cnode->n1 && aabb.to[cnode->axis] > cnode->cut){
				s.push_back(cnode->n1);
			}
		}
	}
	
	

};
















// kd-tree for AABB
// dynamic number of dimensions
//
// not really optimized
//
// please random_shuffle the input
//
template<class F, class DATA = Void, class VECTOR = std::vector<T> >
class KD_AABB{

public://private:	// todo
	const int dims;

	struct Node{
		Node(uint axis_, const F cut, const AABB<VECTOR>& aabb_, const DATA& _data = DATA())
			: axis(axis_), n0_to(cut), n1_fr(cut), aabb(aabb_), data(_data) {}

		~Node(){
			if (n0) delete n0;
			if (n1) delete n1;
		}

		// node info
		uint axis;
		Node *n0 = nullptr;
		Node *n1 = nullptr;
		F n0_to, n1_fr;

		// AABB info
		bool has_element = true;
		const AABB<VECTOR> aabb;
		DATA data;
	};

	Node* root;

public:
	KD_AABB(int dimensions) : dims(dimensions), root(NULL) {}
	~KD_AABB(){
		if(root) delete root;
	}

	Node* insert(const AABB<VECTOR>& aabb, const DATA& data = DATA()){
		if(root == NULL){
			auto cut = (aabb.fr[0] + aabb.to[0]) * (F)0.5;
			return root = new Node(0, cut, aabb, data);
		}

		int caxis = 0;
		Node* cnode = root;

		for(;;){
			// auto aabb_axis_center = (F)0.5 * (aabb_fr[caxis] + aabb_to[caxis]);
			auto aabb_n0_fit = cnode->n0_to - aabb.to[caxis];
			auto aabb_n1_fit = aabb.fr[caxis] - cnode->n1_fr;
			auto next_axis = caxis==dims-1 ? 0 : caxis + 1;
			if(aabb_n1_fit > aabb_n0_fit){
				if(cnode->n1 == NULL){
					auto cut = (aabb.fr[next_axis] + aabb.to[next_axis]) * (F)0.5;
					return cnode->n1 = new Node(next_axis, cut, aabb, data);
				}
				if(aabb_n1_fit < 0) cnode->n1_fr = aabb.fr[caxis];
				cnode = cnode->n1;
			}
			else{
				if(cnode->n0 == NULL){
					auto cut = (aabb.fr[next_axis] + aabb.to[next_axis]) * (F)0.5;
					return cnode->n0 = new Node(next_axis, cut, aabb, data);
				}
				if(aabb_n0_fit < 0) cnode->n0_to = aabb.to[caxis];
				cnode = cnode->n0;
			}
			caxis = next_axis;
		}
	}
	
	static const int CONTINUE = 0;
	static const int BREAK    = 1;
	static const int ERASE    = 2;

	// call fun for each intersecting AABB
	template<class FUN>
	void for_each_intersect(const AABB<VECTOR>& aabb, const FUN&& fun){
		std::vector<Node*> s;
		s.push_back(root);
		
		while(!s.empty()){
			auto cnode = s.back();
			s.pop_back();
			
			if(cnode->has_element && intersect(aabb, cnode->aabb)){
				auto flags = fun(cnode->data);
				if(flags & ERASE){
					cnode->has_element = false;
				}
				if(flags & BREAK) return;
			}
			
			/*if(cnode->n0 && aabb.fr[cnode->axis] < cnode->n0_to){
				s.push_back(cnode->n0);
			}
			if(cnode->n1 && aabb.to[cnode->axis] > cnode->n1_fr){
				s.push_back(cnode->n1);
			}*/

			if(cnode->n0){
				s.push_back(cnode->n0);
			}
			if(cnode->n1){
				s.push_back(cnode->n1);
			}
		}
	}

};











}


