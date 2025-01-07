#pragma once

#include "json_node.h"
#include "test_runner.h"

namespace TestJsonNode {

void TestPack() {
    using namespace json;
    {
        Node node("abc");
        ASSERT_EQUAL(node.AsString(), "abc");
        ASSERT(node.IsString());
        ASSERT(!node.IsDouble());
        ASSERT(!node.IsInt());
        ASSERT(!node.IsArray());
        ASSERT(!node.IsMap());
        ASSERT(!node.IsEmpty());
    }
    {
        Node node("abc");
        string result = node.ReleaseString();
        ASSERT(!node.IsString());
        ASSERT(!node.IsDouble());
        ASSERT(!node.IsInt());
        ASSERT(!node.IsArray());
        ASSERT(!node.IsMap());
        ASSERT(node.IsEmpty());
    }
    {
        Node node(0.5);
        ASSERT_EQUAL(node.AsDouble(), 0.5);
        ASSERT(!node.IsString());
        ASSERT(node.IsDouble());
        ASSERT(!node.IsInt());
        ASSERT(!node.IsArray());
        ASSERT(!node.IsMap());
        ASSERT(!node.IsEmpty());
    }
    {
        Node node(100);
        ASSERT_EQUAL(node.AsDouble(), 100.);
        ASSERT_EQUAL(node.AsInt(), 100);
        ASSERT(!node.IsString());
        ASSERT(node.IsDouble());
        ASSERT(node.IsInt());
        ASSERT(!node.IsArray());
        ASSERT(!node.IsMap());
        ASSERT(!node.IsEmpty());
    }
    {
        Node node(100.);
        ASSERT_EQUAL(node.AsDouble(), 100.);
        ASSERT(!node.IsString());
        ASSERT(node.IsDouble());
        ASSERT(!node.IsInt());
        ASSERT(!node.IsArray());
        ASSERT(!node.IsMap());
        ASSERT(!node.IsEmpty());
    }
    {
        map<std::string, Node> arr;
        arr["a"] = 1.;
        Node node = arr;
        
        ASSERT_EQUAL(node.AsMap().begin()->first, "a");
        ASSERT_EQUAL(node.AsMap().begin()->second.AsDouble(), 1.);
        ASSERT(!node.IsString());
        ASSERT(!node.IsDouble());
        ASSERT(!node.IsInt());
        ASSERT(!node.IsArray());
        ASSERT(node.IsMap());
        ASSERT(!node.IsEmpty());
    }
    {
        vector<Node> vec;
        vec.emplace_back(1.);
        Node node = vec;
        
        ASSERT(node.AsArray()[0] == Node(1.));
        ASSERT(!node.IsString());
        ASSERT(!node.IsDouble());
        ASSERT(!node.IsInt());
        ASSERT(node.IsArray());
        ASSERT(!node.IsMap());
        ASSERT(!node.IsEmpty());
    }

};

}
