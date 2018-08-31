<<<<<<< HEAD
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
	
	

=======
// Copyright (C) 2017 Adam Blaszkiewicz <atablash@atablash.pl>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#pragma once



#include <Eigen/Dense>
#include <Eigen/Geometry>

#include <type_traits>



namespace ab {


//
// basic implementation, requires input random_shuffled
// TODO: remove recursion
//
template<class SCALAR, int DIM, class VAL = void>
class Kd {

	using Scalar = SCALAR;
	static const int Dim = DIM;
	using Val = VAL;

	using Vec = Eigen::Matrix<Scalar, Dim, 1>;
	enum { NeedsToAlign = (sizeof(Vec)%16)==0 };

private:
	struct Key_Val {

		template<class U, class V>
		Key_Val(const std::pair<U,V>& p) : key(p.first), val(p.second) {}

		template<class U, class V>
		operator std::pair<U,V>() {return {key,val}; }

		const Vec key;
		VAL val;
	};


public:
	using value_type = std::conditional_t<
		std::is_same_v<Val,void>,
			const Vec,
			Key_Val>;


private:
	template<class KV>
	static constexpr Vec key_of_keyval(const KV& kv, typename std::enable_if_t<std::is_same_v<KV, Vec>>* = 0) {
		return kv;
	}

	template<class KV>
	static constexpr Vec key_of_keyval(const KV& kv, typename std::enable_if_t<std::is_same_v<KV, Key_Val>>* = 0) {
		return kv.key;
	}


public:
	~Kd(){
		if(root) delete root;
	}

	template<class KV>
	void insert(KV&& keyval) {
		insert_into(root, std::forward<KV>(keyval), 0);
	}



	value_type* find_closest(const Vec& point) {
		if(!root) return nullptr;
		return root->find_closest_if(point, 0, [](auto){return true;});
	}

	template<class Pred>
	value_type* find_closest_if(const Vec& point, Pred&& pred) {
		if(!root) return nullptr;
		return root->find_closest_if(point, 0, std::forward<Pred>(pred));
	}



	std::vector<value_type*> find_k_closest(const Vec& point, int k) {
		if(!root) return std::vector<value_type*>();
		return root->find_k_closest_if(point, k, 0, [](auto){return true;});
	}

	template<class Pred>
	std::vector<value_type*> find_k_closest_if(const Vec& point, int k, Pred&& pred) {
		if(!root) return std::vector<value_type*>();
		return root->find_k_closest_if(point, k, 0, std::forward<Pred>(pred));
	}



	template<class FUN>
	void each_intersect(const Eigen::AlignedBox<Scalar, Dim>& aabb, FUN&& fun) {
		if(!root) return;
		return root->each_intersect(aabb, std::forward<FUN>(fun), 0);
	}




private:

	struct Node;

	template<class KV>
	inline static void insert_into(Node*& node, KV&& keyval, int axis) {
		if(!node) {
			node = new Node(std::forward<KV>(keyval));
		}
		else {
			node->insert(std::forward<KV>(keyval), axis);
		}
	}

	struct Node {
		Node(const value_type& keyval) : kv(keyval) {}

		~Node(){
			if(l) delete l;
			if(r) delete r;
		}

		template<class KV>
		void insert(KV&& new_keyval, int axis) {
			int new_axis = axis + 1;
			if(new_axis == Dim) new_axis = 0;

			const auto point = key_of_keyval(kv);
			const auto new_point = key_of_keyval((value_type)new_keyval);

			if(new_point[axis] < point[axis]) {
				insert_into(l, std::forward<KV>(new_keyval), new_axis);
			}
			else {
				insert_into(r, std::forward<KV>(new_keyval), new_axis);
			}
		}


