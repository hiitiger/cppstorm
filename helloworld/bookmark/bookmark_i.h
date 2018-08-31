#pragma once


struct BookmarkItem
{
    std::int64_t id_;

    std::string type_;
    std::string name_;
    std::string uri_;

    BookmarkItem* parent_;
    std::vector<std::shared_ptr<BookmarkItem>> children_;
};


class IBookmarkRemoteService
{
public:
    virtual ~IBookmarkRemoteService() { ; }

    virtual concurrency::task<std::shared_ptr<BookmarkItem>> loadFromRemote() = 0;

    virtual std::shared_ptr<BookmarkItem> constructItem(const std::string& type, const std::string& name, const std::string& uri) = 0;

    virtual concurrency::task<bool> add(const std::shared_ptr<BookmarkItem>& item, std::shared_ptr<BookmarkItem>& parent, int index) = 0;
    virtual concurrency::task<bool> remove(const std::shared_ptr<BookmarkItem>& item) = 0;
    virtual concurrency::task<bool> move(const std::shared_ptr<BookmarkItem>& item, const std::shared_ptr<BookmarkItem>& parent, int index) = 0;

    virtual concurrency::task<bool> setName(const std::shared_ptr<BookmarkItem>& item, const std::string& name) = 0;
    virtual concurrency::task<bool> setUri(const std::shared_ptr<BookmarkItem>& item, const std::string& uri) = 0;
};


class IBookmarkIndex
{
public:
    virtual ~IBookmarkIndex() { ; }
    virtual void setupRoot(const std::shared_ptr<BookmarkItem>& root) = 0;
    virtual std::shared_ptr<BookmarkItem> rootItem() = 0;
    virtual std::shared_ptr<BookmarkItem> getItem(std::int64_t id) = 0;
    virtual void addItem(const std::shared_ptr<BookmarkItem>& item, const std::shared_ptr<BookmarkItem>& parent, int index) = 0;
    virtual void removeItem(const std::shared_ptr<BookmarkItem>& item) = 0;
};



class IBookmarkModel
{
public:
    virtual ~IBookmarkModel() { ; }

    virtual concurrency::task<bool> reloadModel() = 0;
    virtual concurrency::task<std::int64_t> addItem(const std::string& name, const std::string& uri, std::int64_t parentId, int index) = 0;
    virtual concurrency::task<bool> removeItem(std::int64_t id) = 0;
    virtual concurrency::task<bool> moveItem(std::int64_t id, std::int64_t parentId, int index) = 0;
    virtual concurrency::task<bool> modifyItem(std::int64_t id, std::string field, std::string value) = 0;

    virtual std::shared_ptr<BookmarkItem> rootItem() = 0;
    virtual std::shared_ptr<BookmarkItem> getItem(std::int64_t id) = 0;
    virtual std::shared_ptr<BookmarkItem> getChildItem(const std::shared_ptr<BookmarkItem>& parent, int index) = 0;

    virtual Storm::Event<void()>& modelChangedEvent() = 0;
    virtual Storm::Event<void(std::int64_t)>& itemAddedEvent() = 0;
    virtual Storm::Event<void(std::int64_t)>& itemRemovedEvent() = 0;
    virtual Storm::Event<void(std::int64_t)>& itemModifiedEvent() = 0;
};