#include "stable.h"
#include "bookmarkremoteservice.h"
#include "webclient.h"

BookmarkRemoteService::BookmarkRemoteService(const std::string& apiUrl)
    : apiUrl_(apiUrl)
{

}

concurrency::task<std::shared_ptr<BookmarkItem>> BookmarkRemoteService::loadFromRemote()
{
    using namespace concurrency_;
    return _get()
    | pool([this](WebClient::HttpResponse res) {
        if (res.status == 200)
        {
            std::string err;
            auto obj = json11::Json::parse(res.body, err);
			if (!obj.is_empty())
			{
				return this->resolveBookmarks(obj);
			}
        }

        return std::shared_ptr<BookmarkItem>();
    })
    | ui([this](std::shared_ptr<BookmarkItem> item) {
        return item;
    });
}

std::shared_ptr<BookmarkItem> BookmarkRemoteService::constructItem(const std::string& type, const std::string& name, const std::string& uri)
{
    auto item = std::make_shared<BookmarkItem>();
    item->id_ = nextId();
    item->type_ = type;
    item->name_ = name;
    item->uri_ = uri;
    return item;
}

concurrency::task<bool> BookmarkRemoteService::add(const std::shared_ptr<BookmarkItem>& item, std::shared_ptr<BookmarkItem>& parent, int index)
{
    if (!parent || index < 0 || index >(int)parent->children_.size())
    {
        __return false;
    }

    json11::Json postData = {
        { "action", "add" },
        { "item", this->toJson(item) },
        { "index", index },
    };

    auto res = __await _post(postData);
    return res.status == 200;
}

concurrency::task<bool> BookmarkRemoteService::remove(const std::shared_ptr<BookmarkItem>& item)
{
    //check item...

    json11::Json postData = {
        { "action", "remove" },
        { "item", this->toJson(item) },
    };

    auto res = __await _post(postData);
    return res.status == 200;
}

concurrency::task<bool> BookmarkRemoteService::move(const std::shared_ptr<BookmarkItem>& item, const std::shared_ptr<BookmarkItem>& parent, int index)
{
    //check parent...

    json11::Json postData = {
        { "action", "move" },
        { "item", this->toJson(item) },
        { "modify",{
            { "parentId" , std::to_string(parent->id_) },
            { "index", index }
        } }
    };
    auto res = __await _post(postData);
    return res.status == 200;
}

concurrency::task<bool> BookmarkRemoteService::setName(const std::shared_ptr<BookmarkItem>& item, const std::string& name)
{
    //check root...

    json11::Json postData = {
        { "action", "modify.name" },
        { "item", this->toJson(item) },
        { "modify",{
            { "name", name }
        } }
    };
    auto res = __await _post(postData);
    return res.status == 200;
}

concurrency::task<bool> BookmarkRemoteService::setUri(const std::shared_ptr<BookmarkItem>& item, const std::string& uri)
{
    //check type...

    json11::Json postData = {
        { "action", "modify.name" },
        { "item", this->toJson(item) },
        { "modify",{
            { "uri", uri }
        } }
    };
    auto res = __await _post(postData);
    return res.status == 200;
}

std::shared_ptr<BookmarkItem> BookmarkRemoteService::resolveBookmarks(json11::Json obj)
{
    auto rootObj = obj["root"];
    auto root = resolveBookmarkItem(rootObj, nullptr);
    DAssert(root->id_ == 1);
    DAssert(root->name_ == "root");
    return root;
}

std::shared_ptr<BookmarkItem> BookmarkRemoteService::resolveBookmarkItem(json11::Json obj, const std::shared_ptr<BookmarkItem>& parent)
{
    auto item = std::make_shared<BookmarkItem>();
    item->id_ = std::stoll(obj["id"].string_value());
    item->type_ = obj["type"].string_value();
    item->name_ = obj["name"].string_value();
    item->uri_ = obj["uri"].string_value();
    item->parent_ = parent.get();

    resolveBookmarkChildren(obj, item);

    if (parent)
    {
        parent->children_.push_back(item);
    }
    return item;
}

void BookmarkRemoteService::resolveBookmarkChildren(json11::Json obj, const std::shared_ptr<BookmarkItem>& item)
{
    auto children = obj["children"];
    if (children.is_array())
    {
        const auto sz = children.size();
        for (auto i = 0; i != sz; ++i)
        {
            resolveBookmarkItem(children[i], item);
        }
    }
}

void BookmarkRemoteService::setupRoot(const std::shared_ptr<BookmarkItem>& root)
{
    this->nextId_ = 0;
    root_ = root;
    idMap_.clear();

    std::function<void(const std::shared_ptr<BookmarkItem>&)> setupIndex = [&](const std::shared_ptr<BookmarkItem>& item) {
        this->idMap_.insert(std::make_pair(item->id_, item));
        this->nextId_ = std::max(this->nextId_, item->id_);
        for (auto child: item->children_)
        {
            setupIndex(child);
        }
    };

    setupIndex(root_);
}

std::shared_ptr<BookmarkItem> BookmarkRemoteService::rootItem()
{
    return root_;
}

std::shared_ptr<BookmarkItem> BookmarkRemoteService::getItem(std::int64_t id)
{
    auto it = idMap_.find(id);
    if (it != idMap_.end())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

void BookmarkRemoteService::addItem(const std::shared_ptr<BookmarkItem>& item, const std::shared_ptr<BookmarkItem>& parent, int index)
{
    item->parent_ = parent.get();
    parent->children_.insert(parent->children_.begin() + index, item);
    idMap_.insert(std::make_pair(item->id_, item));
}

void BookmarkRemoteService::removeItem(const std::shared_ptr<BookmarkItem>& item)
{
    DAssert(item->parent_);
    auto it = std::find(item->parent_->children_.begin(), item->parent_->children_.end(), item);
    item->parent_->children_.erase(it);
    item->parent_ = nullptr;

    idMap_.erase(idMap_.find(item->id_));
}

json11::Json BookmarkRemoteService::toJson(const std::shared_ptr<BookmarkItem>& item)
{
    json11::Json obj = {
        { "id",  std::to_string(item->id_)},
        { "type", item->type_},
        { "name",  item->name_ },
        { "uri",  item->uri_ },
        { "parentId",  item->parent_ ? std::to_string(item->parent_->id_) : "0"},
    };

    return obj;
}

int64_t BookmarkRemoteService::nextId()
{
    nextId_ += 1;
    return nextId_;
}

concurrency::task<WebClient::HttpResponse> BookmarkRemoteService::_get()
{
    return concurrency_::async([url = this->apiUrl_]{
        WebClient wc;
        return wc.get(url);
    });
}

concurrency::task<WebClient::HttpResponse> BookmarkRemoteService::_post( json11::Json postData)
{
    return concurrency_::async([postData, url = this->apiUrl_] {
        WebClient wc;
        return wc.post(url, postData.dump(), "application/json");
    });
}
