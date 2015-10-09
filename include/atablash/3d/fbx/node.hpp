// DON'T USE THIS
// USE AUTODESK FBX SDK!!!

#pragma once

#include <atablash/debug.hpp>
#include <atablash/zlib/zlib.hpp>

#include <cstdint>
#include <unordered_map>



namespace ab{







template<typename T>
bool CheckArrayType(const uint8_t c){
	ASS(false);
	return false;
}
template<> bool CheckArrayType<float>(const uint8_t c){	return c == 'f';}
template<> bool CheckArrayType<double>(const uint8_t c){ return c == 'd';}
template<> bool CheckArrayType<long long>(const uint8_t c){	return c == 'l';}
template<> bool CheckArrayType<int>(const uint8_t c){ return c == 'i';}
template<> bool CheckArrayType<bool>(const uint8_t c){ return c == 'b';}








template<typename T>
bool CheckType(const uint8_t c){
	ASS(false);
	return false;
}
template<> bool CheckType<float>(const uint8_t c){ return c == 'F'; }
template<> bool CheckType<double>(const uint8_t c){	return c == 'D'; }
template<> bool CheckType<int64_t>(const uint8_t c){ return c == 'L'; }
template<> bool CheckType<int32_t>(const uint8_t c){ return c == 'I'; }
template<> bool CheckType<bool>(const uint8_t c){ return c == 'C'; }
template<> bool CheckType<int16_t>(const uint8_t c){ return c == 'Y'; }








class FBXNode
{
public:
	FBXNode() : propsIndices(NULL), props(NULL) {}
	~FBXNode();

	static const int PROP_TYPE_UNKNOWN = 0;
	static const int PROP_TYPE_PRIMITIVE = 1;
	static const int PROP_TYPE_ARRAY = 2;
	static const int PROP_TYPE_SPECIAL = 3;

	static int PropType(unsigned char type);

	static unsigned int PropTypeSize(unsigned char type);	// in bytes

	unsigned int GetPropSize(int pos);	// including type byte

	bool ParseProps();

	FBXNode* subnode(const char *_name){
		// TODO: should not be linear!
		for (unsigned int i = 0; i < subnodes.size(); ++i)
			if (subnodes[i]->name == _name) return subnodes[i];

		return NULL;
	}



	class Prop
	{
	public:
		Prop(FBXNode *_fbx, unsigned int _offset) : fbx(_fbx), offset(_offset) {}


		template<typename T>
		T Get(unsigned int arrayIndex)
		{
			ASS(PropType(fbx->props[offset]) == PROP_TYPE_ARRAY);

			bool typeok = CheckArrayType<T>(fbx->props[offset]);
			ASS(typeok);

			uint32_t arrayLength = *(uint32_t*)&fbx->props[offset + 1];
			uint32_t encoding = *(uint32_t*)&fbx->props[offset + 5];

			ASS(arrayLength > arrayIndex);

			if (encoding)
			{
				auto r = fbx->decompressedProps.insert(make_pair(offset,std::vector<uint8_t>()));
				if (r.second)
				{
					uint32_t compressedLength = *(uint32_t*)&fbx->props[offset + 9];
					r.first->second.resize(PropTypeSize(fbx->props[offset]) * arrayLength);

					Inflate(r.first->second.data(), r.first->second.size(), &fbx->props[offset+13], compressedLength);
				}
				return *(T*)(&r.first->second[0] + sizeof(T)*arrayIndex);
			}
			else
			{
				//ASS(encoding == 0);
				return *(T*)(fbx->props + offset + 13 + sizeof(T)*arrayIndex);
			}
		}

		unsigned int ArraySize();

		template<typename T>
		T Get()
		{
			ASS(PropType(fbx->props[offset]) == PROP_TYPE_PRIMITIVE);

			bool typeok = CheckType<T>(fbx->props[offset]);
			ASS(typeok);

			return *(T*)(fbx->props + offset + 1);
		}



	private:
		FBXNode *fbx;
		unsigned int offset;
	};

	Prop Property(int idx)
	{
		return Prop(this, propsIndices[idx]);
	}

	uint32_t numProps;
	uint32_t *propsIndices;		// stores offsets of where props start in the `props` stream

	uint32_t propsSize;
	uint8_t *props;

	std::unordered_map<unsigned int, std::vector<uint8_t>> decompressedProps;

	std::vector<FBXNode*> subnodes;

