#pragma once




#include <atablash/kd.hpp>
#include <atablash/3d/segment.hpp>
#include <atablash/3d/solid-cut.hpp>

#include <atablash/3d/triangle-collision.hpp>


// external - std
#include <sstream>

int dupa = 1000;

namespace ab{




template<class TRIMESH>
struct StickifyData{
	typedef typename TRIMESH::POS_FLOAT F;

	std::vector<uint> v_todo;
	std::vector<uint> p_todo;

	uint vv_done = 0;
	uint ev_done = 0;
	uint ve_done = 0;
	uint ee_done = 0;
	uint pv_done = 0;
	uint vp_done = 0;
	uint pe_done = 0;
	uint edges_done = 0; // edges added to KD
	uint polys_done = 0; // polys added to KD

	typedef KD<F, uint, ab::Vector3<F>> Vertices;
	Vertices vertices = Vertices(3);
	//std::unordered_map<uint, Vertices::Node*> vertices_where;

	typedef KD_AABB<F, uint, ab::Vector3<F>> Edges;
	Edges edges = Edges(3);
	std::unordered_map<uint, typename Edges::Node*> edges_where;

	typedef KD_AABB<F, uint, ab::Vector3<F>> Polys;
	Polys polys = Polys(3);
	std::unordered_map<uint, typename Polys::Node*> polys_where;

	// erase poly (and adjacent edges) from KDs
	inline void p_erase(uint p){
		p_erase_edges(p);
		p_erase_poly(p);
	}

	inline void p_erase_poly(uint p){
		auto r = polys_where.find(p);
		if (r == polys_where.end()) return;

		ASS(r->second->has_element);
		r->second->has_element = false;
		polys_where.erase(r);
	}

	inline void p_erase_edges(uint p){
		for (uint k = 0; k < 3; ++k){
			uint edge = 3 * p + k;
			auto r = edges_where.find(edge);
			if (r == edges_where.end()) continue;

			ASS(r->second->has_element);
			r->second->has_element = false;
			edges_where.erase(r);
		}
	}



	// vertices against vertices' kd
	// todo optim: hash instead of kd-tree
	// returns num welds
	inline uint vv(TRIMESH& mesh, const F& epsVV){
		const F epsVVSqr = epsVV*epsVV;
		uint num = 0;
		while (vv_done < v_todo.size()){
			uint i = v_todo[vv_done++];
			ASS(mesh.vremap[i] == i); // only weld representants
			auto r = vertices.closest(mesh.vs(i).pos());
			if (r.distSqr <= epsVVSqr){
				// weld!
				ASS(mesh.vs(*r.data).pos() == *r.point);
				// TODO:  auto newPos = (r.point + mesh.vs(i).pos()) * (F)0.5;
				mesh.vs(i).pos() = mesh.vs(*r.data).pos();
				mesh.vremap[i] = *r.data;
				++num;
			}
			else{
				vertices.insert(mesh.vs(i).pos(), i);
			}
		}
		return num;
	}



	// add new edges to kd-tree
	template<class TRIMESH>
	void add_edges(TRIMESH& mesh){
		for (; edges_done < p_todo.size(); ++edges_done){
			uint p = p_todo[edges_done];

			if (mesh.pflags[p] & TRIMESH::P_DELETE) continue;

			p_erase_edges(p);

			for (uint k = 0; k < 3; ++k){
				uint a = mesh.ps(p)[k];
				uint b = mesh.ps(p)[k == 2 ? 0 : k + 1];
				a = mesh.vremap[a];
				b = mesh.vremap[b];
				ASS(a != b);
				AABB<ab::Vector3<F>> aabb;

				Segment3<F> segment{ mesh.vs(a).pos(), mesh.vs(b).pos() };
				aabb.fr = segment.fr;
				aabb.to = segment.to;
				if (aabb.fr[0] > aabb.to[0]) std::swap(aabb.fr[0], aabb.to[0]);
				if (aabb.fr[1] > aabb.to[1]) std::swap(aabb.fr[1], aabb.to[1]);
				if (aabb.fr[2] > aabb.to[2]) std::swap(aabb.fr[2], aabb.to[2]);
				auto where = edges.insert(aabb, p * 3 + k);
				ASS(edges_where.find(p * 3 + k) == edges_where.end());
				edges_where[p * 3 + k] = where;
			}
		}
	}






