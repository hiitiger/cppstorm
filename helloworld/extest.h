#pragma once

class Foo : public Storm::CoreObject
{
public:
    ~Foo() {
        std::cout << "\n  ~Foo():" << std::endl;
    }

    void foo(int x)
    {
        std::cout << "\n foo x:" << x;
    }

    void foo_same(int x)
    {
        std::cout << "\n foo_same x:" << x;
    }

    void foo2(int x, const std::string& str) const
    {
        std::cout << "\n foo x:" << x << ", str:" << str;
    }

    void foo3(int x, std::string str) 
    {
        std::cout <<  x * str.length() << std::endl;
    }

    Storm::Event<int(Foo*, int, int)> tsignal;
    Storm::Event<int(Foo*, int, int)> tsignal2;
};

class FooTrackable : public Storm::Trackable<>
{
public:

    int onSignal1(Foo*, int x, int y)
    {
        std::cout << "trackbale on onSignal1" << std::endl;
        return x + y;
    }

    int onSignal2(Foo*, int x, int y)
    {
        std::cout << "trackbale on signal2" << std::endl;
        return x + y;
    }
};

struct CopyTest
{
    CopyTest(int ,int ){}

    void show()
    {
        std::cout << "CopyTest::show()"<<std::endl;
    }

    CopyTest(const CopyTest& )
    {
        std::cout << "CopyTest(const CopyTest& d)" << std::endl;
    }


    CopyTest(CopyTest&& )
    {
        std::cout << "CopyTest(CopyTest&& d)" << std::endl;
    }

    CopyTest& operator=(const CopyTest&)
    {
        std::cout << "CopyTest& operator=(const CopyTest&)" << std::endl;
        return *this;
    }
};

struct  AbstractBB
{
    virtual void print()
    {
        std::cout << "AbstractBB:" << std::endl;
    }

    void show(int x)
    {
        std::cout << "AbstractBB show:" << x << std::endl;
    }
};

struct CircleBB : public AbstractBB
{
    int x = 0;
    int y = 0;

    double radius = 0;

    virtual void print()
    {
        std::cout << "x:" << x << ", y:" << y << ", r:" << radius << std::endl;
    }

    CircleBB(const CircleBB& d) = default;


    CircleBB(CircleBB&& d)
    {
        x = d.x;
        y = d.y;
        radius = d.radius;

        memset(&d, 0, sizeof(d));
    }

    CircleBB& operator=(const CircleBB&) = default;
 

    bool operator==(const CircleBB& o)
    {
        return x == o.x && y == o.y && Storm::compareValue(radius, o.radius);
    }


    CircleBB(int xx, int yy, double r) :x(xx), y(yy), radius(r) { ; }
};

struct TestEvent
{
    Storm::Event<void(std::shared_ptr<int>)> tsignal;
};

int cac_punch(int x, int y)
{
    return x * y;
}



