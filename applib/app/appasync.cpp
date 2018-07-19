#include "stable.h"
#include "appasync.h"
#include "app.h"

namespace AppUI
{

    struct AppUITaskpplSchedule : public concurrency::scheduler_interface
    {
        virtual void schedule(Concurrency::TaskProc_t proc,  void* param)
        {
            AppUI::async([proc, param]() {
                proc(param);
            });
        }
    };

    void async(const Callback0& cb)
    {
        Storm::App::instance()->post(cb);
    }

    void async(Callback0&& cb)
    {
        Storm::App::instance()->post(std::move(cb));
    }

    void asyncDelayed(const Callback0& cb, int milliSecs)
    {
        Storm::App::instance()->postDelayed(cb, milliSecs);
    }

    void asyncDelayed(Callback0&& cb, int milliSecs)
    {
        Storm::App::instance()->postDelayed(std::move(cb), milliSecs);
    }

    std::shared_ptr<concurrency::scheduler_interface> static_pplScheduler()
    {
        static std::shared_ptr<concurrency::scheduler_interface>  s = std::make_shared<AppUITaskpplSchedule>();
        return s;
    }

    std::thread::id threadId()
    {
        return Storm::App::instance()->threadId();
    }

}


namespace concurrency_ 
{
    struct WorkerPoolTaskpplSchedule : public concurrency::scheduler_interface
    {
        virtual void schedule(concurrency::TaskProc_t proc, void* param)
        {
            Storm::WorkerPool::async([proc, param]() {
                proc(param);
            });
        }
    };


    std::shared_ptr<concurrency::scheduler_interface>& static_pplScheduler()
    {
        static std::shared_ptr<concurrency::scheduler_interface>  s = std::make_shared<WorkerPoolTaskpplSchedule>();
        return s;
    }
}