	// add new polys to kd-tree
	template<class TRIMESH>
	void add_polys(TRIMESH& mesh){
		for (; polys_done < p_todo.size(); ++polys_done){
			uint p = p_todo[polys_done];

			if (mesh.pflags[p] & TRIMESH::P_DELETE) continue;

			p_erase_poly(p);

			uint a = mesh.ps(p)[0];
			uint b = mesh.ps(p)[1];
			uint c = mesh.ps(p)[2];
			a = mesh.vremap[a];
			b = mesh.vremap[b];
			c = mesh.vremap[c];

			// important to get remapped v's actual pos
			ab::Triangle3<F> triangle = { mesh.vs(a).pos(), mesh.vs(b).pos(), mesh.vs(c).pos() };
			AABB<ab::Vector3<F>> aabb = triangle.aabb();

			auto where = polys.insert(aabb, p);
			ASS(polys_where.find(p) == polys_where.end());
			polys_where[p] = where;
		}
	}






	// edges against vertices' kd
	template<class TRIMESH>
	inline uint ev(TRIMESH& mesh,
		const typename TRIMESH::POS_FLOAT& epsVE){
		uint num = 0;
		for (; ev_done < p_todo.size(); ++ev_done){
			uint p = p_todo[ev_done];

			if (mesh.pflags[p] & TRIMESH::P_DELETE) continue;

			bool was = false;
			for (uint k = 0; k<3; ++k){
				uint a = mesh.ps(p)[k];
				uint b = mesh.ps(p)[k == 2 ? 0 : k + 1];
				a = mesh.vremap[a];
				b = mesh.vremap[b];
				AABB<ab::Vector3<F>> aabb;
				Segment3<F> segment{ mesh.vs(a).pos(), mesh.vs(b).pos() };
				aabb.fr = segment.fr;
				aabb.to = segment.to;
				if (aabb.fr[0] > aabb.to[0]) std::swap(aabb.fr[0], aabb.to[0]);
				if (aabb.fr[1] > aabb.to[1]) std::swap(aabb.fr[1], aabb.to[1]);
				if (aabb.fr[2] > aabb.to[2]) std::swap(aabb.fr[2], aabb.to[2]);
				aabb.fr[0] -= epsVE;
				aabb.fr[1] -= epsVE;
				aabb.fr[2] -= epsVE;
				aabb.to[0] += epsVE;
				aabb.to[1] += epsVE;
				aabb.to[2] += epsVE;
				vertices.for_each_intersect(aabb, [&](const ab::Vector3<F>& point, uint v){
					if (v == a || v == b) return decltype(vertices)::CONTINUE; // intersection with self endpoint

					auto r = intersection(segment, point);
					if (r.distSqr > epsVE*epsVE) return decltype(vertices)::CONTINUE;

					// don't weld triangles to self
					auto legal = edgeCutLegal(mesh, p*3+k, point);
					if (!legal) return decltype(vertices)::CONTINUE;

					++num;
					auto ec = edgeCut(mesh, p * 3 + k, point);
					if (ec.AVC != uint(-1)) p_erase(ec.AVC);
					if (ec.ADV != uint(-1)) p_erase(ec.ADV);
					if (ec.ADV != uint(-1)) p_todo.push_back(ec.ADV);
					if (ec.AVC != uint(-1)) p_todo.push_back(ec.AVC);
					if (ec.VBC != uint(-1)) p_todo.push_back(ec.VBC);
					if (ec.DBV != uint(-1)) p_todo.push_back(ec.DBV);
					if (ec.V != uint(-1)) mesh.vremap[ec.V] = v;
					if (ec.V2 != uint(-1)) mesh.vremap[ec.V2] = v;

					return decltype(vertices)::BREAK;
				});
				if (num) break;
			}
			if (num) break;
		}
		return num;
	}








