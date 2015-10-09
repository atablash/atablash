#pragma once

#include <atablash/patterns/ecs.hpp>

#include <atablash/types.hpp>
#include <atablash/linalg/2/vector.hpp>
#include <atablash/linalg/3/vector.hpp>

#include <atablash/3d/triangle.hpp>

#include "trimesh-common.hpp"
#include "hashweld.hpp"

#include "build-elinks.hpp"

#include <atablash/union-find.hpp>


namespace ab{





template<int x> struct LOG2{static const int r = LOG2<(x>>1)>::r + 1;};
template<> struct LOG2<1>{ static const int r = 0; };




// optimized for OpenGL
// vertices with several UVs are duplicated
// whole TriMesh is the same material
// connect material in separate component object (with shared_ptr inside?)
class TriMeshD : public ab::Entity{
public:	
	typedef long double POS_FLOAT;	// TODO: szablon albo nie wiem
	typedef ui32 IDX_INT;  // index array element type // TODO: szablon albo nie wiem (ui16 vs ui32)
	typedef ui32 IIDX_INT; // index array index type
private:
	static const int dimensions = 3;
private:
	ui8 voffsets[LOG2<(int)VExtra::terminator>::r + 1];
	ui8 vsize = dimensions * sizeof(POS_FLOAT);

	// indices (separate from polygons extra data because of opengl)
	//ui8 isize = 3*sizeof(idxint);

	// polygons
	//ui8 psize = 0; // sizeof 1 polygon extras

public:
	TriMeshD(VExtra vextras = (VExtra)0){
		init(vextras);
	}
	
	TriMeshD& operator=(const TriMeshD&) = default;

	void init(VExtra vextras){
		clear();
		memset(voffsets, 0, sizeof(voffsets));
		int coffset = sizeof(POS_FLOAT)*dimensions;
		for(uint i=0;; ++i){
			auto mask = (1u << i);
			if(mask >= (uint)VExtra::terminator)break;
			if((uint)vextras & mask){
				voffsets[i] = coffset;
				coffset += VExtraSizes[i];
			}
		}
		vsize = coffset;
	}

	void clear(){
		numv = 0;
		vdata.clear();
		
		nump = 0;
		//pdata.clear();
		//idata.clear();
		indices.clear();
	}


	//
	// vertices access
	//

private:
	template<class> class Vs;
	template<class> class V;
	
