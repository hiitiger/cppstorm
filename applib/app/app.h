#pragma once

namespace Storm
{
    class App 
    {
    public:
        enum State
        {
            E_Init,
            E_Running,
            E_Quiting,
            E_Quited,
        };

        App();
        virtual ~App();
        static App* instance();

        void post(const Callback0& func);
        void post(Callback0&& func);
 
        void postDelayed(const Callback0& func, int milliSeconds);
        void postDelayed(Callback0&&, int milliSeconds);

        void run();
        virtual void quit();

        std::thread::id threadId();
        ThreadDispatcherPtr appDispatcher();

    protected:
        virtual void runLoop();
        virtual void onSysQuitRecived(CoreRunloop*);
        virtual void onFrame();
        virtual void onQuit();

    protected:
        static App* self;
        std::thread::id appThreadId_;
        State state_;
        std::unique_ptr<CoreRunloop> runLoop_;

    private:
        friend class AppThread;
        std::unique_ptr<AppThread> appThread_;
    };
}