	// vertices against edges' kd
	template<class TRIMESH>
	inline uint ve(TRIMESH& mesh,
		const typename TRIMESH::POS_FLOAT& epsVE
		){
		uint num = 0;
		while (ve_done < v_todo.size()){
			uint v = v_todo[ve_done++];

			//ASS(v == mesh.vremap[v]);
			// todo: maybe should ignore such `v`?
			v = mesh.vremap[v];

			AABB<ab::Vector3<F>> aabb;
			aabb.fr = aabb.to = mesh.vs(v).pos();
			aabb.grow(epsVE);

			edges.for_each_intersect(aabb, [&](uint kd_edge){
				uint edge_a = mesh.indices[kd_edge];
				uint edge_b = mesh.indices[kd_edge % 3 == 2 ? kd_edge - 2 : kd_edge + 1];
				edge_a = mesh.vremap[edge_a];
				edge_b = mesh.vremap[edge_b];

				// intersection with self endpoint - ignore
				if (edge_a == v || edge_b == v) return decltype(edges)::CONTINUE;

				Segment3<F> kd_segment{ mesh.vs(edge_a).pos(), mesh.vs(edge_b).pos() };
				auto r = intersection(kd_segment, mesh.vs(v).pos());
				if (r.distSqr > epsVE*epsVE) return decltype(edges)::CONTINUE;

				// don't weld triangles to self
				auto legal = edgeCutLegal(mesh, kd_edge, mesh.vs(v).pos());
				if (!legal) return decltype(vertices)::CONTINUE;

				++num;

				auto ec = edgeCut(mesh, kd_edge, mesh.vs(v).pos());
				if (ec.AVC != uint(-1)) p_erase(ec.AVC);
				if (ec.ADV != uint(-1)) p_erase(ec.ADV);
				if (ec.AVC != uint(-1)) p_todo.push_back(ec.AVC);
				if (ec.VBC != uint(-1)) p_todo.push_back(ec.VBC);
				if (ec.ADV != uint(-1)) p_todo.push_back(ec.ADV);
				if (ec.DBV != uint(-1)) p_todo.push_back(ec.DBV);

				mesh.vremap[ec.V] = mesh.vremap[ec.V2] = mesh.vremap[v];

				return decltype(edges)::ERASE | decltype(edges)::BREAK;
			});
			if (num) break;
		}
		return num;
	}