	template<> class V<TriMeshD>{
		friend Vs<TriMeshD>; friend TriMeshD;
	public:
		ab::Vector3<POS_FLOAT>& pos(){
			return *(ab::Vector3<POS_FLOAT>*)&g.vdata[ith*g.vsize];
		}
		// todo: color + alpha v4b
		v3b& rgb(){
			int ioff = LOG2<(int)VExtra::RGB>::r;
			ASS(g.voffsets[ioff]);
			return *(v3b*)&g.vdata[ith*g.vsize + g.voffsets[ioff]];
		}
		ui8& alpha(){
			int ioff = LOG2<(int)VExtra::ALPHA>::r;
			ASS(g.voffsets[ioff]);
			return *(ui8*)&g.vdata[ith*g.vsize + g.voffsets[ioff]];
		}
		v2f& uv(){
			int ioff = LOG2<(int)VExtra::UV>::r;
			ASS(g.voffsets[ioff]);
			return *(v2f*)&g.vdata[ith*g.vsize + g.voffsets[ioff]];
		}
		v3f& normal(){
			int ioff = LOG2<(int)VExtra::NORMAL>::r;
			ASS(g.voffsets[ioff]);
			return *(v3f*)&g.vdata[ith*g.vsize + g.voffsets[ioff]];
		}
		template<class O>
		V& operator=(const O& o){
			ASS(sizeof(g.voffsets) == sizeof(o.g.voffsets));
			for(uint i=0; i<sizeof(g.voffsets); ++i) ASS(g.voffsets[i] == o.g.voffsets[i]); // for now only assignments of same vertex structures are supported
			memcpy(&g.vdata[ith*g.vsize], &o.g.vdata[o.ith*o.g.vsize], g.vsize);
			if(g.usingVRemap){
				if(&g == &o.g) g.vremap[ith] = o.g.vremap[o.ith];
				else g.vremap[ith] = ith; // erase vremap if moving vertex from another mesh
			}
			return *this;
		}
		
		template<class F>
		V& lerpAttributes(V& a, V& b, const F& t){
			F tt = 1.0f - t;
			if(g.rgbOffset())    rgb()    = tt * a.rgb()     +  t * b.rgb();
			if(g.alphaOffset())  alpha()  = (ui8) (tt * a.alpha()   +  t * b.alpha()); // check bounds on cast?
			if(g.uvOffset())     uv()     = tt * a.uv()      +  t * b.uv();
			if(g.normalOffset()) normal() = tt * a.normal()  +  t * b.normal();
			// TODO: how to avoid those ifs?
			return *this;
		}
		
		template<class F>
		V& lerpAttributes(V& a, V& b, V& c, const Vector2<F>& alphas){
			if(g.rgbOffset())    rgb()    = a.rgb()     +  alphas.x * (b.rgb() - a.rgb())     +  alphas.y * (c.rgb() - a.rgb());
			if(g.alphaOffset())  alpha()  = (ui8)(a.alpha()     +  alphas.x * (b.alpha() - a.alpha())     +  alphas.y * (c.alpha() - a.alpha()));
			if(g.uvOffset())     uv()     = a.uv()     +  alphas.x * (b.uv() - a.uv())     +  alphas.y * (c.uv() - a.uv());
			if(g.normalOffset()) normal() = a.normal()  +  alphas.x * (b.normal() - a.normal())     +  alphas.y * (c.normal() - a.normal());
			// TODO: how to avoid those ifs?
			return *this;
		}
		
	private:
		V(TriMeshD& _g, int _ith) : g(_g), ith(_ith){}
		TriMeshD& g;
		int ith;
	};
	
	
	template<> class V<const TriMeshD>{
		friend Vs<const TriMeshD>; friend Vs<TriMeshD>; friend TriMeshD;
	public:
		const ab::Vector3<POS_FLOAT>& pos(){
			return *(ab::Vector3<POS_FLOAT>*)&g.vdata[ith*g.vsize];
		}
		// todo: color + alpha v4b
		const v3b& rgb(){
			int ioff = LOG2<(int)VExtra::RGB>::r;
			ASS(g.voffsets[ioff]);
			return *(v3b*)&g.vdata[ith*g.vsize + g.voffsets[ioff]];
		}
		const ui8& alpha(){
			int ioff = LOG2<(int)VExtra::ALPHA>::r;
			ASS(g.voffsets[ioff]);
			return *(ui8*)&g.vdata[ith*g.vsize + g.voffsets[ioff]];
		}
		const v2f& uv(){
			int ioff = LOG2<(int)VExtra::UV>::r;
			ASS(g.voffsets[ioff]);
			return *(v2f*)&g.vdata[ith*g.vsize + g.voffsets[ioff]];
		}
		const v3f& normal(){
			int ioff = LOG2<(int)VExtra::NORMAL>::r;
			ASS(g.voffsets[ioff]);
			return *(v3f*)&g.vdata[ith*g.vsize + g.voffsets[ioff]];
		}
	private:
		V(const TriMeshD& _g, int _ith) : g(_g), ith(_ith){}
		const TriMeshD& g;
		int ith;
	};