void example_event()
{

    //auto foo = Storm::refThis(new Foo);
    //foo->tsignal.add([&](Foo* sender, int a, int b) {

    //    std::cout << "\n event listener, thread id: " << std::this_thread::get_id() << ", sender: " << sender << ",a:" << a << ", b:" << b << std::endl;
    //    return a - b;
    //});
 
    //{
    //    FooTrackable fooTrack;
    //    foo->tsignal.add(&FooTrackable::onSignal1, &fooTrack);
    //    foo->tsignal.add(&FooTrackable::onSignal2, &fooTrack);

    //    foo->tsignal(foo, 1, 1);
    //}

    //foo->tsignal(foo, 2, 2);

    //{
    //    FooTrackable fooTrack;
    //    foo->tsignal.add(&FooTrackable::onSignal1, &fooTrack);
    //    auto conn = foo->tsignal.add(&FooTrackable::onSignal2, &fooTrack);

    //    foo->tsignal2.add(&FooTrackable::onSignal1, &fooTrack);
    //    foo->tsignal2.add(&FooTrackable::onSignal2, &fooTrack);

    //    foo->tsignal(foo, 3, 3);
    //    foo->tsignal2(foo, 223, 223);
    //   // foo->tsignal.removeTrackable(&fooTrack);
    //    conn.disconnect();
    //    foo->tsignal(foo, 123, 123);
    //    foo->tsignal2(foo, 223, 223);

    //}

    //foo->tsignal(foo, 100, 100);

    //auto callback = Storm::delegate(&cac_punch);
    //std::cout << "\n callback: " << callback(10, 10);

    //auto callback2 = Storm::delegate([](int x, int y) {
    //    return x * y + 100;
    //});

    //auto recCallback2 = Storm::Delegate<int(int, int)>(callback2);
    //std::cout << "\n callback2 res: " << recCallback2(10, 10);

    //auto callback3 = Storm::delegate(&Foo::foo3, foo.get());
    //std::cout << "\n callback3 res: " << callback3(10, "Hello invoker");

    //auto callback4 = Storm::delegate([=](int x, int y) {
    //    return x * y + callback2(x, y);
    //});

    //std::cout << "\n callback4 res: " << callback4(10, 10);

    //callback = callback2;
    //std::cout << "\n callback: " << callback(10, 10);



    using namespace Storm;

    Event<void(std::string v, std::string&& str)> signal;

    auto conn1 = signal.add([](std::string v, std::string&& str) {
        std::cout << "slot 1: " << v << std::string(std::move(str)) << std::endl;
    });

    auto conn2 = signal.add([](std::string v, std::string&& str) {
        DAssert(v.size());
        DAssert(!str.size());
        std::cout << "slot 2: " << v << str << std::endl;
    });

    std::string value = "value";
    signal.emit(value, "123emit");
    signal(value, "123call");

    DAssert(value.size());
}

int  scall(int x, std::string& str, std::string&& str2)
{
    while (x--)
    {
        str.append(str2);
    }
    return (int)str.length();
};

int  scall0()
{
    return (int)rand() % 1000;
};

void example_callback()
{
    int capvalue = 10;
    auto f1 = [=](int x, std::string& str, std::string&& str2)
    {
        std::string xstr = std::move(str2);
        std::cout << "capture value: " << capvalue << std::endl;
        while (x--)
        {
            str.append(xstr).append(std::to_string(x));
        }
        return (int)str.length();
    };


    {
        Storm::Callback<int(int x, std::string& str, std::string&& str2)> test = (scall);

        std::string str = "19";
        std::string str2 = "r value ref";
        std::string str3 = std::move(str2);

        test(10, std::ref(str), std::move(str3));
        str3 = "@";

        f1(5, str, std::move(str3));
        std::cout << "f1, str3:" << str3 << std::endl;


        //std::function<int()> cb1 = Storm::bind(scall, 5, std::ref(str), std::move(str3));

        //auto f1_res = cb1();

        //std::cout << "cb1:" << f1_res << ", str:" << str << std::endl;
    }

    {
        Storm::Callback<int()> test = scall0;

        Storm::Callback<int()> test2(test);

        std::cout<<test2() <<std::endl;
    }

    {

        CopyTest test = { 1, 1 };
        std::string str2 = "19";
        std::string pp = "@@";
        auto cb2 = Storm::bind([test=std::move(test)](std::string& str2, const std::string& pp) mutable
        {
            test.show();
            str2.append(pp);
            return (int)str2.length();
        }, std::ref(str2), std::cref(pp));

        std::cout << "cb2: " << cb2() << ", str2:" << str2 << std::endl;
        std::cout << "cb2: " << cb2() << ", str2:" << str2 << std::endl;
    }
   
    {
        Storm::Callback<int()> cb3 = Storm::bind(cac_punch, 10, 18);
        std::cout << "cb3: " << cb3() << std::endl;

        auto foo = new Foo;
        auto cb4 = Storm::bind(&Foo::foo3, Storm::WeakObjectPtr<Foo>(foo), 10);
        auto cb5 = Storm::bind(cb4, "cb foo");
        cb5();

        delete foo;

        auto cbf = std::bind(cb5);
        cbf();

    }

    {
        auto spCirclrBB = std::make_shared<CircleBB>(10, 10, 10.0);
        auto spDelegate = Storm::delegate(&CircleBB::print, spCirclrBB);
        spDelegate();

        auto spCallback1 = Storm::bind(&AbstractBB::show, spCirclrBB);

        spCallback1(10);
    }



    auto spTestEvent = std::make_shared<TestEvent>();

    spTestEvent->tsignal.add([](std::shared_ptr<int> v) {
        std::cout << "\n----Test async emit" << std::endl;
        std::cout << "--------In delegate callback, thread id:"<<std::this_thread::get_id()<<", value v is:" << *v.get() <<std::endl;
        std::cout << "----Test async emit end" << std::endl;

        DAssert(*v.get() == 1001);
    });

    auto value = std::make_shared<int>(1000);
    Storm::App::instance()->appDispatcher()->emitAsync(spTestEvent, &spTestEvent->tsignal, value);

    *value.get() = 1001;
}