	// edges against edges' kd
	template<class TRIMESH>
	inline uint ee(TRIMESH& mesh, const typename TRIMESH::POS_FLOAT& epsEE){
		uint num = 0;
		for (; ee_done < p_todo.size(); ++ee_done){
			uint p = p_todo[ee_done];

			if (mesh.pflags[p] & TRIMESH::P_DELETE) continue;

			for (uint k = 0; k<3; ++k){
				uint a = mesh.ps(p)[k];
				uint b = mesh.ps(p)[k == 2 ? 0 : k + 1];
				a = mesh.vremap[a];
				b = mesh.vremap[b];
				AABB<ab::Vector3<F>> aabb;

				Segment3<F> segment{ mesh.vs(a).pos(), mesh.vs(b).pos() };
				aabb.fr = segment.fr;
				aabb.to = segment.to;
				if (aabb.fr[0] > aabb.to[0]) std::swap(aabb.fr[0], aabb.to[0]);
				if (aabb.fr[1] > aabb.to[1]) std::swap(aabb.fr[1], aabb.to[1]);
				if (aabb.fr[2] > aabb.to[2]) std::swap(aabb.fr[2], aabb.to[2]);
				aabb.grow(epsEE);
				edges.for_each_intersect(aabb, [&](uint kd_edge){
					uint edge_a = mesh.indices[kd_edge];
					uint edge_b = mesh.indices[kd_edge % 3 == 2 ? kd_edge - 2 : kd_edge + 1];
					edge_a = mesh.vremap[edge_a];
					edge_b = mesh.vremap[edge_b];
					//if(a == edge_a && b == edge_b) return decltype(edges)::BREAK; // intersection with self - this edge is already in kd-tree
					//if(a == edge_b && b == edge_a) return decltype(edges)::BREAK; // intersection with self - this edge is already in kd-tree

					// intersection with adjacent edge - ignore
					if (a == edge_a || a == edge_b || b == edge_a || b == edge_b) return decltype(edges)::CONTINUE;

					Segment3<F> kd_segment{ mesh.vs(edge_a).pos(), mesh.vs(edge_b).pos() };
					auto r = intersection(segment, kd_segment);
					if (r.distSqr > epsEE*epsEE) return decltype(edges)::CONTINUE;

					++num;

					auto ec = edgeCutOrFlip(mesh, p * 3 + k, r.point);
					if (ec.AVC != uint(-1)) p_erase(ec.AVC);
					if (ec.ADV != uint(-1)) p_erase(ec.ADV);
					if (ec.AVC != uint(-1)) p_todo.push_back(ec.AVC);
					if (ec.VBC != uint(-1)) p_todo.push_back(ec.VBC);
					if (ec.ADV != uint(-1)) p_todo.push_back(ec.ADV);
					if (ec.DBV != uint(-1)) p_todo.push_back(ec.DBV);

					auto ec2 = edgeCutOrFlip(mesh, kd_edge, r.point);
					if (ec2.AVC != uint(-1)) p_erase(ec2.AVC);
					if (ec2.ADV != uint(-1)) p_erase(ec2.ADV);
					if (ec.V != uint(-1)){
						if(ec2.V != uint(-1)) mesh.vremap[ec2.V] = mesh.vremap[ec.V];
						if(ec2.V2 != uint(-1)) mesh.vremap[ec2.V2] = mesh.vremap[ec.V];
					}
					if (ec2.AVC != uint(-1)) p_todo.push_back(ec2.AVC);
					if (ec2.VBC != uint(-1)) p_todo.push_back(ec2.VBC);
					if (ec2.ADV != uint(-1)) p_todo.push_back(ec2.ADV);
					if (ec2.DBV != uint(-1)) p_todo.push_back(ec2.DBV);

					if(ec.V != uint(-1)) v_todo.push_back(mesh.vremap[ec.V]);

					return decltype(edges)::ERASE | decltype(edges)::BREAK;
				});
				if (num) break;
			}
			if (num) break; // first process simpler stickifies
		}
		return num;
	}








	// polys against vertices' kd
	template<class TRIMESH>
	inline uint pv(TRIMESH& mesh, const typename TRIMESH::POS_FLOAT& epsVP){
		typedef typename TRIMESH::POS_FLOAT F;
		uint num = 0;
		while (pv_done < p_todo.size()){
			uint p = p_todo[pv_done++];
			if (mesh.pflags[p] & TRIMESH::P_DELETE) continue;

			uint a = mesh.ps(p)[0];
			uint b = mesh.ps(p)[1];
			uint c = mesh.ps(p)[2];
			a = mesh.vremap[a];
			b = mesh.vremap[b];
			c = mesh.vremap[c];

			// important to get remapped v's actual pos
			ab::Triangle3<F> triangle = { mesh.vs(a).pos(), mesh.vs(b).pos(), mesh.vs(c).pos() };
			AABB<ab::Vector3<F>> aabb = triangle.aabb();
			aabb.grow(epsVP);
			bool was = false;
			vertices.for_each_intersect(aabb, [&](const ab::Vector3<F>& point, uint v){
				if (v == a || v == b || v == c) return decltype(vertices)::CONTINUE; // intersection with self vertex

				auto r = intersection(triangle, point);
				if (r.distSqr > epsVP*epsVP) return decltype(vertices)::CONTINUE;

				++num;
				auto ec = triCut(mesh, p, point);

				// test new polys against everything
				p_todo.push_back(ec.ABV);
				p_todo.push_back(ec.BCV);
				p_todo.push_back(ec.CAV);


				ASS(mesh.vremap[v] == v);
				mesh.vremap[ec.V] = v; //mesh.vremap[v];

				return decltype(vertices)::BREAK;
			});

			if (num) break; // first process simpler stickifies
		}
		return num;
	}







