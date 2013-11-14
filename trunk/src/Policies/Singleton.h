#ifndef SINGLETON_H
#define SINGLETON_H

#include "CreationPolicy.h"
#include "ThreadingModel.h"
#include "ObjectLifeTime.h"

namespace Gibdd
{
    template <typename T, class ThreadingModel = Gibdd::SingleThreaded<T>,
                          class CreatePolicy = Gibdd::OperatorNew<T>,
                          class LifeTimePolicy = Gibdd::ObjectLifeTime<T>>
    class Singleton
    {
        public:
            static T& Instance();

        protected:
            Singleton() {};

        private:
            Singleton(const Singleton &);
            Singleton& operator=(const Singleton &);

            static void DestroySingleton();

            typedef typename ThreadingModel::Lock Guard;
            static T *si_instance;
            static bool si_destroyed;
    };
}

#endif
