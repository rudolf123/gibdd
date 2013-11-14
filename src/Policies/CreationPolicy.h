#ifndef CREATIONPOLICY_H
#define CREATIONPOLICY_H

#include <stdlib.h>

namespace Gibdd
{
    template <class T> class OperatorNew
    {
        public:
            static T* Create(void) { return (new T); }
            static void Destroy(T *obj) { delete obj; }
    };

    template <class T> class LocalStaticCreation
    {
        union MaxAlign
        {
            char t_[sizeof(T)];
            short int shortInt_;
            int int_;
            long int longInt_;
            float float_;
            double double_;
            long double longDouble_;
            struct Test;
            int Test::* pMember_;
            int (Test::*pMemberFn_)(int);
        };
        public:
            static T* Create(void)
            {
                static MaxAlign si_localStatic;
                return new(&si_localStatic) T;
            }

            static void Destroy(T *obj) { obj->~T(); }
    };

    template<class T> class CreateUsingMalloc
    {
        public:
            static T* Create()
            {
                void* p = ::malloc(sizeof(T));
                if (!p) return 0;
                return new(p) T;
            }

            static void Destroy(T* p)
            {
                p->~T();
                ::free(p);
            }
    };

    template<class T, class CALL_BACK> class CreateOnCallBack
    {
        public:
            static T* Create()
            {
                return CALL_BACK::createCallBack();
            }

            static void Destroy(T *p)
            {
                CALL_BACK::destroyCallBack(p);
            }
    };
}

#endif