void example_list()
{
    CircleBB b(1, 1, 100.0);
    CircleBB b2(2, 2, 100.0);
    CircleBB b3(3, 3, 988.0);

    CircleBB mb = std::move(b);

    Storm::Array<CircleBB> foolist;

    foolist.reserve(32);

    foolist.insert(0, b);
    foolist.insert(0, b2);
    foolist.push_back((b3));
    foolist.push_back(std::move(b3));


    for (int i = 0; i != foolist.size(); ++i)
    {
        const CircleBB& t = foolist[i];
        std::cout << "\n,t.x << " << t.x << ", t.y: " << t.y << ", t.r:" << t.radius;
    }

    std::cout << "\n foolist size:" << foolist.size();
    std::cout << "\n foolist capacity:" << foolist.capacity();

    foolist.clear();
    std::cout << "\n foolist size:" << foolist.size();
    std::cout << "\n foolist capacity:" << foolist.capacity();

    foolist.destroy();
    std::cout << "\n foolist capacity:" << foolist.capacity();

    foolist.reserve(8);

    for (int i = 0; i != 100; ++i)
    {
        CircleBB bb(i, i, i*2.0);
        foolist.push_back(bb);
    }
    std::cout << "\n foolist size:" << foolist.size();
    std::cout << "\n foolist capacity:" << foolist.capacity();

    Storm::Array<CircleBB> foolist2;

    foolist2 = foolist;
    foolist.shrink();
    std::cout << "\n foolist size:" << foolist.size();
    std::cout << "\n foolist capacity:" << foolist.capacity();
    std::cout << "\n foolist2 size:" << foolist2.size();
    std::cout << "\n foolist2 capacity:" << foolist2.capacity();

    foolist2.removeOne(b);
    foolist2.push_back(b);
    std::cout << "\n foolist size:" << foolist.size();
    std::cout << "\n foolist capacity:" << foolist.capacity();
    std::cout << "\n foolist2 size:" << foolist2.size();
    std::cout << "\n foolist2 capacity:" << foolist2.capacity();

   /* for (int i = 0; i != foolist.size(); ++i)
    {
        const CircleBB& t = foolist[i];
        std::cout << "\n,t.x << " << t.x << ", t.y: " << t.y << ", t.r:" << t.radius;
    }

    for (int i = 0; i != foolist2.size(); ++i)
    {
        const CircleBB& t = foolist2[i];
        std::cout << "\n,t.x << " << t.x << ", t.y: " << t.y << ", t.r:" << t.radius;
    }*/
}