	template<class TRIMESH>
	class Vs{ friend TriMeshD;
	public:
		V<TRIMESH> operator[](const ui32 ith){
			ASS(ith >= 0 && ith < g.numv);
			return V<TRIMESH>(g,ith);
		}
		V<const TRIMESH> operator[](const ui32 ith)const{
			ASS(ith >= 0 && ith < g.numv);
			return V<const TRIMESH>(g,ith);
		}
		bool empty() const{
			return g.numv == 0;
		}
		ui32 size() const{
			return g.numv;
		}
		void resize(ui32 _size){
			g.vdata.resize(g.vsize*_size);
			// not valid or required for union-find
			//if(g.usingVRemap){
			//	g.vremap.resize(_size);
			//	for(uint i=g.numv; i<_size; ++i) g.vremap[i]=i;
			//}
			g.numv = _size;
		}
		V<TRIMESH> front(){
			ASS(g.numv>0);
			return V<TRIMESH>(g,0);
		}
		V<TRIMESH> back(){
			return V<TRIMESH>(g,size()-1);
		}
		void push_back(){
			++g.numv;
			g.vdata.resize(g.numv*g.vsize);
			// not valid or required for union-find
			//if(g.usingVRemap) g.vremap.push_back(g.numv-1);
		}
		void pop_back(){
			ASS(g.numv>0);
			--g.numv;
			g.vdata.resize(g.numv*g.vsize);
			// not valid or required for union-find
			//if(g.usingVRemap) g.vremap.pop_back();
		}
	private:
		Vs(TRIMESH& _g) : g(_g){}
		TRIMESH& g;
	};

public:
	Vs<const TriMeshD> vs() const { return Vs<const TriMeshD>(*this); }
	Vs<      TriMeshD> vs()       { return Vs<      TriMeshD>(*this); }

	V<const TriMeshD> vs(uint ith) const { return V<const TriMeshD>(*this,ith); }
	V<      TriMeshD> vs(uint ith)       { return V<      TriMeshD>(*this,ith); }






	//
	// polygons access
	//
	
private:
	template<class> class Ps;
	template<class> class P;

	template<> class P<TriMeshD>{ friend Ps<TriMeshD>; friend TriMeshD;
	public:
		ab::Vector3<IDX_INT>& idx(){
			return *(ab::Vector3<IDX_INT>*)&g.indices[ith*3];
			//return *(ab::Vector3<IDX_INT>*)&g.idata[ith*g.isize];
		}
		IDX_INT& operator[](int i){
			return g.indices[ith*3 + i];
			//return *(IDX_INT*)&g.idata[ith*g.isize + i*sizeof(idxint)];
		}
		Triangle3<POS_FLOAT> triangle() const {
			return Triangle3<POS_FLOAT>(
				g.vs(g.indices[ith * 3 + 0]).pos(),
				g.vs(g.indices[ith * 3 + 1]).pos(),
				g.vs(g.indices[ith * 3 + 2]).pos()
			);
		}
		template<class O>
		P& operator=(const O& o){
			if (&g == &o.g && ith == o.ith) return *this;

			g.indices[ith*3 + 0] = o.g.indices[o.ith*3 + 0];
			g.indices[ith*3 + 1] = o.g.indices[o.ith*3 + 1];
			g.indices[ith*3 + 2] = o.g.indices[o.ith*3 + 2];

			// MOVE elinks
			if (g.isUsingELinks()){
				ASS(&g == &o.g);

				auto x = g.elinks[ith * 3 + 0];
				auto y = g.elinks[ith * 3 + 1];
				auto z = g.elinks[ith * 3 + 2];
				if (x != IIDX_INT(-1)) g.elinks[x] = IIDX_INT(-1);
				if (y != IIDX_INT(-1)) g.elinks[y] = IIDX_INT(-1);
				if (z != IIDX_INT(-1)) g.elinks[z] = IIDX_INT(-1);

				x = g.elinks[ith * 3 + 0] = o.g.elinks[o.ith * 3 + 0];
				y = g.elinks[ith * 3 + 1] = o.g.elinks[o.ith * 3 + 1];
				z = g.elinks[ith * 3 + 2] = o.g.elinks[o.ith * 3 + 2];

				if (x != IIDX_INT(-1)) g.elinks[x] = ith * 3 + 0;
				if (y != IIDX_INT(-1)) g.elinks[y] = ith * 3 + 1;
				if (z != IIDX_INT(-1)) g.elinks[z] = ith * 3 + 2;

				g.elinks[o.ith * 3 + 0] = IIDX_INT(-1);
				g.elinks[o.ith * 3 + 1] = IIDX_INT(-1);
				g.elinks[o.ith * 3 + 2] = IIDX_INT(-1);
			}

			// copy eflags
			if (g.isUsingEFlags()){
				g.eflags[ith * 3 + 0] = o.g.eflags[o.ith * 3 + 0];
				g.eflags[ith * 3 + 1] = o.g.eflags[o.ith * 3 + 1];
				g.eflags[ith * 3 + 2] = o.g.eflags[o.ith * 3 + 2];
			}

			// copy pflags
			if (g.isUsingPFlags()){
				g.pflags[ith] = o.g.pflags[o.ith];
			}

			return *this;
		}
	private:
		P(TriMeshD& _g, ui32 _ith) : g(_g), ith(_ith){}
		TriMeshD& g;
		ui32 ith;
	};
	
