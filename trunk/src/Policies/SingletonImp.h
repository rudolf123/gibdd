#ifndef SINGLETONIMPL_H
#define SINGLETONIMPL_H

#include "Singleton.h"

template<typename T, class ThreadingModel, class CreatePolicy, class LifeTimePolicy>
T& Gibdd::Singleton<T, ThreadingModel, CreatePolicy, LifeTimePolicy >::Instance()
{
    if( !si_instance )
    {
        Guard();
        if( !si_instance )
        {
            if( si_destroyed )
            {
                si_destroyed = false;
                LifeTimePolicy::OnDeadReference();
            }
            si_instance = CreatePolicy::Create();
            LifeTimePolicy::ScheduleCall(&DestroySingleton);
        }
    }

    return *si_instance;
}

template<typename T, class ThreadingModel, class CreatePolicy, class LifeTimePolicy>
void Gibdd::Singleton<T, ThreadingModel, CreatePolicy, LifeTimePolicy>::DestroySingleton()
{
    CreatePolicy::Destroy(si_instance);
    si_instance = NULL;
    si_destroyed = true;
}

#define INSTANTIATE_SINGLETON_1(TYPE) \
    template class Gibdd::Singleton<TYPE, Gibdd::SingleThreaded<TYPE>, Gibdd::OperatorNew<TYPE>, Gibdd::ObjectLifeTime<TYPE> >; \
    template<> TYPE* Gibdd::Singleton<TYPE, Gibdd::SingleThreaded<TYPE>, Gibdd::OperatorNew<TYPE>, Gibdd::ObjectLifeTime<TYPE> >::si_instance = 0; \
    template<> bool Gibdd::Singleton<TYPE, Gibdd::SingleThreaded<TYPE>, Gibdd::OperatorNew<TYPE>, Gibdd::ObjectLifeTime<TYPE> >::si_destroyed = false

#define INSTANTIATE_SINGLETON_2(TYPE, THREADINGMODEL) \
    template class Gibdd::Singleton<TYPE, THREADINGMODEL, Gibdd::OperatorNew<TYPE>, Gibdd::ObjectLifeTime<TYPE> >; \
    template<> TYPE* Gibdd::Singleton<TYPE, THREADINGMODEL, Gibdd::OperatorNew<TYPE>, Gibdd::ObjectLifeTime<TYPE> >::si_instance = 0; \
    template<> bool Gibdd::Singleton<TYPE, THREADINGMODEL, Gibdd::OperatorNew<TYPE>, Gibdd::ObjectLifeTime<TYPE> >::si_destroyed = false

#define INSTANTIATE_SINGLETON_3(TYPE, THREADINGMODEL, CREATIONPOLICY ) \
    template class Gibdd::Singleton<TYPE, THREADINGMODEL, CREATIONPOLICY, Gibdd::ObjectLifeTime<TYPE> >; \
    template<> TYPE* Gibdd::Singleton<TYPE, THREADINGMODEL, CREATIONPOLICY, Gibdd::ObjectLifeTime<TYPE> >::si_instance = 0; \
    template<> bool Gibdd::Singleton<TYPE, THREADINGMODEL, CREATIONPOLICY, Gibdd::ObjectLifeType<TYPE> >::si_destroyed = false

#define INSTANTIATE_SINGLETON_4(TYPE, THREADINGMODEL, CREATIONPOLICY, OBJECTLIFETIME) \
    template class Gibdd::Singleton<TYPE, THREADINGMODEL, CREATIONPOLICY, OBJECTLIFETIME >; \
    template<> TYPE* Gibdd::Singleton<TYPE, THREADINGMODEL, CREATIONPOLICY, OBJECTLIFETIME >::si_instance = 0; \
    template<> bool Gibdd::Singleton<TYPE, THREADINGMODEL, CREATIONPOLICY, OBJECTLIFETIME >::si_destroyed = false

#endif
