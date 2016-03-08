/****************************************************************************
**
** This file is part of the Ponder library, formerly CAMP.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
** Contact: Tegesoft Information (contact@tegesoft.com)
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
** copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions:
** 
** The above copyright notice and this permission notice shall be included in
** all copies or substantial portions of the Software.
** 
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
** THE SOFTWARE.
**
****************************************************************************/

#include <ponder/class.hpp>
#include <ponder/classget.hpp>
#include <ponder/pondertype.hpp>
#include <ponder/class.hpp>
#include <ponder/classbuilder.hpp>
#include <boost/test/unit_test.hpp>

namespace InheritanceTest
{
    struct MyClass1
    {
        MyClass1() : p1(10), po1(10) {}
        virtual ~MyClass1() {}
        int p1;
        int f1() const {return 1;}
        int po1;
        int fo1() {return 1;}
        PONDER_RTTI();
    };
    
    struct MyClass2
    {
        MyClass2() : p2(20), po2(20) {}
        virtual ~MyClass2() {}
        int p2;
        int f2() const {return 2;}
        virtual int fv() const {return p2;}
        int po2;
        int fo2() {return 2;}
        PONDER_RTTI();
    };
    
    struct MyClass3 : public MyClass1, public MyClass2
    {
        MyClass3() : p3(30), po3(30) {}
        virtual ~MyClass3() {}
        int p3;
        int f3() const {return 3;}
        virtual int fv() const {return p3;}
        int po3;
        int fo3() {return 3;}
        PONDER_RTTI();
    };
    
    struct MyClass4 : public MyClass3
    {
        MyClass4() : p4(40), po4(40) {}
        virtual ~MyClass4() {}
        int p4;
        int f4() const {return 4;}
        virtual int fv() const {return p4;}
        int po4;
        int fo4() {return 4;}
        PONDER_RTTI();
    };
    
    void declare()
    {
        ponder::Class::declare<MyClass1>("InheritanceTest::MyClass1")
        .function("f1", &MyClass1::f1)
        .property("p1", &MyClass1::p1)
        .function("overridden", &MyClass1::fo1)
        .property("overridden", &MyClass1::po1);
        
        ponder::Class::declare<MyClass2>("InheritanceTest::MyClass2")
        .function("f2", &MyClass2::f2)
        .property("p2", &MyClass2::p2)
        .function("virtual", &MyClass2::fv)
        .function("overridden", &MyClass2::fo2)
        .property("overridden", &MyClass2::po2);
        
        ponder::Class::declare<MyClass3>("InheritanceTest::MyClass3")
        .base<MyClass1>()
        .base<MyClass2>()
        .function("f3", &MyClass3::f3)
        .property("p3", &MyClass3::p3)
        .function("overridden", &MyClass3::fo3)
        .property("overridden", &MyClass3::po3);
        
        ponder::Class::declare<MyClass4>("InheritanceTest::MyClass4")
        .base<MyClass3>()
        .function("f4", &MyClass4::f4)
        .property("p4", &MyClass4::p4)
        .function("overridden", &MyClass4::fo4)
        .property("overridden", &MyClass4::po4);
    }
}

PONDER_AUTO_TYPE(InheritanceTest::MyClass1, &InheritanceTest::declare)
PONDER_AUTO_TYPE(InheritanceTest::MyClass2, &InheritanceTest::declare)
PONDER_AUTO_TYPE(InheritanceTest::MyClass3, &InheritanceTest::declare)
PONDER_AUTO_TYPE(InheritanceTest::MyClass4, &InheritanceTest::declare)

using namespace InheritanceTest;

//-----------------------------------------------------------------------------
struct InheritanceFixture
{
    InheritanceFixture()
    {
        class1 = &ponder::classByType<MyClass1>();
        class2 = &ponder::classByType<MyClass2>();
        class3 = &ponder::classByType<MyClass3>();
        class4 = &ponder::classByType<MyClass4>();
    }

    const ponder::Class* class1;
    const ponder::Class* class2;
    const ponder::Class* class3;
    const ponder::Class* class4;
};

