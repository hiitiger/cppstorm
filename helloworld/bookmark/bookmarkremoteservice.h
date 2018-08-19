#pragma once
#include "bookmark_i.h"
#include "webclient.h"

class BookmarkRemoteService : public IBookmarkRemoteService, public IBookmarkIndex
{
public:
    BookmarkRemoteService(const std::string& apiUrl);

    virtual concurrency::task<std::shared_ptr<BookmarkItem>> loadFromRemote() ;


    virtual std::shared_ptr<BookmarkItem> constructItem(const std::string& type, const std::string& name, const std::string& uri);

    virtual concurrency::task<bool> add(const std::shared_ptr<BookmarkItem>& item, std::shared_ptr<BookmarkItem>& parent, int index) ;

    virtual concurrency::task<bool> remove(const std::shared_ptr<BookmarkItem>& item) ;
    virtual concurrency::task<bool> move(const std::shared_ptr<BookmarkItem>& item, const std::shared_ptr<BookmarkItem>& parent, int index) ;

    virtual concurrency::task<bool> setName(const std::shared_ptr<BookmarkItem>& item, const std::string& name) ;
    virtual concurrency::task<bool> setUri(const std::shared_ptr<BookmarkItem>& item, const std::string& uri);

    virtual std::shared_ptr<BookmarkItem> getItem(std::int64_t id);


private:
    std::shared_ptr<BookmarkItem> resolveBookmarks(json11::Json obj);
    std::shared_ptr<BookmarkItem> resolveBookmarkItem(json11::Json obj, const std::shared_ptr<BookmarkItem>& parent);
    void resolveBookmarkChildren(json11::Json obj, const std::shared_ptr<BookmarkItem>& item);

    virtual void setupRoot(const std::shared_ptr<BookmarkItem>& root);
    virtual std::shared_ptr<BookmarkItem> rootItem() ;
    virtual void addItem(const std::shared_ptr<BookmarkItem>& item, const std::shared_ptr<BookmarkItem>& parent, int index);
    virtual void removeItem(const std::shared_ptr<BookmarkItem>& item);

    json11::Json toJson(const std::shared_ptr<BookmarkItem>& item);
    int64_t nextId();

protected:
    virtual concurrency::task<WebClient::HttpResponse> _get();
    virtual concurrency::task<WebClient::HttpResponse> _post( json11::Json postData);

private:
    std::string apiUrl_;
    std::shared_ptr<BookmarkItem> root_;
    std::map<std::int64_t, std::shared_ptr<BookmarkItem>> idMap_;
    std::int64_t nextId_ = 0;
};


class DummyBMServer : public BookmarkRemoteService
{
public:
    DummyBMServer(const std::string& apiUrl) : BookmarkRemoteService(apiUrl) { ; }

protected:
    virtual concurrency::task<WebClient::HttpResponse> _get()
    {
        const auto fileName = "bookmarks.json";
        std::ifstream in(fileName);

        std::stringstream buffer;
        buffer << in.rdbuf();

        std::string contents(buffer.str());

        WebClient::HttpResponse res = { true, 200, contents };
        __return res;
    }
    virtual concurrency::task<WebClient::HttpResponse> _post(json11::Json postData)
    {
        WebClient::HttpResponse res = { true, 200, "" };
        __return res;
    }

};