	std::string name;
};

template<>
inline std::string FBXNode::Prop::Get<std::string>()
{
	ASS(fbx->props[offset] == 'S');

	std::string result;
	uint32_t sz = *(uint32_t*)(fbx->props + offset + 1);
	result.resize(sz);
	memcpy(&result[0], &fbx->props[offset + 5], sz);
	return result;
}









// implementation





FBXNode::~FBXNode(){
	if (propsIndices) delete[] propsIndices;
	if (props) delete[] props;

	for (unsigned int i = 0; i < subnodes.size(); ++i){
		delete subnodes[i];
	}
}

int FBXNode::PropType(unsigned char type){
	static unsigned char known[256];
	static bool done = false;

	if (!done)
	{
		done = true;
		memset(known, 0, 256);
		known['Y'] = PROP_TYPE_PRIMITIVE;
		known['C'] = PROP_TYPE_PRIMITIVE;
		known['I'] = PROP_TYPE_PRIMITIVE;
		known['F'] = PROP_TYPE_PRIMITIVE;
		known['D'] = PROP_TYPE_PRIMITIVE;
		known['L'] = PROP_TYPE_PRIMITIVE;
		known['f'] = PROP_TYPE_ARRAY;
		known['d'] = PROP_TYPE_ARRAY;
		known['l'] = PROP_TYPE_ARRAY;
		known['i'] = PROP_TYPE_ARRAY;
		known['b'] = PROP_TYPE_ARRAY;
		known['S'] = PROP_TYPE_SPECIAL;
		known['R'] = PROP_TYPE_SPECIAL;
	}

	return known[type];
}


// in bytes
unsigned int FBXNode::PropTypeSize(unsigned char type){
	static uint8_t known[256];
	static bool done = false;

	if (!done)
	{
		done = true;
		memset(known, 0, 256);
		known['Y'] = 2;
		known['C'] = 1;
		known['I'] = 4;
		known['F'] = 4;
		known['D'] = 8;
		known['L'] = 8;
		known['f'] = 4;	// array!
		known['d'] = 8;	// array!
		known['l'] = 8;	// array!
		known['i'] = 4;	// array!
		known['b'] = 1;	// array!
	}

	return known[type];
}

unsigned int FBXNode::GetPropSize(int pos)	// including type byte
{
	unsigned char type = props[pos++];
	int propType = PropType(type);
	if (propType == PROP_TYPE_UNKNOWN)
	{
		MessageLog(std::wstring(L"FBX Node Property Type ") + (wchar_t)type + L" (" + L(type) + L") unknown! Node name: " + L(name),
			MessageLevel::WARNING, MessageType::UNDEFINED);
		delete[] propsIndices;
		propsIndices = NULL;
		return 0;
	}
	else if (propType == PROP_TYPE_PRIMITIVE)
	{
		return PropTypeSize(type) + 1;
	}
	else if (propType == PROP_TYPE_ARRAY)
	{
		uint32_t arraylen = *(uint32_t*)&props[pos];
		pos += 4;
		uint32_t encoding = *(uint32_t*)&props[pos];
		pos += 4;
		uint32_t compressedLen = *(uint32_t*)&props[pos];
		pos += 4;

		int complen = 0;

		if (encoding == 0)
		{
			complen = arraylen*PropTypeSize(type);

			if (compressedLen != complen)
			{
				wchar_t mbuff[256];
				swprintf(mbuff, 256, L"FBX Node Property Array length invalid! %u vs %u.", compressedLen, complen);
				MessageLog(mbuff, MessageLevel::WARNING, MessageType::UNDEFINED);
			}
		}
		else complen = compressedLen;

		return complen + 4 + 4 + 4 + 1;
	}
	else if (propType == PROP_TYPE_SPECIAL)
	{
		return 1 + 4 + props[pos];
	}
	else throw "bug";

	//return 0;
}

bool FBXNode::ParseProps()
{
	if (propsIndices) delete[] propsIndices;

	propsIndices = new uint32_t[numProps];

	unsigned int cpos = 0;
	unsigned int cprop = 0;
	while (cpos < propsSize && cprop < numProps)
	{
		propsIndices[cprop++] = cpos;

		int sz = GetPropSize(cpos);
		if (sz == 0) return false;

		cpos += sz;
	}

	if (cpos != propsSize || cprop != numProps)
	{
		wchar_t mbuff[256];
		swprintf(mbuff, 256, L"FBX Node property list size mismatch!");
		MessageLog(mbuff, MessageLevel::WARNING, MessageType::UNDEFINED);
	}

	return true;
}





unsigned int FBXNode::Prop::ArraySize()
{
	ASS(PropType(fbx->props[offset]) == PROP_TYPE_ARRAY);

	uint32_t arrayLength = *(uint32_t*)&fbx->props[offset + 1];
	return arrayLength;
}