		template<class Pred>
		value_type* find_closest_if(const Vec& query_point, int axis, const Pred& pred) {

			int new_axis = axis + 1;
			if(new_axis == Dim) new_axis = 0;

			auto ll = l;
			auto rr = r;

			const auto point = key_of_keyval(kv);

			if(query_point[axis] >= point[axis]) std::swap(ll, rr);

			value_type* cand = nullptr;
			if(ll) cand = ll->find_closest_if(query_point, new_axis, pred);

			if(cand && (key_of_keyval(*cand) - query_point).norm() < std::abs(point[axis] - query_point[axis])) {
				return cand;
			}

			value_type* cand2 = nullptr;
			if(rr) cand2 = rr->find_closest_if(query_point, new_axis, pred);

			value_type* best = nullptr;
			if(pred(kv)) best = &kv;

			if(cand && (!best ||
					(key_of_keyval(*cand) - query_point).squaredNorm() <
					(key_of_keyval(*best) - query_point).squaredNorm())){
				best = cand;
			}

			if(cand2 && (!best ||
					(key_of_keyval(*cand2) - query_point).squaredNorm() <
					(key_of_keyval(*best) - query_point).squaredNorm())){
				best = cand2;
			}

			return best;
		}


		template<class Pred>
		std::vector<value_type*> find_k_closest_if(const Vec& query_point, int k, int axis, const Pred& pred) {

			int new_axis = axis + 1;
			if(new_axis == Dim) new_axis = 0;

			auto ll = l;
			auto rr = r;

			const auto point = key_of_keyval(kv);

			if(query_point[axis] >= point[axis]) std::swap(ll, rr);

			std::vector<value_type*> result;

			if(ll) result = ll->find_k_closest_if(query_point, k, new_axis, pred);

			if((int)result.size() == k) {
				bool all_on_the_left = true;
				for(auto& pkv : result) {

					const auto cand = key_of_keyval(*pkv);

					if((cand - query_point).norm() > std::abs(point[axis] - query_point[axis])) {
						all_on_the_left = false;
						break;
					}
				}

				if(all_on_the_left) return result;
			}

			if(rr) {
				auto cands = rr->find_k_closest_if(query_point, k, new_axis, pred);
				for(auto& cand : cands) result.emplace_back(std::move(cand));
			}

			if(pred(kv)) result.push_back(&kv);

			// TODO: just merge instead of sorting
			if((int)result.size() > k) {

				// TODO: hack... how to capture key_of_keyval directly?
				auto get_key = [](const value_type& keyval){return key_of_keyval(keyval);};

				std::sort(result.begin(), result.end(), [&query_point, &get_key](auto& a, auto& b) {
					auto aa = get_key(*a);
					auto bb = get_key(*b);
					return (aa - query_point).squaredNorm() < (bb - query_point).squaredNorm();
				});
				result.resize(k);
			}

			return result;
		}




		template<class FUN>
		void each_intersect(const Eigen::AlignedBox<Scalar, Dim>& aabb, const FUN& fun, const int axis) {
			int next_axis = axis + 1;
			if(next_axis == Dim) next_axis = 0;

			const auto p = key_of_keyval(kv);
			
			if(aabb.contains(p)) {
				fun(kv);
			}

			if(p[axis] <= aabb.max()[axis]) {
				if(r) r->each_intersect(aabb, fun, next_axis);
			}

			if(p[axis] >= aabb.min()[axis]) {
				if(l) l->each_intersect(aabb, fun, next_axis);
			}
		}




		value_type kv;

		Node* l = nullptr;
		Node* r = nullptr;

	public:
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW_IF(NeedsToAlign)
	};

	Node* root = nullptr;
>>>>>>> 9b72e5ca8c66da1b7cbbd76143d05ab4813c6b92
};
















<<<<<<< HEAD
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

=======





//
// basic implementation, requires input random_shuffled
// TODO: remove recursion
//
template<class SCALAR, int DIM, class VAL = void>
class Kd_Aabb {

	using Scalar = SCALAR;
	static const int Dim = DIM;
	using Val = VAL;

