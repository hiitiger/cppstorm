#include "stable.h"
#include "app.h"



Storm::App* Storm::App::self = nullptr;

namespace Storm {

    class AppThread;
    class AppThreadDispatcher : public ThreadDispatcher
    {
    public:
        AppThreadDispatcher()
        : ThreadDispatcher()
        {
        }

        ~AppThreadDispatcher()
        {

        }
        void threadStart(const CoreRunloopSafePtr& runloopSafe)
        {
            __super::threadStart(runloopSafe);
        }
        void threadStopping()
        {
            __super::threadStopping();
        }
        void threadStopped()
        {
            __super::threadStopping();
        }
    };

    class AppThread : public CoreThread
    {
        App* app_;
        std::shared_ptr<AppThreadDispatcher> appDispatcher;
    public:
        AppThread(App* app)
        : app_(app)
        {
            appDispatcher.reset(new AppThreadDispatcher());
            appDispatcher->threadStart(app->runLoop_->getSafePtr());

            threadDispatcher_ = appDispatcher;

            createThreadId_ = app->appThreadId_;
            threadId_ = app->appThreadId_;
            runloop_ = app->runLoop_.get();
            runloopSafe_ = runloop_->getSafePtr();

            start();
        }

        ~AppThread()
        {

        }

        void start()
        {
            running_ = true;
        }

        void started()
        {
            started_ = true;
        }

        void stop()
        {
            stopping_ = true;
            appDispatcher->threadStopping();
        }

        void stopped()
        {
            stopping_ = false;
            running_ = false;
            started_ = false;
            appDispatcher->threadStopped();
        }
    };

    Storm::App* App::instance()
    {
        return self;
    }

    App::App()
        : state_(E_Init)
    {
        appThreadId_ = std::this_thread::get_id();
        self = this;

        runLoop_.reset(new CoreRunloop);
        runLoop_->sysQuitRecivedEvent().add(delegate(&App::onSysQuitRecived, this));

        appThread_.reset(new AppThread(this));
    }

    App::~App()
    {
        self = nullptr;
    }

    void App::post(const Callback0& func)
    {
        appThread_->post(func);
    }

    void App::post(Callback0&& func)
    {
        appThread_->post(std::move(func));
    }

    void App::postDelayed(const Callback0& func, int milliSeconds)
    {
        appThread_->postDelayed(func, milliSeconds);
    }

    void App::postDelayed(Callback0&& func, int milliSeconds)
    {
        appThread_->postDelayed(std::move(func), milliSeconds);
    }

    void App::run()
    {
        state_ = E_Running;
        while (state_ == E_Running)
        {
            if (state_ == E_Running)
                runLoop_->runOnce();
            if (state_ == E_Running)
                onFrame();
            if (state_ == E_Running)
                runLoop_->tryIdleWait();
        }

        onQuit();
        appThread_->stopped();
        state_ = E_Quited;
    }

    void App::quit()
    {
        if (std::this_thread::get_id() != appThreadId_)
        {
            return;
        }

        DAssert(state_ == E_Running);
        state_ = E_Quiting;
        runLoop_->postQuit();
        appThread_->stop();
    }

    std::thread::id App::threadId()
    {
        return appThreadId_;
    }

    ThreadDispatcherPtr App::appDispatcher()
    {
        return appThread_->threadDispatcher();
    }

    void App::onSysQuitRecived(CoreRunloop*)
    {
        appDispatcher()->invokeAsync(&App::quit, this);
    }

    void App::onFrame()
    {

    }

    void App::onQuit()
    {
        WorkerPool::pool()->stop();
        Storm::IO::stopIO();
    }

}
