#include "stable.h"
#include "extest.h"


INT WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
    (void)hInstance; (void)hPrevInstance; (void)lpCmdLine; (void)nShowCmd;
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    trace::DebugConsole::allocDebugConsole();

    __debug("main") << "starting...";

    std::function<void()> run_example[] = {


       /*  example_refcount,

         example_event,

         example_list,*/

         example_map,

       /*  example_string,

         example_xml,

         example_json,

         example_callback,

        example_thread,

        example_geometry,

        example_time,

        example_weakptr,

        exmaple_lambda,

        example_bind,

        example_waitable,

        example_throttle,

        example_snowflake,

        example_async,
        
        example_fs*/
    };

    Storm::GuiApp app;
    auto guiWindow = std::make_unique<Storm::GuiWindow>();
    guiWindow->show();

    auto w = Ex::waitable(16).async_wait_on(Ex::Scheduler::static_scheduler()).add([&app, &guiWindow](Ex::WaitableRef) {
        app.appDispatcher()->invokeAsync([&]() {
            guiWindow->render();
        });
    });

    Storm::CoreThread testthread;
    testthread.start();

    for (auto e : run_example)
    {
        testthread.post(e);
    }

    Storm::IO::asyncRead(L"G:\\dump\\libcef.dll.pdb", [](Storm::RefPtr<Storm::IO::IOReqeust>& request){
        std::cout << "status: " << request->status_ << ", sz:" << request->data_.size() << std::endl;
    });


    app.run();
    
    w.dispose();
  
    testthread.stop();

    Ex::Scheduler::static_scheduler().stop();

    __debug("main") << "quit...";

    return 0;
}
