#pragma once
#include "bookmark_i.h"


struct IACommand
{
    virtual void undo() { ; }
    virtual void redo() { ; }
};


class BookmarkModel : public IBookmarkModel
{
public:
    BookmarkModel();

    virtual void setup(std::shared_ptr<IBookmarkRemoteService> service, std::shared_ptr<IBookmarkIndex> index) ;

    virtual concurrency::task<bool> reloadModel() ;
    virtual concurrency::task<std::int64_t> addItem(const std::string& name, const std::string& uri, std::int64_t parentId, int index) ;
    virtual concurrency::task<bool> removeItem(std::int64_t id) ;
    virtual concurrency::task<bool> moveItem(std::int64_t id, std::int64_t parentId, int index) ;
    virtual concurrency::task<bool> modifyItem(std::int64_t id, std::string field, std::string value);

    virtual std::shared_ptr<BookmarkItem> rootItem() ;
    virtual std::shared_ptr<BookmarkItem> getItem(std::int64_t id);
    virtual std::shared_ptr<BookmarkItem> getChildItem(const std::shared_ptr<BookmarkItem>& parent, int index);
    virtual int getChildIndex(const std::shared_ptr<BookmarkItem>& parent, const std::shared_ptr<BookmarkItem>& item);

    virtual Storm::Event<void()>& modelChangedEvent() ;
    virtual Storm::Event<void(std::int64_t)>& itemAddedEvent();
    virtual Storm::Event<void(std::int64_t)>& itemRemovedEvent() ;
    virtual Storm::Event<void(std::int64_t)>& itemModifiedEvent() ;

private:
    std::shared_ptr<IBookmarkRemoteService> bmService_;
    std::shared_ptr<IBookmarkIndex> bmIndex_;

    Storm::Event<void()> modelChangedEvent_;
    Storm::Event<void(std::int64_t)> itemAddedEvent_;
    Storm::Event<void(std::int64_t)> itemRemovedEvent_;
    Storm::Event<void(std::int64_t)> itemModifiedEvent_;


};

struct BookmarkRemoveCommand : public IACommand
{
    std::shared_ptr<BookmarkModel> model_;
    std::shared_ptr<IBookmarkIndex> bmIndex_;
    std::shared_ptr<BookmarkItem> parent_;
    std::shared_ptr<BookmarkItem> item_;
    int index_;

    BookmarkRemoveCommand(std::shared_ptr<BookmarkModel> model, std::shared_ptr<IBookmarkIndex> bmIndex,  std::shared_ptr<BookmarkItem> parent, int index)
        : model_(model_), bmIndex_(bmIndex_), parent_(parent), index_(index)
    {
        item_ = model_->getChildItem(parent_, index_);
    }

    virtual void undo()
    {
        bmIndex_->addItem(item_, parent_, index_);
    }

    virtual void redo()
    {
        bmIndex_->removeItem(item_);
    }
};

struct BookmarkAddCommand : public BookmarkRemoveCommand
{
    BookmarkAddCommand(std::shared_ptr<BookmarkModel> model, std::shared_ptr<IBookmarkIndex> bmIndex, std::shared_ptr<BookmarkItem> item, std::shared_ptr<BookmarkItem> parent, int index)
        : BookmarkRemoveCommand(model, bmIndex, parent, index)
    {
        item_ = item;
    }

    virtual void undo()
    { 
        __super::redo();
    }
    virtual void redo()
    {
        __super::undo();
    }
};

struct BookmarkMoveCommand : public IACommand
{
    std::shared_ptr<BookmarkModel> model_;
    std::shared_ptr<IBookmarkIndex> bmIndex_;
    std::shared_ptr<BookmarkItem> item_;
    std::shared_ptr<BookmarkItem> parent_;
    int index_;
    std::shared_ptr<BookmarkItem> parentOrgin_;
    int indexOrigin_;

    BookmarkMoveCommand(std::shared_ptr<BookmarkModel> model, std::shared_ptr<IBookmarkIndex> bmIndex, const std::shared_ptr<BookmarkItem>& item, std::shared_ptr<BookmarkItem> parent, int index)
        : model_(model_), bmIndex_(bmIndex_), item_(item), parent_(parent), index_(index)
    {
        parentOrgin_ = model_->getItem(item_->parent_->id_);
        indexOrigin_ = model_->getChildIndex(parentOrgin_, item);
    }


    virtual void undo()
    {
        bmIndex_->removeItem(item_);
        bmIndex_->addItem(item_, parentOrgin_, indexOrigin_);
    }
    virtual void redo()
    {
        bmIndex_->removeItem(item_);
        bmIndex_->addItem(item_, parent_, index_);
    }
};