void example_map()
{
    std::cout << std::endl;

    Storm::TinySet<std::string> sset;
    sset.insert("123");
    sset.insert("1234");
    sset.insert("123");
    DAssert(sset.size() == 2);

    Storm::TinyMap<std::string, std::string> smap;

    smap.insert("1", "2");
    smap.insert("1", "3");
    smap.insert("2", "4");
    smap.insert("0", "5");
    smap.insert("3", "6");

    for (int i = 0; i < smap.size(); i++)
    {
        std::cout << smap.keyAt(i) << "," << smap.valueAt(i) << std::endl;
    }

    Storm::Skiplist<int, std::string> map;
    for (int i = 0; i < 10; i++)
    {
        map.insert(i, std::to_string(9-i));
        map.insert(9-i, std::to_string(i));
    }
    std::cout << map << std::endl;
    map.remove(5);
    std::cout << map << std::endl;

    const int num = 1000000;
    std::vector<int> vec(num);
    for (int i = 0; i < num; i++)
    {
        vec[i] = i ;
    }

    {
       /* __perf_scope(tinymap);
        Storm::TinyMap<int, std::string> map;
        for (int i = 0; i < num; i++)
        {
            map.insert(vec[i], std::to_string(i));
        }

        for (int i = 0; i < num; i++)
        {
            map[vec[i]];
        }*/
    }
    auto ix = 0;
    for (int i = 0; i < num; i++)
    {
        ix += vec[i];
    }

    {
        Storm::Skiplist<int, std::string> to_test;
     

        {
            __perf_scope(SkipList);
            Storm::Skiplist<int, std::string> map;
            for (int i = 0; i < num; i++)
            {
                map.insert(std::make_pair(vec[i], std::to_string(i)));
            }

            for (int i = 0; i < num; i++)
            {
                map[vec[i]];
            }

            to_test = std::move(map);
        }


        {
            __perf_scope(stdmap);
            std::map<int, std::string> map;
            for (int i = 0; i < num; i++)
            {
                map.insert(std::make_pair(vec[i], std::to_string(i)));
            }

            for (int i = 0; i < num; i++)
            {
                map[vec[i]];
            }
        }


        vec.clear();
        for ( int i = 0; i != to_test.size(); ++i)
        {
            vec.push_back(std::atoi(to_test.valueAt(i).c_str()));
        }
        DAssert(std::is_sorted(vec.begin(), vec.end()));

        vec.clear();
        for (auto it : to_test)
        {
            std::string vv = it.second;

            vec.push_back(std::atoi((vv).c_str()));
        }
        DAssert(std::is_sorted(vec.begin(), vec.end()));
    }

    {
        using namespace Storm;
        OrderedMap<int, int> omap;
        vec.clear();
        for (int i = 0; i < num; i++)
        {
            vec.push_back( num - i);
        }

        for (int i = 0; i < num; i++)
        {
            omap.insert(vec[i], vec[i]);
        }

        auto values = omap.values();
        DAssert(values == vec);

    }
}

void example_string()
{
    std::cout << std::endl;
    std::vector<std::string> vec{ "1", "2", "3", "@", "abc", "." "com" };

    auto res = stdxx::to_lower<std::string>("aBc");
    auto jvec = stdxx::join(vec, "fxxk");

    std::string jvec2;
    for (auto s : vec)
    {
        jvec2.append(s);
        jvec2.append("fxxk");
    }
    jvec2 = jvec2.substr(0, jvec2.size() - 4);

    DAssert(jvec == jvec2);

    std::vector<std::string> rr;
    auto cvec = stdxx::split<std::string>(("123, fa,143,,  ,,"), ',', false);
    DAssert(cvec == std::vector<std::string>({ "123", " fa", "143", "", "  ", "", ""}));
    auto cvec2 = stdxx::split<std::string>(("123, fa,143,,  ,,"), ',');
    DAssert(cvec2 == std::vector<std::string>({ "123", " fa", "143",  "  "}));

    cvec = stdxx::split<std::string>(",", ',', false);
    cvec = stdxx::split<std::string>("123", ',', false);
    cvec = stdxx::split<std::string>(",,", ',', false);
    cvec = stdxx::split<std::string>(",123", ',', false);

  //  auto rsstr = stdxx::replace_substr(L"123@1@123@1@1avc", L"@1", "@2");

    auto rsstr = stdxx::replace_substr<std::wstring>(L"123@1@123@1@1avc@1", L"@1", L"@2");

    DAssert(stdxx::ends_with(rsstr, L"@2"));

    auto hex1 = Storm::Utils::toHex(0x9898);
    DAssert(hex1 == "9898");

    auto hex2 = Storm::Utils::toHex(0x70f09898);
    DAssert(hex2 == "70f09898");

    auto hex3 = Storm::Utils::toHex(0xa0f09800);
    DAssert(hex3 == "a0f09800");

    std::string xxx = "012345@ABC";
    auto hex4 = Storm::Utils::toHexCode(xxx);
    DAssert(hex4 == "30313233343540414243");

    auto d1 = Storm::Utils::fromHex("9898");
    DAssert(d1 == 0x9898);

    auto d2 = Storm::Utils::fromHex("a0f09800");
    DAssert(d2 == 0xa0f09800);

    auto s1 = Storm::Utils::fromHexCode(hex4);
    DAssert(s1 == "012345@ABC");

    auto s2 = Storm::Utils::fromHexCode("303132333435404142434");
    DAssert(s2 == "012345@ABC@");

    auto s3 = stdxx::to_upper<std::string>(("abcX8.24f"));
    DAssert(s3 == "ABCX8.24F");

    auto s4 = std::string(" 12, 432  ");
    s4 = stdxx::trim(s4);
    DAssert(s4 == "12, 432");

    auto src = std::string("123");
    auto enc = Storm::Utils::simpleEncode(src, 'x');
    auto dec = Storm::Utils::simpleDecode(enc, 'x');
        
    DAssert(src == dec);
}

