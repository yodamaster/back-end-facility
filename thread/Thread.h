#ifndef _THREAD_BASE_H_
#define _THREAD_BASE_H_

#include <pthread.h>
#include <algorithm>

#include "ITask.h"
#include "misc/NonCopyable.h"

/*
 * Naming is a little tricky.
 * for now, ThreadBase is the ideal choice for most use case.
 */

class Thread: public noncopyable
{
    public:
        explicit Thread(ITask*, bool detachable = false);
        virtual ~Thread();

        bool   IsDetachable() const { return detachable_; }
        bool   SetDetachable(bool enable);

        bool   Start();
        bool   Join(void** ret = NULL);
        bool   Cancel();

        virtual bool IsRunning() const;

        // this function is not thread safe, be sure not to call it when the thread is already started.
        ITask* SetTask(ITask* task) { std::swap(task, task_); return task; }
        const ITask* GetTask() const { return task_; }

        pthread_t GetPid() const { return tid_; }

    protected:
        static void* RunTask(void* arg);

    protected:
        ITask* task_;

    private:
        pthread_t tid_;
        volatile mutable int busy_;
        volatile bool detachable_;
};

class ThreadBase: public Thread
{
    public:
        explicit ThreadBase(bool detachable = false);
        ~ThreadBase();

    protected:
        virtual void Run() = 0;

    private:
        using Thread::SetTask;
        using Thread::GetTask;

        class ThreadBaseTask: public ITask
        {
            public:
                explicit ThreadBaseTask(ThreadBase* host) :host_(host) {}

                virtual void Run()
                {
                    host_->Run();
                }

            private:
                ThreadBase* host_;
        };

        ThreadBaseTask task_;
};

#endif

