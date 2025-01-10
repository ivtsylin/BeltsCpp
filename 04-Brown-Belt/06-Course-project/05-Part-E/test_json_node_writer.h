#pragma once

#include "json_node_writer.h"
#include "test_runner.h"

namespace TestJsonNodeWriter {

void TestPack() {
    using namespace json;
    {
        stringstream ss;
        Node node("abc");
        ss << node;
        ASSERT_EQUAL(ss.str(), "\"abc\"");
    }
    {
        stringstream ss;
        Node node(123);
        ss << node;
        ASSERT_EQUAL(ss.str(), "123");
    }
    {
        stringstream ss;
        Node node(0.4);
        ss << node;
        ASSERT_EQUAL(ss.str(), "0.4");
    }
    {
        stringstream ss;
        vector<Node> a{1, 2, 3, 4};
        Node node(a);
        ss << node;
        ASSERT_EQUAL(ss.str(), "[1,2,3,4]");
    }
    {
        stringstream ss;
        map<string, Node> a{{"abc", 2}, {"frg", 900}, {"eee", 14}, {"ggg", 16}};
        Node node(a);
        ss << node;
        ASSERT_EQUAL(ss.str(), "{\"abc\":2,\"eee\":14,\"frg\":900,\"ggg\":16}");
    }
};

}