void example_xml()
{
    const char* xmlstring1 = "<xml><mesh name='\xE5\x8D\x95\xE6\x9C\xBA'><node><![CDATA[0, 0, 100 && trans: 10,10]]></node></mesh></xml>";
    Storm::XmlDocument xmlDoc1;
    xmlDoc1.loadFromUtf8(xmlstring1);

    std::wstring name = xmlDoc1.rootElement()->child(L"mesh")->attributeValue(L"name");
    DAssert(name == L"单机");

    std::wstring nodevalue = xmlDoc1.elementByPath(L"/xml/mesh/node")->text();
    DAssert(nodevalue == L"0, 0, 100 && trans: 10,10");
}

void example_json()
{
    using nlohmann::json;

    json obj_empty;
    DAssert(obj_empty.is_null());
    json obj_emptyArray = json::array();
    DAssert(obj_emptyArray.size() == 0);
    DAssert(obj_emptyArray.is_array());
    json obj_emptyObject = json::object();
    DAssert(obj_emptyObject.size() == 0);
    DAssert(obj_emptyObject.is_object());

    json obj{
        { "k1", "v1" },
        { "k2", 42.0 },
        { "k3",  { "a", 123.0 } },
        { "k4", { {"happy", "no"}, 
                  {"what", {{ "json", true }}}
               }},
        { "k5", nullptr}
      };

    obj["rect"] = { {"x", 123} };

    DAssert(obj["k2"] == 42.0);
    DAssert(obj["k3"].is_array());
    DAssert(obj["k3"][1] == 123.0);
    DAssert(obj["k4"].is_object());
    DAssert(obj["k4"]["happy"] == "no");
    DAssert(obj["k4"]["what"]["json"] == true);
    DAssert(obj["k5"] == nullptr);

    std::cout << obj.dump() << std::endl;

    std::string err;
    const std::string jsonstr = R"({
        "name": "\u5355\u673A",
        "nod" : {
            "cor": [
                10,
                10
            ],
            "r" : 100
        }
    })";

    json obj2 = json::parse(jsonstr);

    std::string name = obj2["name"].get<std::string>();
    std::wstring wname = Storm::Utils::fromUtf8(name);

    DAssert(wname == L"单机");
    DAssert(obj2["nod"]["cor"][1] == 10);
}


void example_thread()
{
    Storm::WorkerPool pool(4);

    for (int i = 0; i != 100; ++i)
    {
        /*pool.add(Storm::bind([](int i) {

            std::this_thread::sleep_for(std::chrono::milliseconds(i));
            std::cout << "index: " << i << ", this thread: " << std::this_thread::get_id()<<std::endl;
        }, i));*/

        pool.add([i = i]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(i));
            std::cout << "index: " << i << ", this thread: " << std::this_thread::get_id() << std::endl;
        });
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(20));

    pool.stop();
    std::cout << "stopped" <<std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "restart..." << std::endl;
    pool.restart(4);
    std::cout << "restarted" << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(2));

    for (int i = 0; i != 100; ++i)
    {
        /*pool.add(Storm::bind([](int i) {

        std::this_thread::sleep_for(std::chrono::milliseconds(i));
        std::cout << "index: " << i << ", this thread: " << std::this_thread::get_id()<<std::endl;
        }, i));*/

        pool.add([i = i]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(i));
            std::cout << "index: " << i << ", this thread: " << std::this_thread::get_id() << std::endl;
        });
    }
    std::this_thread::sleep_for(std::chrono::seconds(2));

}


