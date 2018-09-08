

struct BirthDay
{
    std::int32_t year;
    std::int32_t month;
    std::int32_t day;
};



JSON_AUTO(BirthDay, year, month, day)

struct Person {
    std::string name;
    std::string address;
    std::int32_t age;
    std::optional<std::vector<Person>> children;
    std::optional<BirthDay> birthDay;
};


JSON_AUTO(Person, name, address, age, children, birthDay)

struct A
{
    int a;
    int b;
    int c;
};

JSON_AUTO(A)


inline void from_json2(const json&j, A& object) \
{
    FOR_EACH(JSON_GET);
}


void func()
{
    Person p;
    p.age = 123;
    p.name = "prince";
    p.children = std::vector<Person>();
    Person c;
    c.name = "child1";
    c.age = 101;
    p.children->push_back(c);
    c.name = "child0";
    c.age = 100;
    c.children = p.children;
    p.birthDay = BirthDay({ 2020, 12, 12 });
    p.children->push_back(c);


    json j = p;

    std::cout << j.dump(4) << std::endl;
    std::cout << GET_ARG_COUNT(6, 5, 4, 3, 2, 1, 0);


    Person per = j;

    std::cout << json(per).dump(4) << std::endl;

    A aa;

    json aj = aa;

    from_json2(aj, aa);
}