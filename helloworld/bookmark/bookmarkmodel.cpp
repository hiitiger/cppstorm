#include "stable.h"
#include "bookmarkmodel.h"



BookmarkModel::BookmarkModel()
{

}

void BookmarkModel::setup(std::shared_ptr<IBookmarkRemoteService> service, std::shared_ptr<IBookmarkIndex> index)
{
    bmService_ = service;
    bmIndex_ = index;
}

concurrency::task<bool> BookmarkModel::reloadModel()
{
    auto root = __await bmService_->loadFromRemote();

    bool result = !!root;
    if (result)
    {
        bmIndex_->setupRoot(root);
        this->modelChangedEvent()();
    }
    return result;
}

concurrency::task<std::int64_t> BookmarkModel::addItem(const std::string& name, const std::string& uri, std::int64_t parentId, int index)
{
    int64_t id = 0;
    std::shared_ptr<BookmarkItem> item;
    if (uri.empty())
    {
        item = bmService_->constructItem("Folder", name, uri);
    }
    else
    {
        item = bmService_->constructItem("Uri", name, uri);
    }
    auto parent = bmIndex_->getItem(parentId);
    if (index < 0 || index >(int)parent->children_.size())
    {
        __return id;
    }

    if (__await bmService_->add(item, parent, index))
    {
        bmIndex_->addItem(item, parent, index);
        id = item->id_;
        this->itemAddedEvent()(id);
    }

    return id;
}

concurrency::task<bool> BookmarkModel::removeItem(std::int64_t id)
{
    auto item = bmIndex_->getItem(id);
    auto result = __await bmService_->remove(item);
    if (result)
    {
        if (item->type_ == "Folder")
        {
            auto children = item->children_;
            for (auto child: children)
            {
                bmIndex_->removeItem(child);
            }
        }

        bmIndex_->removeItem(item);
        this->itemRemovedEvent()(id);
    }

    return result;
}

concurrency::task<bool> BookmarkModel::moveItem(std::int64_t id, std::int64_t parentId, int index)
{
    auto item = bmIndex_->getItem(id);
    auto parent = bmIndex_->getItem(parentId);

    if (!parent || parent->type_ != "Folder")
    {
        __return false;
    }
    if(index < 0 || index >(int)parent->children_.size())
    {
        __return false;
    }

    //check parent...
    if (item->parent_ == parent.get())
    {
        auto o_index = getChildIndex(parent, item);
        if (o_index < index)
        {
            index -= 1;
        }
    }


    auto result = __await bmService_->move(item, parent, index);
    if (result)
    {
        bmIndex_->removeItem(item);
        bmIndex_->addItem(item, parent, index);

        this->itemModifiedEvent()(id);
    }

    return result;
}

concurrency::task<bool> BookmarkModel::modifyItem(std::int64_t id, std::string field, std::string value)
{
    bool result = false;
    auto item = bmIndex_->getItem(id);
    if (field == "name")
    {
        result = __await bmService_->setName(item, value);
        if (result)
        {
            item->name_ = value;
        }
    }
    else if(field == "uri")
    {
        if (item->type_ == "Uri")
        {
            result = __await bmService_->setUri(bmIndex_->getItem(id), value);
            if (result)
            {
                item->uri_ = value;
            }
        }
    }

    if (result)
    {
        this->itemModifiedEvent()(id);
    }

    return result;
}

std::shared_ptr<BookmarkItem> BookmarkModel::rootItem()
{
    return bmIndex_->rootItem();
}

std::shared_ptr<BookmarkItem> BookmarkModel::getItem(std::int64_t id)
{
    return bmIndex_->getItem(id);
}

std::shared_ptr<BookmarkItem> BookmarkModel::getChildItem(const std::shared_ptr<BookmarkItem>& parent, int index)
{
    if (index < 0 || index >= (int)parent->children_.size())
    {
        return nullptr;
    }
    return parent->children_[index];
}

int BookmarkModel::getChildIndex(const std::shared_ptr<BookmarkItem>& parent, const std::shared_ptr<BookmarkItem>& item)
{
    if (item->parent_ == parent.get())
    {
        auto it = std::find(parent->children_.begin(), parent->children_.end(), item);
        return it - parent->children_.begin();
    }
    else
    {
        return -1;
    }
}

Storm::Event<void()>& BookmarkModel::modelChangedEvent()
{
    return modelChangedEvent_;
}

Storm::Event<void(std::int64_t) > & BookmarkModel::itemAddedEvent()
{
    return itemAddedEvent_;
}

Storm::Event<void(std::int64_t) > & BookmarkModel::itemRemovedEvent()
{
    return itemRemovedEvent_;
}

Storm::Event<void(std::int64_t) > & BookmarkModel::itemModifiedEvent()
{
    return itemModifiedEvent_;
}