void example_geometry()
{
    Storm::Point<int> pt;
    Storm::Size<int> sz{ 90, 90 };
    Storm::Rect<int> rect(pt, sz);

    rect.moveTo(Storm::Point<int>(10, 10));
    rect.adjust(Storm::Margin<int>(5, 5, 20, 20));
    auto rc = rect;
    std::cout << rc;
}

void example_time()
{
    volatile int sink;
    for (auto size = 1ull; size < 10000000ull; size *= 100) {
        // record start time
        //auto start = std::chrono::steady_clock::now();
        auto start = Storm::TimeTick::now();

        // do some work
        std::vector<int> v((size_t)size, 42);
        sink = std::accumulate(v.begin(), v.end(), 0u); // make sure it's a side effect
        // record end time
        //auto end = std::chrono::steady_clock::now();
        auto end = Storm::TimeTick::now();

        //std::chrono::duration<double> diff = end - start;
        auto diff = end - start;
        std::cout << "Time to fill and iterate a vector of "
            << size << " ints : " << diff.microSecs()  << " microSecs\n";
    }

    for (auto size = 1ull; size < 10000000ull; size *= 100) {
        // record start time
        //auto start = std::chrono::steady_clock::now();
        //auto start = Storm::TimeTick::now();

        __perf_scope(xxx);
        // do some work
        std::vector<int> v((size_t)size, 42);
        sink = std::accumulate(v.begin(), v.end(), 0u); // make sure it's a side effect
                                                        // record end time
                                                        //auto end = std::chrono::steady_clock::now();
        //auto end = Storm::TimeTick::now();

        //std::chrono::duration<double> diff = end - start;
        //auto diff = end - start;
        /* std::cout << "Time to fill and iterate a vector of "
             << size << " ints : " << diff.microSecs() / 1000000.0 << " s\n";*/
    }
}

void example_weakptr()
{
    using namespace Storm;
    CoreObject* obj = new CoreObject;

    WeakObjectPtr<CoreObject> ptr(obj);
    DAssert(ptr);

    WeakObjectPtr<CoreObject> ptr2 = std::move(ptr);
    DAssert(ptr2 && !ptr);

    obj->deleteLater();
    obj->destroyEvent().add([=](CoreObject*) {
        
        CoreRunloop::current()->post([=] {
            std::cout << "after destroy, ptr2 is: " << (bool)ptr2 << std::endl;
        });
        
    });
}

class TestLambda
{
public:
    void runTest(int a , int b)
    {
        // true, n, false

        m_runnig = true;
        thread_.reset(new std::thread([=]() {

            std::cout << "run in thread, m_runnig:" << this->m_runnig << std::endl;
            std::cout << "run in thread, a+b = " << a + b << std::endl;
            ::Sleep(3 * 1000);
            std::cout << "run in thread, after sleep, m_runnig:" << this->m_runnig << std::endl;
        }));

        ::Sleep(1000);
        m_runnig = false;
        thread_->join();
        thread_.reset(nullptr);
    }

private:
    std::unique_ptr<std::thread> thread_;
    volatile bool m_runnig;
};

struct AsyncOperation {
    AsyncOperation() {

    }
    AsyncOperation(const AsyncOperation &){
        std::cout << "AsyncOperation(const AsyncOperation &other)" << '\n';
    }

    ~AsyncOperation() {
        std::cout << " ~AsyncOperation() " << '\n';
    }
    void operator()()
    {
        std::cout << "AsyncOperation::operator()()" << '\n';
    }

    void func() const {
        std::cout << "AsyncOperation::func()" << '\n';
    }
};


void exmaple_lambda()
{
    TestLambda lambda;
    lambda.runTest(10, 10);

    auto cc = std::bind(cac_punch);

    AsyncOperation op;
    std::function<void()> callback = [=]() { op.func(); };
    callback();

    std::function<void()> callback2 = callback;
    callback2();

    auto cb = Storm::bind([=]() { op.func(); });
    cb();
    auto cb2 = cb;
    cb2();
}