	// vertices against polys' kd
	template<class TRIMESH>
	inline uint vp(TRIMESH& mesh, const typename TRIMESH::POS_FLOAT& epsVP){
		typedef typename TRIMESH::POS_FLOAT F;
		uint num = 0;
		while (vp_done < v_todo.size()){
			uint v = v_todo[vp_done++];

			v = mesh.vremap[v];

			AABB<ab::Vector3<F>> aabb;
			auto point = mesh.vs(v).pos();
			aabb.fr = aabb.to = point;
			aabb.grow(epsVP);

			polys.for_each_intersect(aabb, [&](uint kd_poly){
				if (mesh.pflags[kd_poly] & TRIMESH::P_DELETE) return decltype(polys)::ERASE | decltype(polys)::CONTINUE;

				uint a = mesh.ps(kd_poly)[0];
				uint b = mesh.ps(kd_poly)[1];
				uint c = mesh.ps(kd_poly)[2];
				a = mesh.vremap[a];
				b = mesh.vremap[b];
				c = mesh.vremap[c];

				if (a == v || b == v || c == v){
					return decltype(polys)::CONTINUE;
				}

				ab::Triangle3<F> triangle = { mesh.vs(a).pos(), mesh.vs(b).pos(), mesh.vs(c).pos() };

				auto r = intersection(triangle, point);
				if (r.distSqr > epsVP*epsVP) return decltype(vertices)::CONTINUE;

				++num;

				auto ec = triCut(mesh, kd_poly, point);

				// test new polys against everything
				p_todo.push_back(ec.ABV);
				p_todo.push_back(ec.BCV);
				p_todo.push_back(ec.CAV);

				ASS(mesh.vremap[v] == v);
				mesh.vremap[ec.V] = v; //mesh.vremap[v];

				return decltype(vertices)::BREAK;
			});

			if (num) break; // first process simpler stickifies
		}
		return num;
	}