	template<> class P<const TriMeshD>{
		friend Ps<const TriMeshD>; friend Ps<TriMeshD>; friend TriMeshD;
	public:
		const ab::Vector3<IDX_INT>& idx(){
			return *(ab::Vector3<IDX_INT>*)&g.indices[ith*3];
			//return *(ab::Vector3<idxint>*)&g.idata[ith*g.isize];
		}
		const IDX_INT& operator[](int i){
			return g.indices[ith*3 + i];
			//return *(IDX_INT*)&g.idata[ith*g.isize + i*sizeof(idxint)];
		}
		Triangle3<POS_FLOAT> triangle() const {
			return Triangle3<POS_FLOAT>(
				g.vs(g.indices[ith * 3 + 0]).pos(),
				g.vs(g.indices[ith * 3 + 1]).pos(),
				g.vs(g.indices[ith * 3 + 2]).pos()
			);
		}
	private:
		P(const TriMeshD& _g, ui32 _ith) : g(_g), ith(_ith){}
		const TriMeshD& g;
		ui32 ith;
	};

	template<class TRIMESH> class Ps { friend TriMeshD;
	public:
		P<TRIMESH> operator[](const ui32 ith){
			return P<TRIMESH>(g,ith);
		}
		P<const TRIMESH> operator[](const ui32 ith)const{
			return P<const TRIMESH>(g,ith);
		}
		bool empty() const{
			return g.nump == 0;
		}
		ui32 size() const{
			return g.nump;
		}
		void resize(ui32 _size){
			g.indices.resize(_size*3);
			//g.idata.resize(g.isize*_size);
			//g.pdata.resize(g.psize*_size);
			if (g.usingELinks){
				for (uint i = _size; i < g.nump; ++i){
					for (int k = 0; k < 3; ++k){
						auto x = g.elinks[3 * i + k];
						if (x != IIDX_INT(-1)) g.elinks[x] = IIDX_INT(-1);
					}
				}
				g.elinks.resize(_size * 3, IIDX_INT(-1));
			}
			if (g.isUsingPFlags()){
				g.pflags.resize(_size);
			}
			g.nump = _size;
			if (g.usingEFlags) g.eflags.resize(g.nump * 3);
		}
		P<TRIMESH> front(){
			ASS(g.nump>0);
			return P(g,0);
		}
		P<TRIMESH> back(){
			ASS(g.nump>0);
			return P<TRIMESH>(g, g.nump-1);
		}
		void push_back(){
			resize(g.nump + 1);
		}
		void pop_back(){
			ASS(g.nump>0);
			resize(g.nump - 1);
		}
	private:
		Ps(TRIMESH& _g) : g(_g) {}
		TRIMESH& g;
	};
	
public:
	inline Ps<      TriMeshD> ps()       {return Ps<      TriMeshD>(*this);}
	inline Ps<const TriMeshD> ps() const {return Ps<const TriMeshD>(*this);}