void example_bind()
{
    auto spAsynOp = std::make_shared<AsyncOperation>();

    std::weak_ptr<AsyncOperation> wpAsynOp = spAsynOp;

    std::cout << "spAsyncOP use count: "<<spAsynOp.use_count() << std::endl;

    /* not compile
    auto callback = std::bind(&AsyncOperation::operator(), wpAsynOp); 
    callback();*/

  /*  auto callback = std::bind(&AsyncOperation::operator());
    callback(spAsynOp.get());
    std::cout << "spAsyncOP use count after std bind: " << spAsynOp.use_count() << std::endl;
*/
    auto callback2 = Storm::bind(&AsyncOperation::operator());
    callback2(spAsynOp.get());
    std::cout << "spAsyncOP use count after storm bind: " << spAsynOp.use_count() << std::endl;


    {
        //
        int xyz = 100;
        auto lambda = [&]() {
            xyz -= 1;
        };

       // auto lambda2 = lambda;

       // Storm::Callback<void()> func1 = Storm::bind(lambda);


        Storm::Callback<void()>  func1 = Storm::lambda([&, nocopy]() {
            xyz -= 1;
        });
        func1();

        std::cout << "xyz is " << xyz << std::endl;

        std::function<void()> func2 = (lambda);
       // func2();
        std::cout << "xyz is " << xyz << std::endl;

        func1 = func2;
        func1();

        func2 = func1;
        func2();
        std::cout << "xyz is " << xyz << std::endl;

    }

    {

    }
}


void example_waitable()
{
    using namespace Ex;

    auto ev = std::make_shared<Storm::WaitableEvent>();
    auto w1 = waitable(100).async_wait_on(Scheduler::static_scheduler()).add([ev](WaitableRef) {
        std::cout << "waitable timer callback: " << Storm::TimeTick::now().secs() << std::endl;
        ev->set();
    });

    auto w2 = waitable(ev->handle()).timeout(1000).async_wait_on(Scheduler::static_scheduler()).add([](WaitableRef w) {
        std::cout << "waitable handle callback: " << Storm::TimeTick::now().secs() << std::endl;
        std::cout << "waitable state: " << (int)w.state() << std::endl;
    });

    waitable(1500).async_wait_on(Scheduler::static_scheduler()).add([=](WaitableRef w) mutable {
        w1.dispose();
        w2.dispose();
        Storm::CoreRunloop::current()->post([w]()mutable {w.dispose(); });
    });
}

void example_throttle()
{
    using namespace Storm;
    {
        Throttle throttle(3, 100);
        std::vector<bool> vec;
        for (auto i = 0; i != 10; i++)
        {
            vec.push_back(throttle.tick());
            std::this_thread::sleep_for(std::chrono::milliseconds(30));
        }
        std::vector<bool> vec_expect = { true, true, true, false, true, true, true, false, true, true };
        assert(vec_expect == vec);
    }

    {
        Throttle throttle(1, 300);
        std::vector<bool> vec;
        for (auto i = 0; i != 10; i++)
        {
            vec.push_back(throttle.tick());
            std::this_thread::sleep_for(std::chrono::milliseconds(101));
        }
        std::vector<bool> vec_expect = { true, false, false, true, false, false, true, false, false, true };
        assert(vec_expect == vec);
    }

    {
        Throttle throttle(1, 300);
        std::vector<bool> vec;
        for (auto i = 0; i != 10; i++)
        {
            vec.push_back(throttle.tick());
            std::this_thread::sleep_for(std::chrono::milliseconds(90));
        }
        std::vector<bool> vec_expect = { true, false, false, false, true, false, false, false, true, false };
        assert(vec_expect == vec);
    }
}

void example_snowflake()
{
    using namespace Storm;
    using namespace std::chrono;
    {
        Snowflake<true> ff;
        int64_t start = duration_cast<microseconds>(steady_clock::now().time_since_epoch()).count();
        priv::usleep((start / 1000 + 1) * 1000 - start);
        start = duration_cast<microseconds>(steady_clock::now().time_since_epoch()).count();
        for (auto i = 0; i != 4096 * 1; ++i)
        {
            ff.generate();
        }
        int64_t end = duration_cast<microseconds>(steady_clock::now().time_since_epoch()).count();

        std::cout << "duration:" << end - start << std::endl;
    }

    {
        Snowflake<true> ff;
        int64_t start = duration_cast<microseconds>(steady_clock::now().time_since_epoch()).count();
        priv::usleep((start / 1000 + 1) * 1000 - start);
        start = duration_cast<microseconds>(steady_clock::now().time_since_epoch()).count();
        for (auto i = 0; i != 4096 * 2; ++i)
        {
            ff.generate();
        }
        int64_t end = duration_cast<microseconds>(steady_clock::now().time_since_epoch()).count();

        std::cout << "duration:" << end - start << std::endl;
    }

}


