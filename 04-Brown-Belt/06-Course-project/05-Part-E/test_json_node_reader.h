#pragma once

#include "json_node_reader.h"
#include "test_runner.h"

namespace TestJsonNodeReader {

void TestPackCorrect() {
    using namespace json;
    {
        stringstream ss;
        Node node;
        ss << "\"test\"";
        ss >> node;
        ASSERT_EQUAL(node.AsString(), "test");
    }
    
    {
        stringstream ss;
        Node node;
        ss << "true";
        ss >> node;
        ASSERT_EQUAL(node.AsBool(), true);
    }
    {
        stringstream ss;
        Node node;
        ss << "false";
        ss >> node;
        ASSERT_EQUAL(node.AsBool(), false);
    }
    {
        stringstream ss;
        Node node;
        ss << "null";
        ss >> node;
        ASSERT_EQUAL(node.IsEmpty(), true);
    }
    {
        stringstream ss;
        Node node;
        ss << "0";
        ss >> node;
        ASSERT_EQUAL(node.AsInt(), 0);
    }
    {
        stringstream ss;
        Node node;
        ss << "0";
        ss >> node;
        ASSERT_EQUAL(node.AsDouble(), 0.);
    }
    {
        stringstream ss;
        Node node;
        ss << "0.0";
        ss >> node;
        ASSERT_EQUAL(node.AsDouble(), 0.);
    }
    {
        stringstream ss;
        Node node;
        ss << "-0";
        ss >> node;
        ASSERT_EQUAL(node.AsInt(), 0);
    }
    {
        stringstream ss;
        Node node;
        ss << "-0";
        ss >> node;
        ASSERT_EQUAL(node.AsDouble(), 0.);
    }
    {
        stringstream ss;
        Node node;
        ss << "-0.0";
        ss >> node;
        ASSERT_EQUAL(node.AsDouble(), 0.);
    }
    {
        stringstream ss;
        Node node;
        ss << "0.5";
        ss >> node;
        ASSERT_EQUAL(node.AsDouble(), 0.5);
    }
    {
        stringstream ss;
        Node node;
        ss << "-0.5";
        ss >> node;
        ASSERT_EQUAL(node.AsDouble(), -0.5);
    }
    {
        stringstream ss;
        Node node;
        ss << "1";
        ss >> node;
        ASSERT_EQUAL(node.AsInt(), 1);
    }
    {
        stringstream ss;
        Node node;
        ss << "1";
        ss >> node;
        ASSERT_EQUAL(node.AsDouble(), 1.);
    }
    {
        stringstream ss;
        Node node;
        ss << "1.0";
        ss >> node;
        ASSERT_EQUAL(node.AsDouble(), 1.);
    }
    {
        stringstream ss;
        Node node;
        ss << "1.5";
        ss >> node;
        ASSERT_EQUAL(node.AsDouble(), 1.5);
    }
    {
        stringstream ss;
        Node node;
        ss << "1.0";
        ss >> node;
        ASSERT_EQUAL(node.AsDouble(), 1.);
    }
    {
        stringstream ss;
        Node node;
        ss << "1.5";
        ss >> node;
        ASSERT_EQUAL(node.AsDouble(), 1.5);
    }
    {
        stringstream ss;
        Node node;
        ss << "-1.5";
        ss >> node;
        ASSERT_EQUAL(node.AsDouble(), -1.5);
    }
    {
        stringstream ss;
        Node node;
        ss << "-1";
        ss >> node;
        ASSERT_EQUAL(node.AsInt(), -1);
    }
    {
        stringstream ss;
        Node node;
        ss << "-1";
        ss >> node;
        ASSERT_EQUAL(node.AsDouble(), -1.);
    }
    {
        stringstream ss;
        Node node;
        ss << "[0,1,2,3]";
        ss >> node;
        ASSERT_EQUAL(node.AsArray()[0].AsInt(), 0);
        ASSERT_EQUAL(node.AsArray()[1].AsInt(), 1);
        ASSERT_EQUAL(node.AsArray()[2].AsInt(), 2);
        ASSERT_EQUAL(node.AsArray()[3].AsInt(), 3);
    }
    {
        stringstream ss;
        Node node;
        ss << "[]";
        ss >> node;
        ASSERT_EQUAL(node.AsArray().size(), 0);
    }
    {
        stringstream ss;
        Node node;
        ss << "{\"10\":4,\"20\":3}";
        ss >> node;
        auto x = node.AsMap().begin();
        ASSERT_EQUAL(x->first, "10");
        ASSERT_EQUAL(x->second.AsInt(), 4);
        
        ++x;
        ASSERT_EQUAL(x->first, "20");
        ASSERT_EQUAL(x->second.AsInt(), 3);
    }
};

void TestPackIncorrect() {
    using namespace json;
    {
        stringstream ss;
        Node node;
        ss << "\"";
        try {
            ss >> node;
            ASSERT(false);
        } catch (runtime_error& err) {
            ASSERT_EQUAL(string(err.what()), json_errros::ReadStringError);
        } catch (...) {
            ASSERT(false);
        }
    }
    {
        stringstream ss;
        Node node;
        ss << "t";
        try {
            ss >> node;
            ASSERT(false);
        } catch (runtime_error& err) {
            ASSERT_EQUAL(string(err.what()), json_errros::ReadBoolError);
        } catch (...) {
            ASSERT(false);
        }
    }
    {
        stringstream ss;
        Node node;
        ss << "f";
        try {
            ss >> node;
            ASSERT(false);
        } catch (runtime_error& err) {
            ASSERT_EQUAL(string(err.what()), json_errros::ReadBoolError);
        } catch (...) {
            ASSERT(false);
        }
    }
    {
        stringstream ss;
        Node node;
        ss << "nul";
        try {
            ss >> node;
            ASSERT(false);
        } catch (runtime_error& err) {
            ASSERT_EQUAL(string(err.what()), json_errros::ReadNullError01);
        } catch (...) {
            ASSERT(false);
        }
    }
    {
        stringstream ss;
        Node node;
        ss << "nulk";
        try {
            ss >> node;
            ASSERT(false);
        } catch (runtime_error& err) {
            ASSERT_EQUAL(string(err.what()), json_errros::ReadNullError02);
        } catch (...) {
            ASSERT(false);
        }
    }
    {
        stringstream ss;
        Node node;
        ss << "111111111111111111111111111111111111111111";
        try {
            ss >> node;
            ASSERT(false);
        } catch (runtime_error& err) {
            ASSERT_EQUAL(string(err.what()), json_errros::ReadNumberError01);
        } catch (...) {
            ASSERT(false);
        }
    }
    {
        stringstream ss;
        Node node;
        ss << "10.0asd";
        try {
            ss >> node;
            ASSERT(false);
        } catch (runtime_error& err) {
            ASSERT_EQUAL(string(err.what()), json_errros::ReadNumberError02);
        } catch (...) {
            ASSERT(false);
        }
    }
    {
        stringstream ss;
        Node node;
        ss << "[";
        try {
            ss >> node;
            ASSERT(false);
        } catch (runtime_error& err) {
            ASSERT_EQUAL(string(err.what()), json_errros::ReadArrayError01);
        } catch (...) {
            ASSERT(false);
        }
    }
    {
        stringstream ss;
        Node node;
        ss << "[1,2";
        try {
            ss >> node;
            ASSERT(false);
        } catch (runtime_error& err) {
            ASSERT_EQUAL(string(err.what()), json_errros::ReadArrayError02);
        } catch (...) {
            ASSERT(false);
        }
    }
    {
        stringstream ss;
        Node node;
        ss << "[1,]";
        try {
            ss >> node;
            ASSERT(false);
        } catch (runtime_error& err) {
            ASSERT_EQUAL(string(err.what()), json_errros::ReadArrayError02);
        } catch (...) {
            ASSERT(false);
        }
    }
    {
        stringstream ss;
        Node node;
        ss << "[,]";
        try {
            ss >> node;
            ASSERT(false);
        } catch (runtime_error& err) {
            ASSERT_EQUAL(string(err.what()), json_errros::ReadArrayError02);
        } catch (...) {
            ASSERT(false);
        }
    }
    {
        stringstream ss;
        Node node;
        ss << "[1_2]";
        try {
            ss >> node;
            ASSERT(false);
        } catch (runtime_error& err) {
            ASSERT_EQUAL(string(err.what()), json_errros::ReadArrayError03);
        } catch (...) {
            ASSERT(false);
        }
    }
    {
        stringstream ss;
        Node node;
        ss << "{";
        try {
            ss >> node;
            ASSERT(false);
        } catch (runtime_error& err) {
            ASSERT_EQUAL(string(err.what()), json_errros::ReadMapError01);
        } catch (...) {
            ASSERT(false);
        }
    }
    {
        stringstream ss;
        Node node;
        ss << "{0:0";
        try {
            ss >> node;
            ASSERT(false);
        } catch (runtime_error& err) {
            ASSERT_EQUAL(string(err.what()), json_errros::ReadMapError02);
        } catch (...) {
            ASSERT(false);
        }
    }
    {
        stringstream ss;
        Node node;
        ss << "{0:0}";
        try {
            ss >> node;
            ASSERT(false);
        } catch (runtime_error& err) {
            ASSERT_EQUAL(string(err.what()), json_errros::ReadMapError03);
        } catch (...) {
            ASSERT(false);
        }
    }
    {
        stringstream ss;
        Node node;
        ss << "{\"0\"_0}";
        try {
            ss >> node;
            ASSERT(false);
        } catch (runtime_error& err) {
            ASSERT_EQUAL(string(err.what()), json_errros::ReadMapError03);
        } catch (...) {
            ASSERT(false);
        }
    }
    {
        stringstream ss;
        Node node;
        ss << "{\"10\":4,\"10\":3}";
        try {
            ss >> node;
            ASSERT(false);
        } catch (runtime_error& err) {
            ASSERT_EQUAL(string(err.what()), json_errros::ReadMapError04);
        } catch (...) {
            ASSERT(false);
        }
    }
    
    

    
};

}


