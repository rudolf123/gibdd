#ifndef OBJECTLIFETIME_H
#define OBJECTLIFETIME_H

#include <stdexcept>

typedef void (* Destroyer)(void);

namespace Gibdd
{
    void at_exit( void (*func)() );

    template <class T> class ObjectLifeTime
    {
        public:
            inline static void ScheduleCall(void (*destroyer)() )
            {
                at_exit( destroyer );
            }
            static void OnDeadReference(void);
    };

    template <class T> inline void ObjectLifeTime<T>::OnDeadReference(void)
    {
        throw std::runtime_error("Dead Reference");
    }
}

#endif