void example_async()
{
    using namespace concurrency_;

   auto t = delayed(1000)
        | ui([] {
            std::cout << "running on ui" << std::endl;
        })
        | delay(100)
        | pool([] {
            std::cout << "running on pool" << std::endl;
            throw std::exception("test exception");
            return std::this_thread::get_id();
        })
        | delay<std::thread::id>(100)
        | pool([](std::thread::id id) {
            std::cout << "running on pool" << std::endl;
            std::cout <<  "received  thread_id" << id  << std::endl;
        });
       
    try
    {
        t.wait();
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }

    //__await async([] {
    //    throw std::exception("test");
    //});
    //std::cout << "running on pool, this_thread: " << std::this_thread::get_id() << ", ui thread: " << AppUI::threadId() << std::endl;

    //__awaitui;
    //std::cout << "running on ui, this_thread: " << std::this_thread::get_id() << ", ui thread:" << AppUI::threadId() << std::endl;

    //__awaitpool;
    //std::cout << "running on pool, this_thread: " << std::this_thread::get_id() << ", ui thread: " << AppUI::threadId() << std::endl;

    stdxx::timer::forever(2000, [c = CopyTest(1, 2)]{

        static int x = 0;
        std::cout << "x: " << x++ << std::endl;

        __debug("test2") << "forever(2000) " << x;

        return  x > 10 ? false : true;
    });

    stdxx::timer::forever(1000, [c = CopyTest(1, 2)]{
        static int y = 0;
        std::cout << "y: " << y++ << std::endl;

        __debug("test1") << "forever(1000) " << y;
        return  y > 10 ? false : true;
    });
}

inline void example_buffer()
{
    using namespace Storm;

    Buffer buf(100);
    DAssert(buf.capacity() == 100);

    const char data[] = "bian";
    buf.add((const uint8_t*)data, 4);
    DAssert(buf.size() == 4);

    auto ptr = buf.allocToAdd(100);
    DAssert(buf.size() == 104);

    for (auto i =0; i != 100/4; ++i)
    {
        memcpy(ptr, data, 4);
        ptr += 4;
    }
    ptr = buf.data() + 100;
    memcpy(ptr, "fxxk", 4);

    buf.remove(4, 96);
    DAssert(buf.size() == 8);

    std::string str((const char*)buf.data(), 4);
    DAssert(str == data);

    std::string str2((const char*)buf.data() + 4, 4);
    DAssert(str2 == "fxxk");

}

void example_fs()
{
    using namespace Storm;
    {
        Path path1;
        std::wcout << path1.fullPath() << ", " << path1.absoluteDir() << std::endl;
    }
    
    {
        Path path1(L"../");
        std::wcout << path1.fullPath() << ", " << path1.absoluteDir() << std::endl;
        std::wcout << path1.fileName() << ", " << path1.baseName() << ", " << path1.extension() << std::endl;
    }
    
    {
        Path path1(L".abc");
        std::wcout << path1.fullPath() << ", " << path1.absoluteDir() << std::endl;
        std::wcout << path1.fileName() << ", " << path1.baseName() << ", " << path1.extension() << std::endl;
    }
    {
        Path path1(L"a.text.g");
        std::wcout << path1.fullPath() << ", " << path1.absoluteDir() << std::endl;
        std::wcout << path1.fileName() << ", " << path1.baseName() <<", " << path1.fullExtension() << std::endl;
    }

    {
        std::experimental::filesystem::path path1(L"a.text.g");
        std::wcout << path1.native() << ", " << path1.parent_path() << std::endl;
        std::wcout << path1.filename() << ", " << path1.stem() << ", " << path1.extension() << std::endl;
    }

    File file(Utils::applicationDirPath() +  L"\\..\\..\\storm.pro");
    file.open(File::ReadOnly);
    auto lines = file.readLines();
    for (auto line : lines)
    {
        std::string str((const char*)line.data(), line.size());
        std::cout << str;
    }
}