	inline P<      TriMeshD> ps(uint ith)       {return P<      TriMeshD>(*this,ith);}
	inline P<const TriMeshD> ps(uint ith) const {return P<const TriMeshD>(*this,ith);}



// raw access
public:
	const void* getVdata()const{ return &vdata[0]; }
	const void* getIdata()const{ return &indices[0]; }
	//const void* getPdata()const{ return &pdata[0]; }
	int sizeofV()const{ return vsize; }
	//int sizeofI()const{ return isize; }
	//int sizeofP()const{ return psize; }
	
	int rgbOffset()const{ return voffsets[LOG2<(int)VExtra::RGB>::r];}
	int alphaOffset()const{ return voffsets[LOG2<(int)VExtra::ALPHA>::r];}
	int uvOffset()const{ return voffsets[LOG2<(int)VExtra::UV>::r];}
	int normalOffset()const{ return voffsets[LOG2<(int)VExtra::NORMAL>::r];}
	int tangentUOffset()const{ return voffsets[LOG2<(int)VExtra::TANGENT_U>::r];}
	int tangentVOffset()const{ return voffsets[LOG2<(int)VExtra::TANGENT_V>::r];}


private:
	ui32 numv = 0;
	std::vector<ui8> vdata;

	ui32 nump = 0;
public: // todo: private?
	std::vector<IDX_INT> indices;
	//std::vector<ui8> pdata;
	
	
	



	// V REMAP
	// map vertices with the same position to the first of them
private:
	bool usingVRemap = false;

public:
	ab::UnionFind<ui32> vremap;
	void updateVRemap(){
		vremap = hashWeldExact(*this).vremap;
		usingVRemap = true;
	}
	void enableVRemap(){
		if(usingVRemap) return;
		vremap = hashWeldExact(*this).vremap;
		usingVRemap = true;
	}
	void disableVRemap(){
		if(!usingVRemap) return;
		usingVRemap = false;
		vremap.clear();
	}
	bool isUsingVRemap() const {
		return usingVRemap;
	}
	
	
	
	
	
	
	// E LINKS
private:
	bool usingELinks = false;
public:
	// link every polygon's edge to adjacent polygon's edge
	std::vector<IIDX_INT> elinks;
//public:
//	ui32 eLink(ui32 i)const{
//		ASS(!elinks.empty()); // first need to call updateELinks()
//		return elinks[i];
//	}
	void updateELinks(){
		elinks = buildELinks(*this);
		usingELinks = true;
	}
	enum class ELinks {};
	void enableELinks(){
		if(usingELinks) return;
		elinks = buildELinks(*this);
		usingELinks = true;
	}
	void disableELinks(){
		usingELinks = false;
		elinks.clear();
	}
	bool isUsingELinks() const {
		return usingELinks;
	}
	






	// E FLAGS
private:
	bool usingEFlags = false;
public:
	std::vector<ui8> eflags;
	void enableEFlags(){
		if (usingEFlags) return;
		eflags.resize(nump*3);
		usingEFlags = true;
	}
	void disableEFlags(){
		if (!usingEFlags) return;
		usingEFlags = false;
		eflags.clear();
	}
	void resetEFlags(){
		eflags.clear();
		eflags.resize(nump);
		usingEFlags = true;
	}
	bool isUsingEFlags() const {
		return usingEFlags;
	}



	// P FLAGS
private:
	bool usingPFlags = false;
public:
	static const ui8 P_DELETE = 1u << 7;
	std::vector<ui8> pflags;
	void enablePFlags(){
		if (usingPFlags) return;
		pflags.resize(nump);
		usingPFlags = true;
	}
	void disablePFlags(){
		if (!usingPFlags) return;
		usingPFlags = false;
		pflags.clear();
	}
	void resetPFlags(){
		pflags.clear();
		pflags.resize(nump);
		usingPFlags = true;
	}
	bool isUsingPFlags()const {
		return usingPFlags;
	}



};









}