//-----------------------------------------------------------------------------
//                         Tests for inherited metaclasses
//-----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_SUITE(INHERITANCE, InheritanceFixture)

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(regularFunctions)
{
    MyClass1 object1;
    MyClass2 object2;
    MyClass3 object3;
    MyClass4 object4;

    BOOST_CHECK_EQUAL(class1->function("f1").call(object1), ponder::Value(1));
    BOOST_CHECK_EQUAL(class3->function("f1").call(object3), ponder::Value(1));
    BOOST_CHECK_EQUAL(class4->function("f1").call(object4), ponder::Value(1));

    BOOST_CHECK_EQUAL(class2->function("f2").call(object2), ponder::Value(2));
    BOOST_CHECK_EQUAL(class3->function("f2").call(object3), ponder::Value(2));
    BOOST_CHECK_EQUAL(class4->function("f2").call(object4), ponder::Value(2));

    BOOST_CHECK_EQUAL(class3->function("f3").call(object3), ponder::Value(3));
    BOOST_CHECK_EQUAL(class4->function("f3").call(object4), ponder::Value(3));

    BOOST_CHECK_EQUAL(class4->function("f4").call(object4), ponder::Value(4));
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(virtualFunctions)
{
    MyClass1 object1;
    MyClass2 object2;
    MyClass3 object3;
    MyClass4 object4;

    BOOST_CHECK_EQUAL(class2->function("virtual").call(object2), ponder::Value(20));
    BOOST_CHECK_EQUAL(class3->function("virtual").call(object3), ponder::Value(30));
    BOOST_CHECK_EQUAL(class4->function("virtual").call(object4), ponder::Value(40));

    BOOST_CHECK_EQUAL(class2->function("virtual").call(object2), ponder::Value(20));
    BOOST_CHECK_EQUAL(class2->function("virtual").call(object3), ponder::Value(30));
    BOOST_CHECK_EQUAL(class2->function("virtual").call(object4), ponder::Value(40));
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(properties)
{
    MyClass1 object1;
    MyClass2 object2;
    MyClass3 object3;
    MyClass4 object4;

    BOOST_CHECK_EQUAL(class1->property("p1").get(object1), ponder::Value(10));
    BOOST_CHECK_EQUAL(class3->property("p1").get(object3), ponder::Value(10));
    BOOST_CHECK_EQUAL(class4->property("p1").get(object4), ponder::Value(10));

    BOOST_CHECK_EQUAL(class2->property("p2").get(object2), ponder::Value(20));
    BOOST_CHECK_EQUAL(class3->property("p2").get(object3), ponder::Value(20));
    BOOST_CHECK_EQUAL(class4->property("p2").get(object4), ponder::Value(20));

    BOOST_CHECK_EQUAL(class3->property("p3").get(object3), ponder::Value(30));
    BOOST_CHECK_EQUAL(class4->property("p3").get(object4), ponder::Value(30));

    BOOST_CHECK_EQUAL(class4->property("p4").get(object4), ponder::Value(40));
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(castWithPointerOffset)
{
    MyClass3 object3;
    MyClass4 object4;

    MyClass2* base3 = &object3;
    MyClass3* base4 = &object4;

    BOOST_CHECK_EQUAL(class2->property("p2").get(base3), ponder::Value(20));
    BOOST_CHECK_EQUAL(class2->property("p2").get(base4), ponder::Value(20));

    BOOST_CHECK_EQUAL(class3->property("p3").get(base3), ponder::Value(30));
    BOOST_CHECK_EQUAL(class3->property("p3").get(base4), ponder::Value(30));

    BOOST_CHECK_EQUAL(class4->property("p4").get(base4), ponder::Value(40));
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(overriddenFunction)
{
    MyClass1 object1;
    MyClass2 object2;
    MyClass3 object3;
    MyClass4 object4;

    BOOST_CHECK_EQUAL(class1->function("overridden").call(object1), ponder::Value(1));
    BOOST_CHECK_EQUAL(class2->function("overridden").call(object2), ponder::Value(2));
    BOOST_CHECK_EQUAL(class3->function("overridden").call(object3), ponder::Value(3));
    BOOST_CHECK_EQUAL(class4->function("overridden").call(object4), ponder::Value(4));

    BOOST_CHECK_EQUAL(class4->function("overridden").call(object1), ponder::Value(4));
    BOOST_CHECK_EQUAL(class4->function("overridden").call(object2), ponder::Value(4));
    BOOST_CHECK_EQUAL(class4->function("overridden").call(object3), ponder::Value(4));
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(overriddenProperty)
{
    MyClass1 object1;
    MyClass2 object2;
    MyClass3 object3;
    MyClass4 object4;

    // Here, the result should always depend on the metaclass, not the object
    // (just like in C++ where non-virtual functions are resolved using the static type)

    BOOST_CHECK_EQUAL(class1->property("overridden").get(object1), ponder::Value(10));
    BOOST_CHECK_EQUAL(class2->property("overridden").get(object2), ponder::Value(20));
    BOOST_CHECK_EQUAL(class3->property("overridden").get(object3), ponder::Value(30));
    BOOST_CHECK_EQUAL(class4->property("overridden").get(object4), ponder::Value(40));

    BOOST_CHECK_EQUAL(class1->property("overridden").get(object3), ponder::Value(10));
    BOOST_CHECK_EQUAL(class2->property("overridden").get(object3), ponder::Value(20));

    BOOST_CHECK_EQUAL(class1->property("overridden").get(object4), ponder::Value(10));
    BOOST_CHECK_EQUAL(class2->property("overridden").get(object4), ponder::Value(20));
    BOOST_CHECK_EQUAL(class3->property("overridden").get(object4), ponder::Value(30));
}

BOOST_AUTO_TEST_SUITE_END()