	// polys against edges' kd
	template<class TRIMESH>
	inline uint pe(TRIMESH& mesh, const typename TRIMESH::POS_FLOAT& epsEP){
		typedef typename TRIMESH::POS_FLOAT F;
		uint num = 0;
		while (pe_done < p_todo.size()){
			uint p = p_todo[pe_done++];
			if (mesh.pflags[p] & TRIMESH::P_DELETE) continue;

			uint a = mesh.ps(p)[0];
			uint b = mesh.ps(p)[1];
			uint c = mesh.ps(p)[2];
			a = mesh.vremap[a];
			b = mesh.vremap[b];
			c = mesh.vremap[c];

			// important to get remapped v's actual pos
			ab::Triangle3<F> triangle = { mesh.vs(a).pos(), mesh.vs(b).pos(), mesh.vs(c).pos() };
			AABB<ab::Vector3<F>> aabb = triangle.aabb();
			aabb.grow(epsEP);

			edges.for_each_intersect(aabb, [&](uint kd_edge){
				uint edge_a = mesh.indices[kd_edge];
				uint edge_b = mesh.indices[kd_edge % 3 == 2 ? kd_edge - 2 : kd_edge + 1];
				edge_a = mesh.vremap[edge_a];
				edge_b = mesh.vremap[edge_b];
				if (edge_a == a || edge_a == b || edge_a == c) return decltype(vertices)::CONTINUE; // intersection with self vertex
				if (edge_b == a || edge_b == b || edge_b == c) return decltype(vertices)::CONTINUE; // intersection with self vertex

				Segment3<F> kd_segment{ mesh.vs(edge_a).pos(), mesh.vs(edge_b).pos() };
				auto r = intersection(triangle, kd_segment);

				/////////////
				if (!r.cut) return decltype(vertices)::CONTINUE;

				++num;
				auto tc = triCut(mesh, p, r.point);
				// test new polys against everything
				p_todo.push_back(tc.ABV);
				p_todo.push_back(tc.BCV);
				p_todo.push_back(tc.CAV);

				
				auto ec = edgeCutOrFlip(mesh, kd_edge, r.point);
				if (ec.AVC != uint(-1)) p_erase(ec.AVC);
				if (ec.ADV != uint(-1)) p_erase(ec.ADV);
				if (ec.AVC != uint(-1)) p_todo.push_back(ec.AVC);
				if (ec.VBC != uint(-1)) p_todo.push_back(ec.VBC);
				if (ec.ADV != uint(-1)) p_todo.push_back(ec.ADV);
				if (ec.DBV != uint(-1)) p_todo.push_back(ec.DBV);
				

				mesh.vremap[ec.V] = tc.V;

				////////////
				return decltype(vertices)::BREAK;
			});

			if (num) break; // first process simpler stickifies
		}
		return num;
	}



};






















	


// stickify if distance <= eps:
// * vertex-vertex
// * vertex-edge (cut edge)
// * edge-edge (cut both edges)
// * vertex-poly (cut poly)
// * edge-poly (cut edge and poly)
template<class TRIMESH>
void stickify(TRIMESH& mesh,
	const typename TRIMESH::POS_FLOAT& epsVV = 1e-1,//1e-2,
	const typename TRIMESH::POS_FLOAT& epsVE = 4e-2,//4e-3,
	const typename TRIMESH::POS_FLOAT& epsEE = 2e-2,//2e-3,
	const typename TRIMESH::POS_FLOAT& epsVP = 1e-3,
	const typename TRIMESH::POS_FLOAT& epsEP = 5e-4
){
	typedef typename TRIMESH::POS_FLOAT F;
	ASS(mesh.isUsingVRemap());
	ASS(mesh.isUsingELinks());
	if (!mesh.isUsingPFlags()){
		mesh.enablePFlags();
	}
	
	StickifyData<TRIMESH> sd;
	
	uint num_vv = 0;
	uint num_ve = 0;
	uint num_ev = 0;
	uint num_ee = 0;
	uint num_pv = 0;
	uint num_vp = 0;
	uint num_pe = 0;
	uint num_ep = 0;
	
	// build list of polys to check against vertices, edges, polys
	// also used for edges
	for(uint p=0; p<mesh.ps().size(); ++p) sd.p_todo.push_back(p);
	//std::random_shuffle(sd.p_todo.begin(), sd.p_todo.end());		// TODO!!!!
	
	// build list of vertices to check against vertices, edges, polys; only vremap roots
	for(uint i=0; i<mesh.vs().size(); ++i) if(mesh.vremap[i] == i) sd.v_todo.push_back(i);
	//std::random_shuffle(sd.v_todo.begin(), sd.v_todo.end());		// TODO!!!!

	int dbg = 0;
	
	for(;;){
		++dbg;
		if (dbg == dupa) break;
		//if (dbg == 1000) break;

		// vertices against vertices
		uint change = sd.vv(mesh, epsVV);
		num_vv += change;
		if(change) continue;


		// delete degenerate triangles, update elinks
		degenerate_delete_mark(mesh, [&sd](uint p){sd.p_erase(p); });
		

		// add new edges to kd-tree
		sd.add_edges(mesh);
		
		
		// edges against vertices
		change = sd.ev(mesh, epsVE);
		num_ev += change;
		if(change) continue;

		

		// vertices against edges
		change = sd.ve(mesh, epsVE);
		num_ve += change;
		if(change) continue;
		
		
		
		// edges against edges
		change = sd.ee(mesh, epsEE);
		num_ee += change;
		if(change) continue;
		
		

		// add new polys to kd-tree
		sd.add_polys(mesh);

		
		// polys against vertices
		change = sd.pv(mesh, epsVP);
		num_pv += change;
		if(change) continue;

		// vertices against polys
		change = sd.vp(mesh, epsVP);
		num_vp += change;
		if(change) continue;
		

		
		
		// polys against edges
		change = sd.pe(mesh, epsEP);
		num_pe += change;
		if(change) continue;
		
		// TODO:
		// edges against polys
		//change = stickify_ep(mesh, epsEP, v_todo, p_todo, ep_done, polys);
		//num_ep += change;
		//if(change) continue;

		
		break;
	}

	apply_pdelete(mesh);
	apply_vremap(mesh);

	

	std::ostringstream oss;
	oss << num_vv << "vv  " << num_ev << "ev  " << num_ve << "ve  " << num_ee << "ee  " << num_pv << "pv  " << num_vp << "vp  " << num_pe << "pe  " << num_ep << "ep";
	LOGD("Stickify result: " + oss.str());
	
}







}