	using Aabb = Eigen::AlignedBox<Scalar, Dim>;
	enum { NeedsToAlign = (sizeof(Aabb)%16)==0 };

	using KeyVal = std::conditional_t<
		std::is_same_v<Val,void>,
			const Aabb,
			std::pair<const Aabb,Val>>;


	// stl
public:
	using value_type = KeyVal;


private:
	template<class KV>
	static Aabb key_of_keyval(const KV& kv, typename std::enable_if_t<std::is_same_v<KV, Aabb>>* = 0) {
		return kv;
	}

	template<class KV>
	static Aabb key_of_keyval(const KV& kv, typename std::enable_if_t<!std::is_same_v<KV, Aabb>>* = 0) {
		return kv.first;
	}


public:
	~Kd_Aabb(){
		if(root) delete root;
	}

	template<class KV>
	void insert(KV&& keyval) {
		insert_into(root, std::forward<KV>(keyval), 0);
	}



	template<class FUN>
	void each_intersect(const Eigen::AlignedBox<Scalar, Dim>& aabb, FUN&& fun) {
		if(!root) return;
		return root->each_intersect(aabb, std::forward<FUN>(fun), 0);
	}

	template<class FUN>
	void each_intersect(const Eigen::Matrix<Scalar, Dim, 1>& p, FUN&& fun) {
		if(!root) return;
		return root->each_intersect(Eigen::AlignedBox<Scalar, Dim>(p, p), std::forward<FUN>(fun), 0);
	}



private:

	struct Node;

	template<class KV>
	inline static void insert_into(Node*& node, KV&& keyval, int axis) {
		if(!node) {
			node = new Node(std::forward<KV>(keyval), axis);
		}
		else {
			node->insert(std::forward<KV>(keyval), axis);
		}
	}

	struct Node {
		Node(const KeyVal& keyval, int axis) : kv(keyval) {
			center = l_to = r_fr = key_of_keyval(keyval).center()[axis];
		}

		~Node(){
			if(l) delete l;
			if(r) delete r;
		}

		template<class KV>
		void insert(KV&& new_keyval, int axis) {
			int new_axis = axis + 1;
			if(new_axis == Dim) new_axis = 0;

			const auto new_aabb = key_of_keyval((KeyVal)new_keyval);

			auto l_extend = new_aabb.max()[axis] - center;
			auto r_extend = center - new_aabb.min()[axis];

			if(l_extend < r_extend) {
				l_to = std::max(l_to, new_aabb.max()[axis]);
				insert_into(l, std::forward<KV>(new_keyval), new_axis);
			}
			else {
				r_fr = std::min(r_fr, new_aabb.min()[axis]);
				insert_into(r, std::forward<KV>(new_keyval), new_axis);
			}
		}




		template<class FUN>
		void each_intersect(const Eigen::AlignedBox<Scalar, Dim>& aabb, const FUN& fun, const int axis) {
			int next_axis = axis + 1;
			if(next_axis == Dim) next_axis = 0;

			const auto my_aabb = key_of_keyval(kv);
			
			if(my_aabb.intersects(aabb)) {
				fun(kv);
			}

			if(r && r_fr <= aabb.max()[axis]) {
				r->each_intersect(aabb, fun, next_axis);
			}

			if(l && l_to >= aabb.min()[axis]) {
				l->each_intersect(aabb, fun, next_axis);
			}
		}




		KeyVal kv;

		Node* l = nullptr;
		Node* r = nullptr;

		Scalar center;
		Scalar l_to;
		Scalar r_fr;

	public:
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW_IF(NeedsToAlign)
	};

	Node* root = nullptr;
>>>>>>> 9b72e5ca8c66da1b7cbbd76143d05ab4813c6b92
};











<<<<<<< HEAD
}
=======









} // namespace ab

>>>>>>> 9b72e5ca8c66da1b7cbbd76143d05ab4813c6b92


