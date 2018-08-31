#pragma once

// external - atablash
#include <atablash/types.hpp>
#include <atablash/debug.hpp>

// external - std
#include <unordered_set>

namespace ab{





// entity, component, system
class Entity;
class ComponentBase;
class System;




class Entity {    friend ComponentBase;
public:
	template<class T> T& get(System* system);
	template<class T> T* tryget();

	void onUpdate();
	
protected:
	Entity() = default;

	// update components' entity pointers
	Entity(Entity&& o);
	
	// duplicate components
	Entity(const Entity& o);
	Entity& operator=(const Entity&);

private:
	umap<ui32, uptr<ComponentBase>> components;
};




// created and owned by Entity
//
// ComponentBase is for polymorphism. Component<CRTP> derives from it and implements clone()
//
class ComponentBase {    friend Entity;
protected:
	ComponentBase(){}

public:
	virtual ~ComponentBase();

// onUpdate required?
public:
	virtual void onUpdate(){}

public:
	Entity& entity(){ return *_entity; }
	System& system(){ return *_system; }

	// remove component from entity
	void remove(){
		ASS(_entity);
		ASS(typeID);
		_entity->components.erase(typeID);
	}
	
	ComponentBase(ComponentBase&& o) = delete;
	
	virtual ComponentBase* clone() const = 0;
	
protected:
	// add new component to system
	ComponentBase(const ComponentBase& o);

// shortcut - not using shortcut to allow components to be entities (tree structure)
/*
public:
	template<class T> T& get(){
		ASS(_entity);
		return _entity->get<T>();
	}
*/

private:
	Entity* _entity = nullptr;		// can't be assigned during creation because
									// derived classes would have to call constructor explicitly
	System* _system = nullptr;
	ui32 typeID = 0;
};

template<class CRTP>
class Component : public ComponentBase {
public:
	ComponentBase* clone() const override {
		return new CRTP(*(const CRTP*)this);
	}
};





class System : Static { friend Entity; friend ComponentBase;
protected:
	System(){}
	System(const System&) = delete;
	System& operator=(const System&) = delete;

	virtual ~System(){
		removeComponents();
	}

protected:
	// called on destruct
	// can also be called by derived system to free components before e.g. releasing OpenGL context
	void removeComponents(){
		while (!components.empty()){
			(*components.begin())->remove();
		}
	}

private:
	// pointers to components owned by entities
	std::unordered_set<ComponentBase*> components;

	// Components removed from entities, waiting for destruction.
	// std::moved here from entities.
	// Components are not immediately destroyed, because they might have to be destroyed in another thread
	// and it's better to do this async.
	//std::vector<uptr<Component>>   deadComponents;
};





// IMPLEMENTATION



	
// update components' entity pointers
inline Entity::Entity(Entity&& o){
	LOGW("Performance warning: Entity(Entity&&)");
	components = std::move(o.components);
	for(auto& e : components) e.second->_entity = this;
}
	
// duplicate components
inline Entity::Entity(const Entity& o){
	LOGW("Performance warning: Entity(Entity&)");
	components.reserve(o.components.size()); // TODO: reserve? shouldn't be sometging different on umaps?
	for(auto& e :  o.components){
		components[e.first] = uptr<ComponentBase>(e.second->clone());
		components[e.first]->_entity = this;
	}
}

// duplicate components
inline Entity& Entity::operator=(const Entity& o){
	LOGW("Performance warning: Entity::operator=(Entity&)");
	components.clear();
	components.reserve(o.components.size()); // TODO: reserve? shouldn't be sometging different on umaps?
	for(auto& e :  o.components){
		components[e.first] = uptr<ComponentBase>(e.second->clone());
		components[e.first]->_entity = this;
	}
	return *this;
}



inline void Entity::onUpdate(){
	for (auto& component : components) component.second->onUpdate();
}


template<class T> inline T& Entity::get(System* system){
	auto typeID = (ui32)typeid(T).hash_code();    // czy rzutowanie do ui32 nie psuje?
	auto& p = components[typeID];

	// this is the only place, where T is created
	if(!p){
		p = std::make_unique<T>();
		p->_entity = this;
		p->typeID = typeID;
		p->_system = system;
		if(system)system->components.insert(p.get());
		((T*)p.get())->onUpdate();
	}

	ASS(typeid(T) == typeid(*p));

	return (T&)*p;
}


template<class T> inline T* Entity::tryget(){
	auto typeID = (ui32)typeid(T).hash_code();    // czy rzutowanie do ui32 nie psuje?
	auto& itr = components.find(typeID);
	if (itr == components.end()) return nullptr;

	ASS(typeid(T) == typeid(*itr->second));

	return (T*)itr->second.get();
}



inline ComponentBase::~ComponentBase(){
	if(_system) _system->components.erase(this);
}
	
// add new component to system
inline ComponentBase::ComponentBase(const ComponentBase& o){
	LOGW("Performance warning: ComponentBase(ComponentBase& o)");
	//_entity = o._entity; // owner (entity) should set this pointer
	_system = o._system;
	if(_system) _system->components.insert(this);
}





}


