#pragma once

#include "../corelib/macros.h"
#include "../corelib/basic/basic.h"

#include "../corelib/class.h"

#include "../corelib/basic/timetick.h"
#include "../corelib/basic/datetime.h"

#include "../corelib/trace/dbg.h"
#include "../corelib/trace/dbgconsole.h"
#include "../corelib/trace/perftimer.h"
#include "../corelib/trace/trace.h"

#include "../corelib/thread/lock.h"
#include "../corelib/thread/rwlock.h"
#include "../corelib/thread/semaphore.h"

#include "../corelib/memory/comptr.h"
#include "../corelib/memory/refcounted.h"
#include "../corelib/memory/sharedata.h"


#include "../corelib/container/array.h"
#include "../corelib/container/kvpair.h"
#include "../corelib/container/tinyset.h"
#include "../corelib/container/tinymap.h"
#include "../corelib/container/map.h"
#include "../corelib/container/orderedmap.h"
#include "../corelib/container/syncqueue.h"
#include "../corelib/container/buffer.h"

#include "../corelib/fs/file.h"
#include "../corelib/fs/path.h"

#include "../corelib/tools/ascii.h"
#include "../corelib/tools/utils.h"
#include "../corelib/tools/textstream.h"
#include "../corelib/tools/log.h"
#include "../corelib/tools/uuid.h"
#include "../corelib/tools/throttle.h"
#include "../corelib/tools/atimer.h"

#include "../corelib/callback/func.h"
#include "../corelib/callback/callback.h"
#include "../corelib/callback/delegate.h"
#include "../corelib/callback/event.h"


#include "../corelib/thread/workerpool.h"
#include "../corelib/thread/message.h"
#include "../corelib/thread/corerunlooptaskqueue.h"
#include "../corelib/thread/corerunloopsafe.h"
#include "../corelib/thread/corerunloop.h"
#include "../corelib/thread/corethread.h"
#include "../corelib/thread/timer.h"
#include "../corelib/thread/dispatcher.h"

#include "../corelib/core/weakobjectptr.h"
#include "../corelib/core/object.h"

#include "../corelib/xml/xmldocument.h"

#include "../corelib/json/json.h"
#include "../corelib/json/jsonutils.h"

#include "../corelib/io/io.h"

#include "../corelib/ex/waitable.h"