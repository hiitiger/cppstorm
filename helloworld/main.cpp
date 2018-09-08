#include "stable.h"
#include "bookmark/bookmarkremoteservice.h"
#include "bookmark/bookmarkmodel.h"
#include "extest.h"
#include "json2.h"

std::shared_ptr<IBookmarkModel> createDummyModel()
{
    auto bmS = std::make_shared<DummyBMServer>("http://localhost:8000/bookmarks");

    auto bmModel = std::make_shared<BookmarkModel>();
    bmModel->setup(bmS, bmS);

    return bmModel;
}

concurrency::task<void> bookmark_run()
{
    std::function<nlohmann::json(std::shared_ptr<BookmarkItem>)> toJson = [&](auto item)
    {
        std::vector<nlohmann::json> children;
        for (auto child: item->children_)
        {
            children.push_back(toJson(child));
        }

        std::map<std::string, nlohmann::json> obj = {
            { "id",  std::to_string(item->id_) },
            { "type", item->type_ },
            { "name",  item->name_ },
            { "uri",  item->uri_ },
            { "parentId", item->parent_ ? std::to_string(item->parent_->id_) : "0" },
            
        };

        if (children.size() > 0)
        {
            obj.insert(std::make_pair("children", nlohmann::json{ children }));
        }
        return obj;
    };

    auto printItem = [=](auto item)
    {
        nlohmann::json obj = toJson(item);

        std::cout << obj.dump() << std::endl;
    };


    auto bmModel = createDummyModel();

    auto res = __await bmModel->reloadModel();
    if (!res) { return; }

    auto rootItem = bmModel->rootItem();
    printItem(rootItem);

    auto l1Folder = __await bmModel->addItem("L1", "", rootItem->id_, 0);
    __await bmModel->modifyItem(l1Folder, "name", "L1new");
    auto l2Folder = __await  bmModel->addItem("L2", "", rootItem->id_, 1);
    __await  bmModel->addItem("u1", "xx://123", rootItem->id_, 1);
    auto u11 = __await  bmModel->addItem("u11", "xx://1234", l1Folder, 0);
    auto u12 = __await  bmModel->addItem("u12", "xx://12345", l1Folder, 0);
    __await  bmModel->moveItem(u11, l2Folder, 0);

    printItem(rootItem);

    __await bmModel->removeItem(l1Folder);
    printItem(rootItem);

}



INT WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
    (void)hInstance; (void)hPrevInstance; (void)lpCmdLine; (void)nShowCmd;

    trace::DebugConsole::allocDebugConsole();
    trace::DebugConsole::setConsoleTitle("xxxFx");

    __debug("main") << "starting...";


   // example_event();

    Storm::App app;

    bookmark_run();

    DbgWarn << "Quit after 3secs";

    //AppUI::asyncDelayed([]() {

    //trace::DebugConsole::releaseDebugConsole();

    //PostQuitMessage(0);
    //}, 3000);

    func();

    app.run();

    __debug("main") << "quit...";

    return 0;
}
