#ifndef THREADINGMODEL_H
#define THREADINGMODEL_H

namespace Gibdd
{
    inline void Guard(void *) {}

    template<typename MUTEX> class GeneralLock
    {
        public:
            GeneralLock(MUTEX &m) : i_mutex(m)
            {
                i_mutex.acquire();
            }

            ~GeneralLock()
            {
                i_mutex.release();
            }
        private:
            GeneralLock(const GeneralLock &);
            GeneralLock& operator=(const GeneralLock &);
            MUTEX &i_mutex;
    };

    template <class T> class SingleThreaded
    {
        public:

            struct Lock
            {
                Lock() {}
                Lock(const T &) {}
                Lock(const SingleThreaded<T> &)
                {
                }
            };

            typedef T VolatileType;
    };

    template<class T, class MUTEX> class ObjectLevelLockable
    {
        public:
            ObjectLevelLockable() : i_mtx() {}

            friend class Lock;

            class Lock
            {
                public:
                    Lock(ObjectLevelLockable<T, MUTEX> &host) : i_lock(host.i_mtx)
                    {
                    }

                private:
                    GeneralLock<MUTEX> i_lock;
            };

            typedef volatile T VolatileType;

        private:
            ObjectLevelLockable(const ObjectLevelLockable<T, MUTEX> &);
            ObjectLevelLockable<T, MUTEX>& operator=(const ObjectLevelLockable<T, MUTEX> &);

            MUTEX i_mtx;
    };

    template<class T, class MUTEX> class ClassLevelLockable
    {
        public:
            class Lock;
            friend class Lock;
            typedef volatile T VolatileType;

            ClassLevelLockable() {}

            class Lock
            {
                public:
                    Lock(T& /*host*/) { ClassLevelLockable<T, MUTEX>::si_mtx.acquire(); }
                    Lock(ClassLevelLockable<T, MUTEX> &) { ClassLevelLockable<T, MUTEX>::si_mtx.acquire(); }
                    Lock() { ClassLevelLockable<T, MUTEX>::si_mtx.acquire(); }
                    ~Lock() { ClassLevelLockable<T, MUTEX>::si_mtx.release(); }
            };

        private:
            static MUTEX si_mtx;
    };

}

template<class T, class MUTEX> MUTEX Gibdd::ClassLevelLockable<T, MUTEX>::si_mtx;

#define INSTANTIATE_CLASS_MUTEX(CTYPE,MUTEX) \
    template class Gibdd::ClassLevelLockable<CTYPE, MUTEX >

#endif
