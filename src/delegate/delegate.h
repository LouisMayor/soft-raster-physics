#pragma once

#include <functional>
#include "types.h"
#include <vector>

// todo: replace raw ptrs with unique_ptrs
// todo: support functions with return types
// todo: support delegates with parameters

struct IBaseMethod {
	IBaseMethod() = default;
	virtual ~IBaseMethod() = default;
	virtual void execute_method() = 0;
};

template<typename ObjectClass>
struct FuncPtrType {
	using Type = void(ObjectClass::*)();
};

template<typename ObjectClass>
class RawMethodInstance : public IBaseMethod {
public:
	using MethodPtr = typename FuncPtrType<ObjectClass>::Type;

	RawMethodInstance(ObjectClass*, MethodPtr);
	void execute_method() final;
	static RawMethodInstance* create(ObjectClass*, typename FuncPtrType<ObjectClass>::Type);

	ObjectClass* Object_Instance;
	MethodPtr Method_Ptr;
};

template <typename ReturnType>
class delegate {
public:
	template<typename ObjectClass>
	void bind_callback(ObjectClass*, typename FuncPtrType<ObjectClass>::Type);
	
	template<typename ObjectClass>
	void unbind_callback(ObjectClass*, typename FuncPtrType<ObjectClass>::Type);
	
	virtual ~delegate();
	void execute() const;

private:
	std::vector<IBaseMethod*> MethodInstances;
};

template <typename ObjectClass>
RawMethodInstance<ObjectClass>* RawMethodInstance<ObjectClass>::create(ObjectClass* ObjectInstance, typename FuncPtrType<ObjectClass>::Type Method) {
	return new RawMethodInstance(ObjectInstance, Method);
}

template <typename ObjectClass>
void RawMethodInstance<ObjectClass>::execute_method() {
	std::invoke(Method_Ptr, Object_Instance);
}

template <typename ObjectClass>
RawMethodInstance<ObjectClass>::RawMethodInstance(ObjectClass* ObjectInstance, MethodPtr Method) {
	Object_Instance = ObjectInstance;
	Method_Ptr = Method;
}

template <typename ReturnType>
template <typename ObjectClass>
void delegate<ReturnType>::bind_callback(ObjectClass* ObjectInstance, typename FuncPtrType<ObjectClass>::Type Method) {
	RawMethodInstance<ObjectClass>* ptr = RawMethodInstance<ObjectClass>::create(ObjectInstance, Method);
	MethodInstances.emplace_back(ptr);
}

template <typename ReturnType>
template <typename ObjectClass>
void delegate<ReturnType>::unbind_callback(ObjectClass* ObjectInstance, typename FuncPtrType<ObjectClass>::Type Method) {
	auto it = std::find_if(MethodInstances.begin(), MethodInstances.end(), [&ObjectInstance, &Method](IBaseMethod* method_instance){
		if (auto Instance = static_cast<RawMethodInstance<ObjectClass>*>(method_instance)) {
			return Instance->Object_Instance == ObjectInstance && Instance->Method_Ptr == Method;
		}
		return false;
	});
	
	// remove entry in array and delete the allocation
	if (it != MethodInstances.end()) {
		const RawMethodInstance<ObjectClass>* raw_instance_ptr = static_cast<RawMethodInstance<ObjectClass>*>(*it);
		MethodInstances.erase(it);
		delete raw_instance_ptr;
	}
}

template <typename ReturnType>
delegate<ReturnType>::~delegate() {
	const i32 size = MethodInstances.size() - 1;
	for(i32 i = size; i >= 0; --i) {
		delete MethodInstances[i]; // TODO: not deletable because of unknown size? fix: custom allocator??? 
	}
}

template <typename ReturnType>
void delegate<ReturnType>::execute() const {
	for (IBaseMethod *ptr : MethodInstances) {
		ptr->execute_method();
	}
}

#define DECLARE_DELEGATE(Name, Type)\
	typedef delegate<Type> Name

#define CREATE_DELEGATE(Name)\
	DECLARE_DELEGATE(Name, void)
